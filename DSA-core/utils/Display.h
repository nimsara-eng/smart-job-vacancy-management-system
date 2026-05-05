#pragma once
#include <string>
#include <vector>
#include "../models/Job.h"
#include "../models/User.h"
#include "../models/Application.h"

class Display {
public:
    // Menus
    static void showMainMenu();
    static void showEmployerMenu();
    static void showApplicantMenu();

    // Job display
    static void showJobDetails(const Job& job);
    static void showJobList(const std::vector<Job>& jobs);

    // User display
    static void showUserProfile(const User& user);

    // Application display
    static void showApplicationDetails(const Application& app);
    static void showApplicationList(const std::vector<Application>& apps);

    // Messages
    static void showSuccess(const std::string& message);
    static void showError(const std::string& message);
    static void showWelcome();
    static void showGoodbye();

    // Input helpers
    static int getIntInput(const std::string& prompt);
    static float getFloatInput(const std::string& prompt);
    static std::string getStringInput(const std::string& prompt);
};