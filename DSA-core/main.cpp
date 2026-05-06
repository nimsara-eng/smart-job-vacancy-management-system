#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "models/Job.h"
#include "models/User.h"
#include "models/Application.h"

#include "data-structures/LinkedList.h"
#include "data-structures/BST.h"
#include "data-structures/HashTable.h"
#include "data-structures/Graph.h"
#include "data-structures/Queue.h"
#include "data-structures/Stack.h"

#include "managers/JobManager.h"
#include "managers/UserManager.h"
#include "managers/ApplicationManager.h"

#include "utils/FileHandler.h"
#include "utils/Display.h"

const std::string JOBS_FILE         = "../data/jobs.json";
const std::string USERS_FILE        = "../data/users.json";
const std::string APPLICATIONS_FILE = "../data/applications.json";

std::string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             1900 + ltm->tm_year,
             1 + ltm->tm_mon,
             ltm->tm_mday);
    return std::string(buf);
}

// ─── SKILL INPUT FIX ──────────────────────────────────────────────────────────
// Asks one skill at a time, max 5, stops immediately on empty Enter
std::vector<std::string> collectSkills(int maxCount) {
    std::vector<std::string> skills;
    std::cout << "Enter up to " << maxCount << " skills (leave blank and press Enter to stop):\n";
    for (int i = 0; i < maxCount; i++) {
        std::cout << "  Skill " << (i + 1) << ": ";
        std::string skill;
        std::getline(std::cin, skill);
        if (skill.empty()) break;   // ← stops on empty Enter
        skills.push_back(skill);
    }
    return skills;
}

void runEmployerMenu(User* currentUser, JobManager& jobManager, ApplicationManager& appManager, UserList& userList);
void runApplicantMenu(User* currentUser, JobManager& jobManager, ApplicationManager& appManager);

void runEmployerMenu(User* currentUser, JobManager& jobManager, ApplicationManager& appManager, UserList& userList) {
    int choice = -1;
    while (choice != 0) {
        Display::showEmployerMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                Job job;
                job.company     = currentUser->name;
                job.title       = Display::getStringInput("Job Title: ");
                job.location    = Display::getStringInput("Location: ");
                job.category    = Display::getStringInput("Category (e.g. IT, Finance, Health): ");
                job.description = Display::getStringInput("Description: ");
                job.salary      = Display::getFloatInput("Salary: ");
                std::string remote = Display::getStringInput("Is it remote? (y/n): ");
                job.isRemote = (remote == "y" || remote == "Y");
                job.requiredSkills = collectSkills(5);
                job.isOpen = true;
                jobManager.addJob(job, userList);
                break;
            }
            case 2: {
                std::vector<Job> allJobs = jobManager.getAllJobs();
                std::vector<Job> myJobs;
                for (Job& j : allJobs) {
                    if (j.company == currentUser->name) myJobs.push_back(j);
                }
                Display::showJobList(myJobs);
                break;
            }
            case 3: {
                int jobId = Display::getIntInput("Enter Job ID: ");
                std::vector<Application> apps = appManager.getApplicationsByJob(jobId);
                Display::showApplicationList(apps);
                break;
            }
            case 4: {
                if (appManager.isQueueEmpty()) {
                    Display::showError("No applications in the queue.");
                } else {
                    Application* next = appManager.peekNextInQueue();
                    Display::showApplicationDetails(*next);
                    std::string action = Display::getStringInput("(approve/reject/skip): ");
                    if (action == "approve") {
                        appManager.updateStatus(next->applicationId, "approved");
                        appManager.processNextApplication();
                        Display::showSuccess("Application approved.");
                    } else if (action == "reject") {
                        appManager.updateStatus(next->applicationId, "rejected");
                        appManager.processNextApplication();
                        Display::showSuccess("Application rejected.");
                    } else {
                        std::cout << "Skipped.\n";
                    }
                }
                break;
            }
            case 5: {
                int jobId = Display::getIntInput("Enter Job ID: ");
                std::string statusStr = Display::getStringInput("Set status (open/close): ");
                bool isOpen = (statusStr == "open" || statusStr == "Open");
                jobManager.updateJobStatus(jobId, isOpen);
                break;
            }
            case 6: {
                int jobId = Display::getIntInput("Enter Job ID to remove: ");
                jobManager.removeJob(jobId);
                break;
            }
            case 0:
                Display::showSuccess("Logged out.");
                break;
            default:
                Display::showError("Invalid choice. Try again.");
        }
    }
}

void runApplicantMenu(User* currentUser, JobManager& jobManager, ApplicationManager& appManager) {
    int choice = -1;
    while (choice != 0) {
        Display::showApplicantMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::vector<Job> jobs = jobManager.getJobsSortedByTitle();
                Display::showJobList(jobs);
                break;
            }
            case 2: {
                std::string keyword = Display::getStringInput("Enter title keyword: ");
                Display::showJobList(jobManager.searchByTitle(keyword));
                break;
            }
            case 3: {
                std::string category = Display::getStringInput("Enter category: ");
                Display::showJobList(jobManager.searchByCategory(category));
                break;
            }
            case 4: {
                std::string location = Display::getStringInput("Enter location: ");
                Display::showJobList(jobManager.searchByLocation(location));
                break;
            }
            case 5: {
                float minSalary = Display::getFloatInput("Min salary: ");
                float maxSalary = Display::getFloatInput("Max salary: ");
                Display::showJobList(jobManager.searchBySalaryRange(minSalary, maxSalary));
                break;
            }
            case 6: {
                std::vector<Job> matched = jobManager.getMatchingJobsForUser(currentUser->id);
                if (matched.empty()) {
                    std::cout << "No matching jobs found based on your skills.\n";
                } else {
                    Display::showJobList(matched);
                }
                break;
            }
            case 7: {
                int jobId = Display::getIntInput("Enter Job ID to apply for: ");
                Job* job = jobManager.getJobById(jobId);
                if (job == nullptr) { Display::showError("Job not found."); break; }
                if (!job->isOpen)   { Display::showError("This job is closed."); break; }

                std::vector<Application> myApps = appManager.getApplicationsByUser(currentUser->id);
                bool alreadyApplied = false;
                for (Application& app : myApps) {
                    if (app.jobId == jobId) { alreadyApplied = true; break; }
                }
                if (alreadyApplied) { Display::showError("Already applied for this job."); break; }

                Application app;
                app.userId      = currentUser->id;
                app.jobId       = jobId;
                app.appliedDate = getCurrentDate();
                appManager.submitApplication(app);
                Display::showSuccess("Application submitted!");
                break;
            }
            case 8: {
                std::vector<Application> myApps = appManager.getApplicationsByUser(currentUser->id);
                if (myApps.empty()) std::cout << "No applications yet.\n";
                else Display::showApplicationList(myApps);
                break;
            }
            case 0:
                Display::showSuccess("Logged out.");
                break;
            default:
                Display::showError("Invalid choice. Try again.");
        }
    }
}

int main() {
    JobList         jobList;
    UserList        userList;
    ApplicationList applicationList;
    BST             bst;
    JobHashTable    jobHashTable;
    UserHashTable   userHashTable;
    Graph           graph;
    Queue           applicationQueue;

    JobManager         jobManager(jobList, bst, jobHashTable, graph);
    UserManager        userManager(userList, userHashTable, graph);
    ApplicationManager appManager(applicationList, applicationQueue);

    std::vector<Job>         savedJobs  = FileHandler::loadJobs(JOBS_FILE);
    std::vector<User>        savedUsers = FileHandler::loadUsers(USERS_FILE);
    std::vector<Application> savedApps  = FileHandler::loadApplications(APPLICATIONS_FILE);

    for (Job& job : savedJobs) {
        jobList.append(job);
        bst.insert(job);
        jobHashTable.insert(job);
        graph.addJobNode(job.id);
    }
    for (User& user : savedUsers) {
        userList.append(user);
        userHashTable.insert(user);
        graph.addUserNode(user.id);
        userManager.buildGraphEdges(user, jobList);
    }
    for (Application& app : savedApps) {
        applicationList.append(app);
        if (app.status == "pending") applicationQueue.enqueue(app);
    }

    Display::showWelcome();

    int choice = -1;
    while (choice != 0) {
        Display::showMainMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string username = Display::getStringInput("Username: ");
                std::string password = Display::getStringInput("Password: ");
                User* loggedIn = userManager.loginUser(username, password);
                if (loggedIn == nullptr) {
                    Display::showError("Login failed.");
                    break;
                }
                Display::showSuccess("Welcome, " + loggedIn->name + "!");
                Display::showUserProfile(*loggedIn);
                if (loggedIn->role == "employer") {
                    runEmployerMenu(loggedIn, jobManager, appManager, userList);
                } else {
                    runApplicantMenu(loggedIn, jobManager, appManager);
                }
                FileHandler::saveJobs(JOBS_FILE, jobList.getAll());
                FileHandler::saveUsers(USERS_FILE, userList.getAll());
                FileHandler::saveApplications(APPLICATIONS_FILE, applicationList.getAll());
                break;
            }
            case 2: {
                User newUser;
                newUser.name     = Display::getStringInput("Full Name: ");
                newUser.username = Display::getStringInput("Username: ");
                newUser.password = Display::getStringInput("Password: ");
                newUser.email    = Display::getStringInput("Email: ");
                newUser.phone    = Display::getStringInput("Phone: ");
                std::string role = Display::getStringInput("Role (applicant/employer): ");
                if (role != "employer") role = "applicant";
                newUser.role = role;
                if (role == "applicant") {
                    newUser.skills = collectSkills(5);  // ← max 5, stops on empty
                }
                userManager.registerUser(newUser, jobList);
                FileHandler::saveUsers(USERS_FILE, userList.getAll());
                break;
            }
            case 0:
                FileHandler::saveJobs(JOBS_FILE, jobList.getAll());
                FileHandler::saveUsers(USERS_FILE, userList.getAll());
                FileHandler::saveApplications(APPLICATIONS_FILE, applicationList.getAll());
                Display::showGoodbye();
                break;
            default:
                Display::showError("Invalid choice. Try again.");
        }
    }
    return 0;
}