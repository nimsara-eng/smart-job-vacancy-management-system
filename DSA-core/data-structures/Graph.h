#pragma once
#include <string>
#include <vector>
#include "../models/Job.h"
#include "../models/User.h"

class Graph {
private:

    // Represents a connection between a user and a job
    struct Edge {
        int userId;
        int jobId;
        Edge* next;

        Edge(int uid, int jid) {
            userId = uid;
            jobId = jid;
            next = nullptr;
        }
    };

    // Represents a node — either a User or a Job
    struct GraphNode {
        int id;
        std::string type; // "user" or "job"
        GraphNode* next;

        GraphNode(int i, std::string t) {
            id = i;
            type = t;
            next = nullptr;
        }
    };

    GraphNode* nodes;  // linked list of all nodes
    Edge* edges;       // linked list of all edges

    bool hasSkillMatch(User& user, Job& job) {
        for (std::string& userSkill : user.skills) {
            for (std::string& jobSkill : job.requiredSkills) {
                if (userSkill == jobSkill) {
                    return true;
                }
            }
        }
        return false;
    }

public:

    Graph() {
        nodes = nullptr;
        edges = nullptr;
    }

    void addUserNode(int userId) {
        GraphNode* newNode = new GraphNode(userId, "user");
        newNode->next = nodes;
        nodes = newNode;
    }

    void addJobNode(int jobId) {
        GraphNode* newNode = new GraphNode(jobId, "job");
        newNode->next = nodes;
        nodes = newNode;
    }

    void buildEdges(User& user, Job& job) {
        if (hasSkillMatch(user, job)) {
            Edge* newEdge = new Edge(user.id, job.id);
            newEdge->next = edges;
            edges = newEdge;
        }
    }

    // Get all job IDs that match a user
    std::vector<int> getMatchingJobs(int userId) {
        std::vector<int> matchedJobs;
        Edge* current = edges;
        while (current != nullptr) {
            if (current->userId == userId) {
                matchedJobs.push_back(current->jobId);
            }
            current = current->next;
        }
        return matchedJobs;
    }

    // Get all user IDs that match a job
    std::vector<int> getMatchingUsers(int jobId) {
        std::vector<int> matchedUsers;
        Edge* current = edges;
        while (current != nullptr) {
            if (current->jobId == jobId) {
                matchedUsers.push_back(current->userId);
            }
            current = current->next;
        }
        return matchedUsers;
    }

    ~Graph() {
        // Delete all edges
        Edge* currentEdge = edges;
        while (currentEdge != nullptr) {
            Edge* temp = currentEdge;
            currentEdge = currentEdge->next;
            delete temp;
        }

        // Delete all nodes
        GraphNode* currentNode = nodes;
        while (currentNode != nullptr) {
            GraphNode* temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
};