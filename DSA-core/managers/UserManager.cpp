#include "UserManager.h"
#include <iostream>

UserManager::UserManager(UserList& ul, UserHashTable& ht, Graph& g)
    : userList(ul), hashTable(ht), graph(g) {
    nextId = 1;
}

int UserManager::generateId() {
    return nextId++;
}

void UserManager::registerUser(User user) {
    user.id = generateId();

    userList.append(user);
    hashTable.insert(user);
    graph.addUserNode(user.id);

    std::cout << "User registered successfully with ID: " << user.id << std::endl;
}

void UserManager::removeUser(int id) {
    userList.remove(id);
    hashTable.remove(id);
    std::cout << "User removed successfully." << std::endl;
}

User* UserManager::getUserById(int id) {
    return hashTable.search(id);
}

User* UserManager::loginUser(std::string username, std::string password) {
    std::vector<User> allUsers = userList.getAll();
    for (User& user : allUsers) {
        if (user.username == username && user.password == password) {
            return hashTable.search(user.id);
        }
    }
    std::cout << "Invalid username or password." << std::endl;
    return nullptr;
}

void UserManager::buildGraphEdges(User& user, JobList& jobList) {
    std::vector<Job> allJobs = jobList.getAll();
    for (Job& job : allJobs) {
        graph.buildEdges(user, job);
    }
}

std::vector<User> UserManager::getAllUsers() {
    return userList.getAll();
}