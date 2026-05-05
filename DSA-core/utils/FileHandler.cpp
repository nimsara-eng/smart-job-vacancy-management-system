#include "FileHandler.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// ─── JOBS ────────────────────────────────────────────────────────────────────

std::vector<Job> FileHandler::loadJobs(const std::string& filename) {
    std::vector<Job> jobs;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No jobs file found. Starting fresh." << std::endl;
        return jobs;
    }

    json data;
    file >> data;

    for (auto& item : data) {
        Job job;
        job.id          = item["id"];
        job.title       = item["title"];
        job.company     = item["company"];
        job.location    = item["location"];
        job.salary      = item["salary"];
        job.description = item["description"];
        job.category    = item["category"];
        job.isOpen      = item["isOpen"];
        job.isRemote    = item["isRemote"];

        for (auto& skill : item["requiredSkills"]) {
            job.requiredSkills.push_back(skill);
        }

        jobs.push_back(job);
    }

    return jobs;
}

void FileHandler::saveJobs(const std::string& filename, const std::vector<Job>& jobs) {
    json data = json::array();

    for (const Job& job : jobs) {
        json item;
        item["id"]             = job.id;
        item["title"]          = job.title;
        item["company"]        = job.company;
        item["location"]       = job.location;
        item["salary"]         = job.salary;
        item["description"]    = job.description;
        item["category"]       = job.category;
        item["isOpen"]         = job.isOpen;
        item["isRemote"]       = job.isRemote;
        item["requiredSkills"] = job.requiredSkills;

        data.push_back(item);
    }

    std::ofstream file(filename);
    file << data.dump(4);
    std::cout << "Jobs saved successfully." << std::endl;
}

// ─── USERS ───────────────────────────────────────────────────────────────────

std::vector<User> FileHandler::loadUsers(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No users file found. Starting fresh." << std::endl;
        return users;
    }

    json data;
    file >> data;

    for (auto& item : data) {
        User user;
        user.id       = item["id"];
        user.username = item["username"];
        user.password = item["password"];
        user.email    = item["email"];
        user.name     = item["name"];
        user.phone    = item["phone"];
        user.role     = item["role"];

        for (auto& skill : item["skills"]) {
            user.skills.push_back(skill);
        }

        users.push_back(user);
    }

    return users;
}

void FileHandler::saveUsers(const std::string& filename, const std::vector<User>& users) {
    json data = json::array();

    for (const User& user : users) {
        json item;
        item["id"]       = user.id;
        item["username"] = user.username;
        item["password"] = user.password;
        item["email"]    = user.email;
        item["name"]     = user.name;
        item["phone"]    = user.phone;
        item["role"]     = user.role;
        item["skills"]   = user.skills;

        data.push_back(item);
    }

    std::ofstream file(filename);
    file << data.dump(4);
    std::cout << "Users saved successfully." << std::endl;
}

// ─── APPLICATIONS ─────────────────────────────────────────────────────────────

std::vector<Application> FileHandler::loadApplications(const std::string& filename) {
    std::vector<Application> applications;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No applications file found. Starting fresh." << std::endl;
        return applications;
    }

    json data;
    file >> data;

    for (auto& item : data) {
        Application app;
        app.applicationId = item["applicationId"];
        app.userId        = item["userId"];
        app.jobId         = item["jobId"];
        app.status        = item["status"];
        app.appliedDate   = item["appliedDate"];

        applications.push_back(app);
    }

    return applications;
}

void FileHandler::saveApplications(const std::string& filename, const std::vector<Application>& applications) {
    json data = json::array();

    for (const Application& app : applications) {
        json item;
        item["applicationId"] = app.applicationId;
        item["userId"]        = app.userId;
        item["jobId"]         = app.jobId;
        item["status"]        = app.status;
        item["appliedDate"]   = app.appliedDate;

        data.push_back(item);
    }

    std::ofstream file(filename);
    file << data.dump(4);
    std::cout << "Applications saved successfully." << std::endl;
}