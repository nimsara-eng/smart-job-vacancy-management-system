#pragma once
#include <string>
#include <vector>
#include "../data-structures/LinkedList.h"
#include "../data-structures/HashTable.h"
#include "../data-structures/Graph.h"
#include "../models/User.h"

class UserManager {
private:
    UserList& userList;
    UserHashTable& hashTable;
    Graph& graph;

    int nextId;

public:
    UserManager(UserList& ul, UserHashTable& ht, Graph& g);

    // BUG FIX: registerUser now takes JobList& so graph edges are built
    // between the new user and all existing jobs at registration time.
    void registerUser(User user, JobList& jobList);
    void removeUser(int id);

    User* getUserById(int id);
    User* loginUser(std::string username, std::string password);  // returns nullptr if not found
    std::vector<User> getAllUsers();

    // BUG FIX: made public so it can be called externally if needed
    void buildGraphEdges(User& user, JobList& jobList);

    bool usernameExists(const std::string& username);  // BUG FIX: added to prevent duplicate usernames

    int generateId();
};