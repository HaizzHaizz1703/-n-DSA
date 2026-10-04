#include "TraCuu.h"
#include <cstring>

void initTraCuu(TraCuu* tc) {
    tc->count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        tc->isOccupied[i] = false;
    }
}

// Băm bằng cách cộng mã ASCII của chuỗi
int hashCourseId(const char* id) {
    int sum = 0;
    for (int i = 0; id[i] != '\0'; i++) {
        sum += id[i];
    }
    return sum % HASH_SIZE;
}

void insertCourseHT(TraCuu* tc, Course c) {
    if (tc->count >= HASH_SIZE) return;

    int index = hashCourseId(c.id);

    // Linear Probing: Tiến tới ô tiếp theo nếu ô hiện tại đã có dữ liệu
    while (tc->isOccupied[index] == true) {
        index = (index + 1) % HASH_SIZE;
    }

    tc->table[index] = c;
    tc->isOccupied[index] = true;
    tc->count++;
}

Course* searchCourseHT(TraCuu* tc, const char* id) {
    int index = hashCourseId(id);
    int startIndex = index;

    while (tc->isOccupied[index] == true) {
        if (strcmp(tc->table[index].id, id) == 0) {
            return &(tc->table[index]); // Tìm thấy
        }
        index = (index + 1) % HASH_SIZE;
        if (index == startIndex) break; // Quay lại điểm đầu -> hết bảng
    }
    return nullptr;
}