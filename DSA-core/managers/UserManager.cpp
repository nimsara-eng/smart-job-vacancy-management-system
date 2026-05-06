#include "UserManager.h"
#include <iostream>

UserManager::UserManager(UserList& ul, UserHashTable& ht, Graph& g)
    : userList(ul), hashTable(ht), graph(g) {
    nextId = 1;
}

int UserManager::generateId() {
    return nextId++;
}

// BUG FIX: added duplicate username check and JobList parameter to build graph edges.
// Original registerUser had no duplicate check — two users could register with the
// same username, making login unpredictable (returns whichever appears first in list).
void UserManager::registerUser(User user, JobList& jobList) {
    if (usernameExists(user.username)) {
        std::cout << "Username '" << user.username << "' is already taken. Please choose another." << std::endl;
        return;
    }

    user.id = generateId();

    userList.append(user);
    hashTable.insert(user);
    graph.addUserNode(user.id);

    // BUG FIX: build graph edges immediately so this user appears in job matching.
    // Original code required a separate explicit call to buildGraphEdges, which
    // was easy to forget and caused matching to silently return empty results.
    buildGraphEdges(user, jobList);

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

// BUG FIX: original iterated a local copy vector (allUsers) and took &references
// into it, then called hashTable.search() - that part is fine. But the local vector
// is destroyed after the function returns - the returned pointer from hashTable is
// safe (points into hash table, not the vector). Added clearer comment for safety.
User* UserManager::loginUser(std::string username, std::string password) {
    std::vector<User> allUsers = userList.getAll();
    for (const User& user : allUsers) {
        if (user.username == username && user.password == password) {
            // Return pointer into hash table (safe - hash table owns the data)
            return hashTable.search(user.id);
        }
    }
    std::cout << "Invalid username or password." << std::endl;
    return nullptr;
}

// BUG FIX: added this missing helper. Without it, registerUser could not check
// for duplicate usernames, allowing two users with the same login credentials.
bool UserManager::usernameExists(const std::string& username) {
    std::vector<User> allUsers = userList.getAll();
    for (const User& user : allUsers) {
        if (user.username == username) return true;
    }
    return false;
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