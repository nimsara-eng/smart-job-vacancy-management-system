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

    
    void registerUser(User user, JobList& jobList);
    void removeUser(int id);

    User* getUserById(int id);
    User* loginUser(std::string username, std::string password);  
    std::vector<User> getAllUsers();

    
    void buildGraphEdges(User& user, JobList& jobList);

    bool usernameExists(const std::string& username);  

    int generateId();
};