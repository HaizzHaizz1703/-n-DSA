#include "../src/core/dsa/PriorityQueue.h"
#include <iostream>

using namespace std;

int main() {
    PriorityQueue pq;
    initPQ(pq);

    pushPQ(pq, "SV_Thuong", 1);
    pushPQ(pq, "SV_NamCuoi", 5); // Ưu tiên số 5 cao hơn số 1

    string firstOut = popPQ(pq);
    if (firstOut == "SV_NamCuoi") {
        cout << "Test PASSED - Lay dung sinh vien uu tien cao: " << firstOut << "\n";
    }
    else {
        cout << "Test FAILED!\n";
    }
    return 0;
}