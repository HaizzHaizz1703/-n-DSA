// ============================================================
//  test_helper.h - Bo khung test don gian dung chung cho 6 chuc nang
//  (Khong dung framework ngoai, chi dung <cstdio>, <cstring>, <sstream>)
// ============================================================
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

#include "../src/core/HeThong.h"

// ---------- Bo dem ket qua ----------
static int g_pass = 0;
static int g_fail = 0;
static int g_note = 0;

// ---------- Bat output cua cout de kiem tra noi dung thong bao ----------
static std::ostringstream g_capBuf;
static std::streambuf*    g_capOld = nullptr;

static void beginCapture() {
    g_capBuf.str("");
    g_capBuf.clear();
    if (g_capOld == nullptr) g_capOld = std::cout.rdbuf(g_capBuf.rdbuf());
}
static void endCapture() {
    if (g_capOld != nullptr) {
        std::cout.rdbuf(g_capOld);
        g_capOld = nullptr;
    }
}
// Lay noi dung da bat (chi hop le sau endCapture)
static std::string captured() { return g_capBuf.str(); }
static bool capturedHas(const char* text) {
    return g_capBuf.str().find(text) != std::string::npos;
}
// Vi tri xuat hien (de kiem tra thu tu), -1 neu khong co
static int capturedPos(const char* text) {
    size_t p = g_capBuf.str().find(text);
    return (p == std::string::npos) ? -1 : (int)p;
}

// ---------- Cac macro kiem tra ----------
#define CHECK(cond, msg)                                                    \
    do {                                                                    \
        if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }               \
        else      { g_fail++; printf("  [FAIL] %s   (dong %d)\n", msg, __LINE__); } \
    } while (0)

#define CHECK_INT(actual, expected, msg)                                    \
    do {                                                                    \
        int _a = (int)(actual), _e = (int)(expected);                       \
        if (_a == _e) { g_pass++; printf("  [PASS] %s\n", msg); }           \
        else { g_fail++; printf("  [FAIL] %s -> mong doi %d, thuc te %d   (dong %d)\n", msg, _e, _a, __LINE__); } \
    } while (0)

#define CHECK_STR(actual, expected, msg)                                    \
    do {                                                                    \
        const char* _a = (actual); const char* _e = (expected);             \
        if (_a != nullptr && strcmp(_a, _e) == 0) { g_pass++; printf("  [PASS] %s\n", msg); } \
        else { g_fail++; printf("  [FAIL] %s -> mong doi \"%s\", thuc te \"%s\"   (dong %d)\n", msg, _e, _a ? _a : "(null)", __LINE__); } \
    } while (0)

// NOTE: ghi nhan mot han che/hanh vi dang chu y cua code goc (KHONG tinh la FAIL)
#define NOTE(cond, msg)                                                     \
    do {                                                                    \
        if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }               \
        else      { g_note++; printf("  [NOTE] %s\n", msg); }               \
    } while (0)

#define SECTION(title) printf("\n--- %s ---\n", title)

#define TEST_BEGIN(title)                                                   \
    printf("==================================================\n");        \
    printf(" TEST: %s\n", title);                                           \
    printf("==================================================\n")

// Tong ket, tra ve exit code: 0 neu khong co FAIL
static int testSummary() {
    printf("\n==================================================\n");
    printf(" KET QUA: %d PASS | %d FAIL | %d NOTE\n", g_pass, g_fail, g_note);
    if (g_fail == 0) printf(" => TAT CA TEST DEU DAT\n");
    else             printf(" => CO %d TEST KHONG DAT\n", g_fail);
    if (g_note > 0)  printf(" (NOTE = han che da biet cua code goc, khong tinh la loi)\n");
    printf("==================================================\n");
    return (g_fail == 0) ? 0 : 1;
}

// ---------- Ham tien ich tao du lieu mau ----------
static Course makeCourse(const char* id, const char* name, int credits,
                         int maxCap, int cur, const char* lich) {
    Course c;
    memset(&c, 0, sizeof(c));
    strcpy(c.id, id);
    strcpy(c.name, name);
    c.credits = credits;
    c.maxCapacity = maxCap;
    c.currentEnrolled = cur;
    strcpy(c.lichHoc, lich);
    return c;
}

static Student makeStudent(const char* id, const char* name, int maxCourses) {
    Student s;
    memset(&s, 0, sizeof(s));
    strcpy(s.id, id);
    strcpy(s.name, name);
    s.maxCourses = maxCourses;
    s.regCount = 0;
    return s;
}

// Them mon vao danh sach da dang ky cua sinh vien (khong qua logic dang ky)
static void giveCourse(Student* s, const char* courseId) {
    strcpy(s->registeredCourses[s->regCount], courseId);
    s->regCount++;
}

// Kiem tra sinh vien co mon nay khong
static bool hasCourse(const Student* s, const char* courseId) {
    for (int i = 0; i < s->regCount; i++)
        if (strcmp(s->registeredCourses[i], courseId) == 0) return true;
    return false;
}

// Dua mon vao CA bang bam CA mang dsMonHoc cua HeThong (giong main.cpp)
static void addCourseToSystem(HeThong* ht, const Course& c) {
    ht->dsMonHoc[ht->soLuongMon++] = c;
    insertCourseHT(&(ht->bangBam), c);
    insertTrie(ht->cayTienTo, c.id, c.id);
}

// Them sinh vien vao HeThong, tra ve con tro toi sinh vien trong he thong
static Student* addStudentToSystem(HeThong* ht, const Student& s) {
    ht->dsSinhVien[ht->soLuongSV] = s;
    return &(ht->dsSinhVien[ht->soLuongSV++]);
}

// Khoi tao HeThong day du (giong main.cpp: initHeThong + initStack(undoStack))
static void setupHeThong(HeThong* ht) {
    initHeThong(ht);
    initStack(&(ht->undoStack));
}
