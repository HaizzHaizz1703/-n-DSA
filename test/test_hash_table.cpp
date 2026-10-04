#include "../src/core/dsa/HashTable.h"
#include <iostream>

using namespace std;

int main() {
    HashTable ht;
    initHashTable(ht);

    Course c1 = { "DASA230179", "Cau truc du lieu", 3, 40, 10 };
    insertCourseHT(ht, c1);

    Course* res = searchCourseHT(ht, "DASA230179");
    if (res != nullptr) {
        cout << "Test PASSED - Tim thay mon: " << res->name << "\n";
    }
    else {
        cout << "Test FAILED!\n";
    }
    return 0;
}