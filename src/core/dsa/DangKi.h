#pragma once
#include "../models/Student.h"
#include "../models/Course.h"

struct WaitlistNode {
    char studentId[20];
    char courseId[20];
    int priority; // Mức độ ưu tiên (số càng lớn càng ưu tiên)
};

// Hàng đợi ưu tiên (Priority Queue) cài đặt bằng mảng
struct PriorityQueue {
    WaitlistNode arr[500];
    int size;
};

void initPQ(PriorityQueue* pq);
void pushPQ(PriorityQueue* pq, WaitlistNode node);
bool dangKyMon(Student* sv, Course* c, PriorityQueue* pq, int doUuTien);