#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "User.h"

// ─────────────────────────────────────────────────────────────────────────────
// SocialGraph — the core data structure of the whole system
//
//  Internal representation:
//    users : unordered_map<int, User>          — HashMap  (O(1) profile lookup)
//    adj   : unordered_map<int,                — Graph    (adjacency list)
//              unordered_set<int>>             — Set      (O(1) edge query)
// ─────────────────────────────────────────────────────────────────────────────
class SocialGraph {
private:
    std::unordered_map<int, User>                   users;  // HashMap: id -> profile
    std::unordered_map<int, std::unordered_set<int>> adj;   // Graph:   id -> friend set
    int nextId;

public:
    SocialGraph();

    // User management
    int  addUser(const std::string& name, int age,
                 std::vector<std::string> interests = {});
    bool removeUser(int id);
    bool userExists(int id) const;
    const User& getUser(int id) const;
    const std::unordered_map<int, User>& getAllUsers() const;

    // Friendship management
    bool addFriendship(int a, int b);
    bool removeFriendship(int a, int b);
    bool areFriends(int a, int b) const;
    const std::unordered_set<int>& getFriends(int id) const;

    // Mutual friends (Set intersection)
    int              getMutualFriendCount(int a, int b) const;
    std::vector<int> getMutualFriends(int a, int b)    const;

    // Stats
    int getUserCount() const;
    int getEdgeCount() const;

    // Display
    void displayNetwork() const;
};
