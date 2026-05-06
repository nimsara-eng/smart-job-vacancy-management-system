#pragma once
#include <vector>
#include "../models/Job.h"

class BST {
private:

    struct Node {
        Job job;
        Node* left;
        Node* right;

        Node(Job j) {
            job = j;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    Node* insert(Node* node, Job job) {
        if (node == nullptr) {
            return new Node(job);
        }
        if (job.salary < node->job.salary) {
            node->left = insert(node->left, job);
        } else if (job.salary > node->job.salary) {
            node->right = insert(node->right, job);
        }
        // If salary is equal, insert to the right (handles duplicate salaries)
        else {
            node->right = insert(node->right, job);
        }
        return node;
    }

    void inOrder(Node* node, std::vector<Job>& result) {
        if (node == nullptr) return;
        inOrder(node->left, result);
        result.push_back(node->job);
        inOrder(node->right, result);
    }

    void searchByRange(Node* node, float minSalary, float maxSalary, std::vector<Job>& result) {
        if (node == nullptr) return;
        if (node->job.salary >= minSalary && node->job.salary <= maxSalary) {
            result.push_back(node->job);
        }
        if (minSalary < node->job.salary) {
            searchByRange(node->left, minSalary, maxSalary, result);
        }
        if (maxSalary > node->job.salary) {
            searchByRange(node->right, minSalary, maxSalary, result);
        }
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // BUG FIX: Old remove() deleted by salary (wrong - multiple jobs can share a salary).
    // New removeById() deletes the exact node matching the job ID.
    Node* removeById(Node* node, int id) {
        if (node == nullptr) return nullptr;

        // Search both subtrees since BST is ordered by salary, not id
        if (node->job.id == id) {
            // Node found - 3 cases
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            } else {
                Node* successor = findMin(node->right);
                node->job = successor->job;
                node->right = removeById(node->right, successor->job.id);
            }
        } else {
            // Must search both sides since we're searching by ID, not salary
            node->left  = removeById(node->left, id);
            node->right = removeById(node->right, id);
        }
        return node;
    }

    void destroyTree(Node* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:

    BST() {
        root = nullptr;
    }

    void insert(Job job) {
        root = insert(root, job);
    }

    std::vector<Job> inOrder() {
        std::vector<Job> result;
        inOrder(root, result);
        return result;
    }

    std::vector<Job> searchByRange(float minSalary, float maxSalary) {
        std::vector<Job> result;
        searchByRange(root, minSalary, maxSalary, result);
        return result;
    }

    // BUG FIX: renamed from remove(float salary) to removeById(int id)
    // Old version deleted by salary - wrong when multiple jobs share the same salary
    void removeById(int id) {
        root = removeById(root, id);
    }

    ~BST() {
        destroyTree(root);
    }
};