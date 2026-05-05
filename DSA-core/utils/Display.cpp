#include "Display.h"
#include <iostream>
#include <iomanip>

// ─── MENUS ───────────────────────────────────────────────────────────────────

void Display::showWelcome() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   SMART JOB VACANCY MANAGEMENT SYSTEM \n";
    std::cout << "========================================\n";
}

void Display::showGoodbye() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   Thank you for using SJVMS. Goodbye!  \n";
    std::cout << "========================================\n";
}

void Display::showMainMenu() {
    std::cout << "\n--- MAIN MENU ---\n";
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
}

void Display::showEmployerMenu() {
    std::cout << "\n--- EMPLOYER MENU ---\n";
    std::cout << "1. Post a Job\n";
    std::cout << "2. View My Jobs\n";
    std::cout << "3. View Applications for a Job\n";
    std::cout << "4. Process Next Application\n";
    std::cout << "5. Update Job Status (Open/Close)\n";
    std::cout << "6. Remove a Job\n";
    std::cout << "0. Logout\n";
    std::cout << "Enter choice: ";
}

void Display::showApplicantMenu() {
    std::cout << "\n--- APPLICANT MENU ---\n";
    std::cout << "1. Browse All Jobs\n";
    std::cout << "2. Search Jobs by Title\n";
    std::cout << "3. Search Jobs by Category\n";
    std::cout << "4. Search Jobs by Location\n";
    std::cout << "5. Search Jobs by Salary Range\n";
    std::cout << "6. View Matching Jobs (Based on My Skills)\n";
    std::cout << "7. Apply for a Job\n";
    std::cout << "8. View My Applications\n";
    std::cout << "0. Logout\n";
    std::cout << "Enter choice: ";
}

// ─── JOB DISPLAY ─────────────────────────────────────────────────────────────

void Display::showJobDetails(const Job& job) {
    std::cout << "\n--- JOB DETAILS ---\n";
    std::cout << "ID       : " << job.id << "\n";
    std::cout << "Title    : " << job.title << "\n";
    std::cout << "Company  : " << job.company << "\n";
    std::cout << "Location : " << job.location << "\n";
    std::cout << "Salary   : $" << std::fixed << std::setprecision(2) << job.salary << "\n";
    std::cout << "Category : " << job.category << "\n";
    std::cout << "Remote   : " << (job.isRemote ? "Yes" : "No") << "\n";
    std::cout << "Status   : " << (job.isOpen ? "Open" : "Closed") << "\n";
    std::cout << "Description: " << job.description << "\n";
    std::cout << "Required Skills: ";
    for (const std::string& skill : job.requiredSkills) {
        std::cout << skill << " ";
    }
    std::cout << "\n";
}

void Display::showJobList(const std::vector<Job>& jobs) {
    if (jobs.empty()) {
        std::cout << "No jobs found.\n";
        return;
    }
    std::cout << "\n--- JOB LIST ---\n";
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(25) << "Title"
              << std::setw(20) << "Company"
              << std::setw(12) << "Salary"
              << std::setw(10) << "Status"
              << "\n";
    std::cout << std::string(72, '-') << "\n";

    for (const Job& job : jobs) {
        std::cout << std::left
                  << std::setw(5)  << job.id
                  << std::setw(25) << job.title
                  << std::setw(20) << job.company
                  << std::setw(12) << job.salary
                  << std::setw(10) << (job.isOpen ? "Open" : "Closed")
                  << "\n";
    }
}

// ─── USER DISPLAY ────────────────────────────────────────────────────────────

void Display::showUserProfile(const User& user) {
    std::cout << "\n--- USER PROFILE ---\n";
    std::cout << "ID       : " << user.id << "\n";
    std::cout << "Name     : " << user.name << "\n";
    std::cout << "Username : " << user.username << "\n";
    std::cout << "Email    : " << user.email << "\n";
    std::cout << "Phone    : " << user.phone << "\n";
    std::cout << "Role     : " << user.role << "\n";
    std::cout << "Skills   : ";
    for (const std::string& skill : user.skills) {
        std::cout << skill << " ";
    }
    std::cout << "\n";
}

// ─── APPLICATION DISPLAY ─────────────────────────────────────────────────────

void Display::showApplicationDetails(const Application& app) {
    std::cout << "\n--- APPLICATION DETAILS ---\n";
    std::cout << "Application ID : " << app.applicationId << "\n";
    std::cout << "Job ID         : " << app.jobId << "\n";
    std::cout << "User ID        : " << app.userId << "\n";
    std::cout << "Status         : " << app.status << "\n";
    std::cout << "Applied Date   : " << app.appliedDate << "\n";
}

void Display::showApplicationList(const std::vector<Application>& apps) {
    if (apps.empty()) {
        std::cout << "No applications found.\n";
        return;
    }
    std::cout << "\n--- APPLICATION LIST ---\n";
    std::cout << std::left
              << std::setw(8)  << "App ID"
              << std::setw(10) << "Job ID"
              << std::setw(10) << "User ID"
              << std::setw(12) << "Status"
              << std::setw(15) << "Date"
              << "\n";
    std::cout << std::string(55, '-') << "\n";

    for (const Application& app : apps) {
        std::cout << std::left
                  << std::setw(8)  << app.applicationId
                  << std::setw(10) << app.jobId
                  << std::setw(10) << app.userId
                  << std::setw(12) << app.status
                  << std::setw(15) << app.appliedDate
                  << "\n";
    }
}

// ─── MESSAGES ────────────────────────────────────────────────────────────────

void Display::showSuccess(const std::string& message) {
    std::cout << "[SUCCESS] " << message << "\n";
}

void Display::showError(const std::string& message) {
    std::cout << "[ERROR] " << message << "\n";
}

// ─── INPUT HELPERS ───────────────────────────────────────────────────────────

int Display::getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    std::cin >> value;
    std::cin.ignore();
    return value;
}

float Display::getFloatInput(const std::string& prompt) {
    float value;
    std::cout << prompt;
    std::cin >> value;
    std::cin.ignore();
    return value;
}

std::string Display::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}