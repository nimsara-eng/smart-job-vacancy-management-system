#pragma once
#include "../models/Application.h"

class Queue {
private:
    struct Node {
        Application application;
        Node* next;

        Node(Application a) {
            application = a;
            next = nullptr;
        }
    };

    Node* front;
    Node* rear;
    int size;

public:

    Queue() {
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void enqueue(Application application) {
        Node* newNode = new Node(application);
        if (isEmpty()) {
            front = newNode;
            rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    void dequeue() {
        if (isEmpty()) return;
        Node* temp = front;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        size--;
    }

    Application* peek() {
        if (isEmpty()) return nullptr;
        return &front->application;
    }

    int getSize() {
        return size;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};