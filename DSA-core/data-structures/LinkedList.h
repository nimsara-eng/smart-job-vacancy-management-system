#pragma once
#include <vector>
#include "../models/Job.h"
#include "../models/User.h"
#include "../models/Application.h"

// ─── JOB LINKED LIST ─────────────────────────────────────────────────────────

class JobList {
private:
    struct Node {
        Job job;
        Node* next;

        Node(Job j) {
            job = j;
            next = nullptr;
        }
    };

    Node* head;
    int size;

public:

    JobList() {
        head = nullptr;
        size = 0;
    }

    void append(Job job) {
        Node* newNode = new Node(job);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void remove(int id) {
        if (head == nullptr) return;

        if (head->job.id == id) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->job.id == id) {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
                size--;
                return;
            }
            current = current->next;
        }
    }

    Job* findById(int id) {
        Node* current = head;
        while (current != nullptr) {
            if (current->job.id == id) {
                return &current->job;
            }
            current = current->next;
        }
        return nullptr;
    }

    std::vector<Job> getAll() {
        std::vector<Job> result;
        Node* current = head;
        while (current != nullptr) {
            result.push_back(current->job);
            current = current->next;
        }
        return result;
    }

    int getSize() {
        return size;
    }

    // BUG FIX: original destructor was correct; no change needed here.
    ~JobList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

// ─── USER LINKED LIST ─────────────────────────────────────────────────────────

class UserList {
private:
    struct Node {
        User user;
        Node* next;

        Node(User u) {
            user = u;
            next = nullptr;
        }
    };

    Node* head;
    int size;

public:

    UserList() {
        head = nullptr;
        size = 0;
    }

    void append(User user) {
        Node* newNode = new Node(user);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void remove(int id) {
        if (head == nullptr) return;

        if (head->user.id == id) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->user.id == id) {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
                size--;
                return;
            }
            current = current->next;
        }
    }

    User* findById(int id) {
        Node* current = head;
        while (current != nullptr) {
            if (current->user.id == id) {
                return &current->user;
            }
            current = current->next;
        }
        return nullptr;
    }

    std::vector<User> getAll() {
        std::vector<User> result;
        Node* current = head;
        while (current != nullptr) {
            result.push_back(current->user);
            current = current->next;
        }
        return result;
    }

    int getSize() {
        return size;
    }

    ~UserList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

// ─── APPLICATION LINKED LIST ──────────────────────────────────────────────────

class ApplicationList {
private:
    struct Node {
        Application application;
        Node* next;

        Node(Application a) {
            application = a;
            next = nullptr;
        }
    };

    Node* head;
    int size;

public:

    ApplicationList() {
        head = nullptr;
        size = 0;
    }

    void append(Application application) {
        Node* newNode = new Node(application);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void remove(int id) {
        if (head == nullptr) return;

        if (head->application.applicationId == id) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->application.applicationId == id) {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
                size--;
                return;
            }
            current = current->next;
        }
    }

    Application* findById(int id) {
        Node* current = head;
        while (current != nullptr) {
            if (current->application.applicationId == id) {
                return &current->application;
            }
            current = current->next;
        }
        return nullptr;
    }

    std::vector<Application> getAll() {
        std::vector<Application> result;
        Node* current = head;
        while (current != nullptr) {
            result.push_back(current->application);
            current = current->next;
        }
        return result;
    }

    int getSize() {
        return size;
    }

    ~ApplicationList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};