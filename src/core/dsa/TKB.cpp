#include "TKB.h"
#include <iostream>

using namespace std;

void inThoiKhoaBieu(Student* sv, TraCuu* tc) {
    cout << "\n========================================================\n";
    cout << "THOI KHOA BIEU CUA SINH VIEN: " << sv->name << " (" << sv->id << ")\n";
    cout << "So mon dang ky: " << sv->regCount << "/" << sv->maxCourses << "\n";
    cout << "--------------------------------------------------------\n";

    if (sv->regCount == 0) {
        cout << "Sinh vien chua dang ky mon hoc nao.\n";
    }
    else {
        for (int i = 0; i < sv->regCount; i++) {
            // Nhờ Bảng Băm tìm thông tin chi tiết của môn học
            Course* c = searchCourseHT(tc, sv->registeredCourses[i]);

            if (c != nullptr) {
                cout << "- " << c->id << " | " << c->name << " (" << c->credits << "TC)\n";
                cout << "  Lich hoc: " << c->lichHoc << "\n";
            }
            else {
                cout << "- " << sv->registeredCourses[i] << " | (Loi: Khong tim thay thong tin!)\n";
            }
        }
    }
    cout << "========================================================\n";
}