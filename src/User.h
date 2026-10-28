#pragma once
#include <string>
#include <vector>
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
// User — core profile stored in the HashMap (unordered_map<int, User>)
// ─────────────────────────────────────────────────────────────────────────────
struct User {
    int id;
    std::string name;
    int age;
    std::vector<std::string> interests;  // used for interest-score bonus in suggestions

    User() : id(-1), name(""), age(0) {}
    User(int id, const std::string& name, int age,
         std::vector<std::string> interests = {})
        : id(id), name(name), age(age), interests(std::move(interests)) {}

    void display() const {
        std::cout << "  [" << id << "] " << name << ", Age: " << age;
        if (!interests.empty()) {
            std::cout << " | Interests: ";
            for (size_t i = 0; i < interests.size(); ++i) {
                if (i) std::cout << ", ";
                std::cout << interests[i];
            }
        }
        std::cout << "\n";
    }
};
