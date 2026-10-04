#include "../src/core/dsa/Trie.h"
#include <iostream>

using namespace std;

int main() {
    Trie t;
    initTrie(t);

    insertTrie(t, "DASA230179");
    insertTrie(t, "DASA230180");
    insertTrie(t, "MATH123456");

    cout << "Test Trie - Tim tien to 'DASA':\n";
    searchPrefixTrie(t, "DASA");
    // Kết quả in ra phải là DASA230179 và DASA230180

    return 0;
}