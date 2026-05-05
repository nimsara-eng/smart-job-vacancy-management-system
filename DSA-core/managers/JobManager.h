#pragma once
#include <vector>
#include <string>
#include "../data-structures/LinkedList.h"
#include "../data-structures/BST.h"
#include "../data-structures/HashTable.h"
#include "../data-structures/Graph.h"
#include "../algorithms/Sort.h"
#include "../algorithms/Search.h"
#include "../models/Job.h"

class JobManager {
private:
    JobList& jobList;
    BST& bst;
    JobHashTable& hashTable;
    Graph& graph;

    int nextId;

public:
    JobManager(JobList& jl, BST& b, JobHashTable& ht, Graph& g);

    void addJob(Job job);
    void removeJob(int id);
    void updateJobStatus(int id, bool isOpen);

    Job* getJobById(int id);
    std::vector<Job> getAllJobs();
    std::vector<Job> getJobsSortedBySalary();
    std::vector<Job> getJobsSortedByTitle();
    std::vector<Job> searchByTitle(std::string keyword);
    std::vector<Job> searchByCategory(std::string category);
    std::vector<Job> searchByLocation(std::string location);
    std::vector<Job> searchBySalaryRange(float minSalary, float maxSalary);
    std::vector<Job> getMatchingJobsForUser(int userId, UserList& userList);

    int generateId();
};