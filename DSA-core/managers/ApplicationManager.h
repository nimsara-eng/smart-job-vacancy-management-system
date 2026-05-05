#pragma once
#include <string>
#include <vector>
#include "../data-structures/LinkedList.h"
#include "../data-structures/Queue.h"
#include "../models/Application.h"

class ApplicationManager {
private:
    ApplicationList& applicationList;
    Queue& applicationQueue;

    int nextId;

public:
    ApplicationManager(ApplicationList& al, Queue& q);

    void submitApplication(Application application);
    void processNextApplication();
    void updateStatus(int applicationId, std::string status);

    Application* getApplicationById(int id);
    std::vector<Application> getAllApplications();
    std::vector<Application> getApplicationsByJob(int jobId);
    std::vector<Application> getApplicationsByUser(int userId);

    Application* peekNextInQueue();
    bool isQueueEmpty();

    int generateId();
};