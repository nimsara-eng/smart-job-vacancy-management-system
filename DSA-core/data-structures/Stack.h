#include <string>

class Stack {
private:
    struct Node {
        std::string data;
        Node* next;

        Node(std::string str) {
            data = str;
            next = nullptr;
        }
    };

public:
    Node* top;
    int size;

    Stack() {
        top = nullptr;
        size = 0;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void push(std::string str) {
        Node* newNode = new Node(str);
        newNode->next = top;
        top = newNode;
        size++;
    }

    void pop() {
        if (isEmpty()) return;
        Node* temp = top;
        top = top->next;
        delete temp;
        size--;
    }

    std::string peek() {
        if (isEmpty()) return "";
        return top->data;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};