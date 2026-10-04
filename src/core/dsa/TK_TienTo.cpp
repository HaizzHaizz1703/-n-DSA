#include "TK_TienTo.h"
#include <cstring>
#include <cstdlib>

TrieNode* createNode() {
    TrieNode* node = new TrieNode(); // Cấp phát động
    node->idCount = 0;
    for (int i = 0; i < 128; i++) {
        node->children[i] = nullptr;
    }
    return node;
}

void insertTrie(TrieNode* root, const char* text, const char* courseId) {
    TrieNode* curr = root;
    for (int i = 0; text[i] != '\0'; i++) {
        int index = (int)text[i];
        if (curr->children[index] == nullptr) {
            curr->children[index] = createNode();
        }
        curr = curr->children[index];

        // Lưu mã môn học vào từng node đi qua để dễ dàng lấy gợi ý
        if (curr->idCount < 10) {
            strcpy(curr->courseIds[curr->idCount], courseId);
            curr->idCount++;
        }
    }
}

// Trả về cái node cuối cùng của chuỗi tiền tố nhập vào
TrieNode* searchPrefixTrie(TrieNode* root, const char* prefix) {
    TrieNode* curr = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = (int)prefix[i];
        if (curr->children[index] == nullptr) {
            return nullptr; // Không tìm thấy tiền tố này
        }
        curr = curr->children[index];
    }
    return curr;
}