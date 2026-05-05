#pragma once
#include <string>
#include <vector>
#include "../models/Job.h"
#include "../models/User.h"
#include "../models/Application.h"

class FileHandler {
public:
    // Jobs
    static std::vector<Job> loadJobs(const std::string& filename);
    static void saveJobs(const std::string& filename, const std::vector<Job>& jobs);

    // Users
    static std::vector<User> loadUsers(const std::string& filename);
    static void saveUsers(const std::string& filename, const std::vector<User>& users);

    // Applications
    static std::vector<Application> loadApplications(const std::string& filename);
    static void saveApplications(const std::string& filename, const std::vector<Application>& applications);
};