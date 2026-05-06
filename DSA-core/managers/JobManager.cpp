#include "JobManager.h"
#include <iostream>

JobManager::JobManager(JobList& jl, BST& b, JobHashTable& ht, Graph& g)
    : jobList(jl), bst(b), hashTable(ht), graph(g) {
    nextId = 1;
}

int JobManager::generateId() {
    return nextId++;
}

// BUG FIX: addJob now accepts UserList so graph edges can be built between
// the new job and ALL existing users. Without this, users registered before
// a job was posted would never get skill-match edges for that job.
void JobManager::addJob(Job job, UserList& userList) {
    job.id = generateId();

    jobList.append(job);
    bst.insert(job);
    hashTable.insert(job);
    graph.addJobNode(job.id);

    // Build edges between this new job and all existing users
    std::vector<User> allUsers = userList.getAll();
    for (User& user : allUsers) {
        graph.buildEdges(user, job);
    }

    std::cout << "Job added successfully with ID: " << job.id << std::endl;
}

void JobManager::removeJob(int id) {
    jobList.remove(id);

    // BUG FIX: was bst.remove(id) which called remove(float salary) with an int id.
    // This caused wrong BST nodes to be deleted (e.g., job id=3 would delete
    // a job with salary=3.0 instead of the correct job).
    bst.removeById(id);

    hashTable.remove(id);
    std::cout << "Job removed successfully." << std::endl;
}

void JobManager::updateJobStatus(int id, bool isOpen) {
    // BUG FIX: original code only updated the hash table copy.
    // getAllJobs() reads from LinkedList, so status change was invisible there.
    // Now we update BOTH the hash table node and the linked list node.

    Job* hashJob = hashTable.search(id);
    if (hashJob == nullptr) {
        std::cout << "Job not found." << std::endl;
        return;
    }
    hashJob->isOpen = isOpen;

    Job* listJob = jobList.findById(id);
    if (listJob != nullptr) {
        listJob->isOpen = isOpen;
    }

    std::cout << "Job status updated." << std::endl;
}

Job* JobManager::getJobById(int id) {
    return hashTable.search(id);
}

std::vector<Job> JobManager::getAllJobs() {
    return jobList.getAll();
}

std::vector<Job> JobManager::getJobsSortedBySalary() {
    std::vector<Job> jobs = jobList.getAll();

    // BUG FIX: jobs.size() returns size_t (unsigned). When jobs is empty,
    // jobs.size() - 1 wraps around to a huge number, causing undefined behavior
    // in quickSort. Guard against empty list before calling.
    if (jobs.size() > 1) {
        Sort::quickSort(jobs, 0, (int)jobs.size() - 1);
    }
    return jobs;
}

std::vector<Job> JobManager::getJobsSortedByTitle() {
    std::vector<Job> jobs = jobList.getAll();
    Sort::sortByTitle(jobs);
    return jobs;
}

std::vector<Job> JobManager::searchByTitle(std::string keyword) {
    std::vector<Job> jobs = jobList.getAll();
    return Search::linearSearchByTitle(jobs, keyword);
}

std::vector<Job> JobManager::searchByCategory(std::string category) {
    std::vector<Job> jobs = jobList.getAll();
    return Search::linearSearchByCategory(jobs, category);
}

std::vector<Job> JobManager::searchByLocation(std::string location) {
    std::vector<Job> jobs = jobList.getAll();
    return Search::linearSearchByLocation(jobs, location);
}

std::vector<Job> JobManager::searchBySalaryRange(float minSalary, float maxSalary) {
    return bst.searchByRange(minSalary, maxSalary);
}

// BUG FIX: removed unused UserList& parameter - graph already has the edges built.
std::vector<Job> JobManager::getMatchingJobsForUser(int userId) {
    std::vector<int> matchedJobIds = graph.getMatchingJobs(userId);
    std::vector<Job> matchedJobs;

    for (int jobId : matchedJobIds) {
        Job* job = hashTable.search(jobId);
        if (job != nullptr && job->isOpen) {  // only return open jobs
            matchedJobs.push_back(*job);
        }
    }

    return matchedJobs;
}