#include "SuggestionEngine.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

SuggestionEngine::SuggestionEngine(const SocialGraph& g) : graph(g) {}

// Count shared interest tags between two users
int SuggestionEngine::calculateInterestScore(int a, int b) const {
    const auto& ia = graph.getUser(a).interests;
    const auto& ib = graph.getUser(b).interests;
    if (ia.empty() || ib.empty()) return 0;
    std::unordered_set<std::string> setA(ia.begin(), ia.end());
    int score = 0;
    for (const auto& tag : ib)
        if (setA.count(tag)) ++score;
    return score;
}

// ── Friend suggestion: BFS + Max-Heap ────────────────────────────────────────
std::vector<Suggestion> SuggestionEngine::suggestFriends(int userId, int topN) const {
    if (!graph.userExists(userId)) return {};

    // Build exclusion set: the user themselves + all direct friends
    std::unordered_set<int> excluded;
    excluded.insert(userId);
    for (int fid : graph.getFriends(userId)) excluded.insert(fid);

    // ── BFS (Queue) to reach level-2 nodes (friends-of-friends) ─────────────
    std::queue<int>         bfsQueue;   // BFS frontier
    std::unordered_set<int> visited;    // visited guard (backed by HashMap)
    std::unordered_set<int> candidates; // unique suggestion pool

    bfsQueue.push(userId);
    visited.insert(userId);

    while (!bfsQueue.empty()) {
        int cur = bfsQueue.front(); bfsQueue.pop();
        for (int lvl1 : graph.getFriends(cur)) {
            if (visited.count(lvl1)) continue;
            visited.insert(lvl1);
            // Every friend of a friend is a candidate
            for (int lvl2 : graph.getFriends(lvl1)) {
                if (!excluded.count(lvl2) && !visited.count(lvl2))
                    candidates.insert(lvl2);
            }
        }
    }

    // ── Score & rank with Max-Heap (priority_queue) ───────────────────────────
    std::priority_queue<Suggestion> maxHeap;
    for (int cid : candidates) {
        Suggestion s;
        s.userId        = cid;
        s.mutualCount   = graph.getMutualFriendCount(userId, cid);
        s.interestScore = calculateInterestScore(userId, cid);
        s.totalScore    = s.mutualCount * 3 + s.interestScore * 2;
        maxHeap.push(s);
    }

    // Extract top N from heap
    std::vector<Suggestion> result;
    while (!maxHeap.empty() && static_cast<int>(result.size()) < topN) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }
    return result;
}

// ── Most connected: partial sort by degree ───────────────────────────────────
std::vector<std::pair<int,int>> SuggestionEngine::getMostConnected(int topN) const {
    std::vector<std::pair<int,int>> degrees;
    degrees.reserve(graph.getUserCount());
    for (const auto& [id, _] : graph.getAllUsers())
        degrees.push_back({id, static_cast<int>(graph.getFriends(id).size())});

    int n = std::min(topN, static_cast<int>(degrees.size()));
    std::partial_sort(degrees.begin(), degrees.begin() + n, degrees.end(),
                      [](const auto& a, const auto& b){ return a.second > b.second; });
    degrees.resize(n);
    return degrees;
}
