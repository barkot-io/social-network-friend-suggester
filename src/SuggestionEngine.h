#pragma once
#include "Graph.h"
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// Suggestion — scored candidate for friend recommendation
// Stored in a max-heap (std::priority_queue) during ranking
// ─────────────────────────────────────────────────────────────────────────────
struct Suggestion {
    int userId;
    int mutualCount;    // number of mutual friends
    int interestScore;  // number of shared interest tags
    int totalScore;     // mutualCount*3 + interestScore*2

    // max-heap: higher score = higher priority
    bool operator<(const Suggestion& o) const { return totalScore < o.totalScore; }
};

// ─────────────────────────────────────────────────────────────────────────────
// SuggestionEngine
//
//  suggestFriends():
//    1. Queue  — BFS to discover friends-of-friends (level-2 nodes)
//    2. Set    — track visited / exclude direct friends
//    3. Max-Heap (priority_queue<Suggestion>) — rank candidates by score
//
//  getMostConnected():
//    partial_sort on degree (connection count) across all users
// ─────────────────────────────────────────────────────────────────────────────
class SuggestionEngine {
private:
    const SocialGraph& graph;
    int calculateInterestScore(int a, int b) const;

public:
    explicit SuggestionEngine(const SocialGraph& g);

    std::vector<Suggestion>       suggestFriends(int userId, int topN = 5) const;
    std::vector<std::pair<int,int>> getMostConnected(int topN = 5)          const;
};
