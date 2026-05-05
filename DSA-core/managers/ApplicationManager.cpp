#include "ApplicationManager.h"
#include <iostream>

ApplicationManager::ApplicationManager(ApplicationList& al, Queue& q)
    : applicationList(al), applicationQueue(q) {
    nextId = 1;
}

int ApplicationManager::generateId() {
    return nextId++;
}

void ApplicationManager::submitApplication(Application application) {
    application.applicationId = generateId();
    application.status = "pending";

    applicationList.append(application);
    applicationQueue.enqueue(application);

    std::cout << "Application submitted successfully with ID: "
              << application.applicationId << std::endl;
}

void ApplicationManager::processNextApplication() {
    if (applicationQueue.isEmpty()) {
        std::cout << "No applications in queue." << std::endl;
        return;
    }

    Application* next = applicationQueue.peek();
    std::cout << "Processing application ID: " << next->applicationId
              << " for Job ID: " << next->jobId << std::endl;

    applicationQueue.dequeue();
}

void ApplicationManager::updateStatus(int applicationId, std::string status) {
    Application* app = applicationList.findById(applicationId);
    if (app == nullptr) {
        std::cout << "Application not found." << std::endl;
        return;
    }
    app->status = status;
    std::cout << "Application status updated to: " << status << std::endl;
}

Application* ApplicationManager::getApplicationById(int id) {
    return applicationList.findById(id);
}

std::vector<Application> ApplicationManager::getAllApplications() {
    return applicationList.getAll();
}

std::vector<Application> ApplicationManager::getApplicationsByJob(int jobId) {
    std::vector<Application> result;
    std::vector<Application> all = applicationList.getAll();
    for (Application& app : all) {
        if (app.jobId == jobId) {
            result.push_back(app);
        }
    }
    return result;
}

std::vector<Application> ApplicationManager::getApplicationsByUser(int userId) {
    std::vector<Application> result;
    std::vector<Application> all = applicationList.getAll();
    for (Application& app : all) {
        if (app.userId == userId) {
            result.push_back(app);
        }
    }
    return result;
}

Application* ApplicationManager::peekNextInQueue() {
    return applicationQueue.peek();
}

bool ApplicationManager::isQueueEmpty() {
    return applicationQueue.isEmpty();
}