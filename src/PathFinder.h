#pragma once
#include "Graph.h"
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// PathFinder — shortest connection path between any two users
//
//  shortestPath():
//    1. Queue           — BFS frontier
//    2. HashMap         — parent tracking (unordered_map<int,int>)
//    3. Stack           — LIFO reversal to reconstruct path source→target
//
//  degreesOfSeparation():
//    path length - 1  (e.g. direct friends = 1 degree)
// ─────────────────────────────────────────────────────────────────────────────
class PathFinder {
private:
    const SocialGraph& graph;

public:
    explicit PathFinder(const SocialGraph& g);

    // Returns ordered list of user IDs from 'from' to 'to'; empty = no path
    std::vector<int> shortestPath(int from, int to) const;

    bool areConnected(int from, int to) const;
    int  degreesOfSeparation(int from, int to) const;
};
