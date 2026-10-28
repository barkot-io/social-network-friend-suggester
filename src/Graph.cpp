#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

static const std::unordered_set<int> EMPTY_SET;

SocialGraph::SocialGraph() : nextId(1) {}

// ── User management ───────────────────────────────────────────────────────────

int SocialGraph::addUser(const std::string& name, int age,
                         std::vector<std::string> interests) {
    int id = nextId++;
    users[id] = User(id, name, age, std::move(interests));
    adj[id]   = {};
    return id;
}

bool SocialGraph::removeUser(int id) {
    if (!userExists(id)) return false;
    for (int fid : adj[id]) adj[fid].erase(id);   // remove back-edges
    adj.erase(id);
    users.erase(id);
    return true;
}

bool SocialGraph::userExists(int id) const {
    return users.find(id) != users.end();
}

const User& SocialGraph::getUser(int id) const {
    return users.at(id);
}

const std::unordered_map<int, User>& SocialGraph::getAllUsers() const {
    return users;
}

// ── Friendship management ─────────────────────────────────────────────────────

bool SocialGraph::addFriendship(int a, int b) {
    if (!userExists(a) || !userExists(b)) return false;
    if (a == b || areFriends(a, b))       return false;
    adj[a].insert(b);
    adj[b].insert(a);
    return true;
}

bool SocialGraph::removeFriendship(int a, int b) {
    if (!areFriends(a, b)) return false;
    adj[a].erase(b);
    adj[b].erase(a);
    return true;
}

bool SocialGraph::areFriends(int a, int b) const {
    auto it = adj.find(a);
    return it != adj.end() && it->second.count(b);
}

const std::unordered_set<int>& SocialGraph::getFriends(int id) const {
    auto it = adj.find(id);
    return it == adj.end() ? EMPTY_SET : it->second;
}

// ── Mutual friends (Set intersection) ────────────────────────────────────────

int SocialGraph::getMutualFriendCount(int a, int b) const {
    if (!userExists(a) || !userExists(b)) return 0;
    const auto& fa = getFriends(a);
    const auto& fb = getFriends(b);
    int count = 0;
    for (int f : fa) count += fb.count(f);
    return count;
}

std::vector<int> SocialGraph::getMutualFriends(int a, int b) const {
    std::vector<int> result;
    if (!userExists(a) || !userExists(b)) return result;
    const auto& fa = getFriends(a);
    const auto& fb = getFriends(b);
    for (int f : fa)
        if (fb.count(f)) result.push_back(f);
    return result;
}

// ── Stats ─────────────────────────────────────────────────────────────────────

int SocialGraph::getUserCount() const { return static_cast<int>(users.size()); }

int SocialGraph::getEdgeCount() const {
    int total = 0;
    for (const auto& [id, friends] : adj) total += static_cast<int>(friends.size());
    return total / 2;
}

// ── Display ───────────────────────────────────────────────────────────────────

void SocialGraph::displayNetwork() const {
    std::cout << "\n  === Network Map (" << getUserCount() << " users, "
              << getEdgeCount() << " connections) ===\n";

    std::vector<int> ids;
    ids.reserve(users.size());
    for (const auto& [id, _] : users) ids.push_back(id);
    std::sort(ids.begin(), ids.end());

    for (int id : ids) {
        const User& u = users.at(id);
        std::cout << "  [" << id << "] " << u.name << " -> ";
        const auto& friends = adj.at(id);
        if (friends.empty()) {
            std::cout << "(no connections)";
        } else {
            bool first = true;
            for (int fid : friends) {
                if (!first) std::cout << ", ";
                std::cout << users.at(fid).name << "(" << fid << ")";
                first = false;
            }
        }
        std::cout << "\n";
    }
}
