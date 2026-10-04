#pragma once

struct TrieNode {
    TrieNode* children[128]; // Bảng mã ASCII
    char courseIds[10][20];  // Lưu tối đa 10 mã môn khớp với nhánh này
    int idCount;
};

TrieNode* createNode();
void insertTrie(TrieNode* root, const char* prefix, const char* courseId);
TrieNode* searchPrefixTrie(TrieNode* root, const char* prefix);