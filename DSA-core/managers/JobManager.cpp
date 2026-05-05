#include "JobManager.h"
#include <iostream>

JobManager::JobManager(JobList& jl, BST& b, JobHashTable& ht, Graph& g)
    : jobList(jl), bst(b), hashTable(ht), graph(g) {
    nextId = 1;
}

int JobManager::generateId() {
    return nextId++;
}

void JobManager::addJob(Job job) {
    job.id = generateId();

    // Add to all data structures
    jobList.append(job);
    bst.insert(job);
    hashTable.insert(job);
    graph.addJobNode(job.id);

    std::cout << "Job added successfully with ID: " << job.id << std::endl;
}

void JobManager::removeJob(int id) {
    jobList.remove(id);
    bst.remove(id);
    hashTable.remove(id);
    std::cout << "Job removed successfully." << std::endl;
}

void JobManager::updateJobStatus(int id, bool isOpen) {
    Job* job = hashTable.search(id);
    if (job == nullptr) {
        std::cout << "Job not found." << std::endl;
        return;
    }
    job->isOpen = isOpen;
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
    Sort::quickSort(jobs, 0, jobs.size() - 1);
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

std::vector<Job> JobManager::getMatchingJobsForUser(int userId, UserList& userList) {
    std::vector<int> matchedJobIds = graph.getMatchingJobs(userId);
    std::vector<Job> matchedJobs;

    for (int jobId : matchedJobIds) {
        Job* job = hashTable.search(jobId);
        if (job != nullptr) {
            matchedJobs.push_back(*job);
        }
    }

    return matchedJobs;
}