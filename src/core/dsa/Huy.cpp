#include "Huy.h"
#include <cstring>
#include <iostream>

using namespace std;

bool huyMon(Student* sv, Course* c) {
    int foundIndex = -1;
    for (int i = 0; i < sv->regCount; i++) {
        if (strcmp(sv->registeredCourses[i], c->id) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        cout << "Loi: Sinh vien chua dang ky mon nay!" << endl;
        return false;
    }

    // Xóa bằng cách dồn các phần tử phía sau lên trước
    for (int i = foundIndex; i < sv->regCount - 1; i++) {
        strcpy(sv->registeredCourses[i], sv->registeredCourses[i + 1]);
    }
    sv->regCount--;
    c->currentEnrolled--;

    return true;
}