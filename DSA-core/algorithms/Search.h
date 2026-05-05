#pragma once
#include <vector>
#include <string>
#include "../models/Job.h"

class Search {
public:

    // Search by title keyword - unsorted list
    static std::vector<Job> linearSearchByTitle(std::vector<Job>& jobs, std::string keyword) {
        std::vector<Job> result;
        for (Job& job : jobs) {
            if (job.title.find(keyword) != std::string::npos) {
                result.push_back(job);
            }
        }
        return result;
    }

    // Search by category - unsorted list
    static std::vector<Job> linearSearchByCategory(std::vector<Job>& jobs, std::string category) {
        std::vector<Job> result;
        for (Job& job : jobs) {
            if (job.category == category) {
                result.push_back(job);
            }
        }
        return result;
    }

    // Search by location
    static std::vector<Job> linearSearchByLocation(std::vector<Job>& jobs, std::string location) {
        std::vector<Job> result;
        for (Job& job : jobs) {
            if (job.location.find(location) != std::string::npos) {
                result.push_back(job);
            }
        }
        return result;
    }

    // Binary search by salary - list MUST be sorted first
    static int binarySearchBySalary(std::vector<Job>& jobs, float targetSalary) {
        int low = 0;
        int high = jobs.size() - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            if (jobs[mid].salary == targetSalary) {
                return mid; // returns index
            } else if (jobs[mid].salary < targetSalary) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        return -1; // not found
    }
};