#pragma once
#include <string>
#include "../models/Job.h"
#include "../models/User.h"

// ─── Job Hash Table ───────────────────────────────────────────────────────────

class JobHashTable {
private:
    struct Node {
        Job job;
        Node* next;
        Node(Job j) : job(j), next(nullptr) {}
    };

    static const int TABLE_SIZE = 100;
    Node* table[TABLE_SIZE];

    int hashFunction(int id) {
        return id % TABLE_SIZE;
    }

public:
    JobHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    void insert(Job job) {
        int index = hashFunction(job.id);
        Node* newNode = new Node(job);
        newNode->next = table[index];
        table[index] = newNode;
    }

    Job* search(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->job.id == id) return &current->job;
            current = current->next;
        }
        return nullptr;
    }

    void remove(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->job.id == id) {
                if (prev == nullptr) table[index] = current->next;
                else prev->next = current->next;
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    ~JobHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};

// ─── User Hash Table ──────────────────────────────────────────────────────────

class UserHashTable {
private:
    struct Node {
        User user;
        Node* next;
        Node(User u) : user(u), next(nullptr) {}
    };

    static const int TABLE_SIZE = 100;
    Node* table[TABLE_SIZE];

    int hashFunction(int id) {
        return id % TABLE_SIZE;
    }

public:
    UserHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    void insert(User user) {
        int index = hashFunction(user.id);
        Node* newNode = new Node(user);
        newNode->next = table[index];
        table[index] = newNode;
    }

    User* search(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->user.id == id) return &current->user;
            current = current->next;
        }
        return nullptr;
    }

    void remove(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->user.id == id) {
                if (prev == nullptr) table[index] = current->next;
                else prev->next = current->next;
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    ~UserHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};