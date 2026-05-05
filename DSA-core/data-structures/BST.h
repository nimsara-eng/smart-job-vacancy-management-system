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

    // Private helper functions
    Node* insert(Node* node, Job job) {
        if (node == nullptr) {
            return new Node(job);
        }
        if (job.salary < node->job.salary) {
            node->left = insert(node->left, job);
        } else if (job.salary > node->job.salary) {
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

    Node* remove(Node* node, float salary) {
        if (node == nullptr) return nullptr;

        if (salary < node->job.salary) {
            node->left = remove(node->left, salary);
        } else if (salary > node->job.salary) {
            node->right = remove(node->right, salary);
        } else {
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
                node->right = remove(node->right, successor->job.salary);
            }
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

    void remove(float salary) {
        root = remove(root, salary);
    }

    ~BST() {
        destroyTree(root);
    }
};