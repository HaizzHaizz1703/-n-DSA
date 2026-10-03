#include "DangKi.h"
#include <cstring>
#include <iostream>

using namespace std;

void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}

// Chèn phần tử sao cho mảng luôn giảm dần theo mức độ ưu tiên
void pushPQ(PriorityQueue* pq, WaitlistNode node) {
    if (pq->size >= 500) return;
    int i = pq->size - 1;
    while (i >= 0 && pq->arr[i].priority < node.priority) {
        pq->arr[i + 1] = pq->arr[i];
        i--;
    }
    pq->arr[i + 1] = node;
    pq->size++;
}

bool dangKyMon(Student* sv, Course* c, PriorityQueue* pq, int doUuTien) {
    if (sv->regCount >= sv->maxCourses) {
        cout << "Loi: Sinh vien vuot qua so mon toi da!" << endl;
        return false;
    }

    // Kiểm tra trùng môn
    for (int i = 0; i < sv->regCount; i++) {
        if (strcmp(sv->registeredCourses[i], c->id) == 0) {
            cout << "Loi: Sinh vien da dang ky mon nay roi!" << endl;
            return false;
        }
    }

    if (c->currentEnrolled < c->maxCapacity) {
        // Đăng ký thành công
        strcpy(sv->registeredCourses[sv->regCount], c->id);
        sv->regCount++;
        c->currentEnrolled++;
        return true;
    }
    else {
        // Môn đã đầy -> Đưa vào hàng đợi ưu tiên
        WaitlistNode node;
        strcpy(node.studentId, sv->id);
        strcpy(node.courseId, c->id);
        node.priority = doUuTien;
        pushPQ(pq, node);
        cout << "Mon hoc da day! Ban da duoc dua vao Waitlist." << endl;
        return false; // Chưa được vào học chính thức
    }
}