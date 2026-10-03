#pragma once

// Đường dẫn tương đối từ thư mục src/core/
#include "models/Course.h"
#include "models/Student.h"
#include "dsa/TraCuu.h"
#include "dsa/TKB.h"
#include "dsa/DangKi.h"
#include "dsa/Huy.h"
#include "dsa/Undo.h"
#include "dsa/TK_TienTo.h"

struct HeThong {
    Student dsSinhVien[200];
    int soLuongSV;

    Course dsMonHoc[100];
    int soLuongMon;

    TraCuu bangBam;
    PriorityQueue waitlist;
    UndoStack lichSu;
    TrieNode* cayTienTo;
    UndoStack undoStack;
};

void initHeThong(HeThong* ht);
Course* heThong_TraCuuMon(HeThong* ht, const char* maHP);
void heThong_XemTKB(HeThong* ht, const char* maSV);
void heThong_DangKyMon(HeThong* ht, const char* maSV, const char* maHP);
void heThong_HuyMon(HeThong* ht, const char* maSV, const char* maHP);
void heThong_HoanTac(HeThong* ht);
void heThong_GoiYMon(HeThong* ht, const char* tienTo);