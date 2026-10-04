// ============================================================
//  TEST CHUC NANG 4: HUY DANG KY HOC PHAN
//  File nguon: src/core/dsa/Huy.cpp (+ heThong_HuyMon)
// ============================================================
#include "test_helper.h"
#include "../src/persistence/DataLoad.h"

static HeThong g_ht;

static void test_cancel_success() {
    SECTION("1. Huy mon thanh cong");
    Student sv = makeStudent("24110001", "Nguyen Van 1", 5);
    giveCourse(&sv, "CS101");
    Course c = makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "x");

    beginCapture();
    bool ok = huyMon(&sv, &c);
    endCapture();

    CHECK(ok, "Tra ve true");
    CHECK_INT(sv.regCount, 0, "regCount giam 1 -> 0");
    CHECK(!hasCourse(&sv, "CS101"), "Mon khong con trong danh sach sinh vien");
    CHECK_INT(c.currentEnrolled, 44, "Si so mon hoc giam 45 -> 44");
}

static void test_cancel_not_registered() {
    SECTION("2. Huy mon chua dang ky");
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "CS101");
    Course c = makeCourse("MA101", "Giai tich", 3, 50, 20, "x");

    beginCapture();
    bool ok = huyMon(&sv, &c);
    endCapture();

    CHECK(!ok, "Tra ve false");
    CHECK(capturedHas("chua dang ky mon nay"), "In thong bao 'chua dang ky mon nay'");
    CHECK_INT(sv.regCount, 1, "regCount khong doi");
    CHECK_INT(c.currentEnrolled, 20, "Si so khong doi");
    CHECK(hasCourse(&sv, "CS101"), "Mon khac van con nguyen");

    // Sinh vien khong co mon nao
    Student empty = makeStudent("E", "Rong", 5);
    beginCapture();
    ok = huyMon(&empty, &c);
    endCapture();
    CHECK(!ok, "Sinh vien chua co mon nao -> huy that bai");
    CHECK_INT(empty.regCount, 0, "regCount van = 0 (khong bi am)");
    CHECK_INT(c.currentEnrolled, 20, "Si so khong doi");
}

static void test_shift_middle() {
    SECTION("3. Xoa phan tu o GIUA danh sach (don mang)");
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "A1"); giveCourse(&sv, "B2"); giveCourse(&sv, "C3"); giveCourse(&sv, "D4");
    Course c = makeCourse("B2", "B", 3, 50, 10, "x");

    beginCapture();
    bool ok = huyMon(&sv, &c);
    endCapture();

    CHECK(ok, "Huy B2 thanh cong");
    CHECK_INT(sv.regCount, 3, "regCount = 3");
    CHECK_STR(sv.registeredCourses[0], "A1", "Vi tri 0: A1");
    CHECK_STR(sv.registeredCourses[1], "C3", "Vi tri 1: C3 (don len thay B2)");
    CHECK_STR(sv.registeredCourses[2], "D4", "Vi tri 2: D4 (don len)");
}

static void test_shift_first_last() {
    SECTION("4. Xoa phan tu DAU va CUOI danh sach");
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "A1"); giveCourse(&sv, "B2"); giveCourse(&sv, "C3");

    Course first = makeCourse("A1", "A", 3, 50, 10, "x");
    beginCapture();
    bool ok1 = huyMon(&sv, &first);
    endCapture();
    CHECK(ok1, "Huy phan tu dau (A1) thanh cong");
    CHECK_INT(sv.regCount, 2, "regCount = 2");
    CHECK_STR(sv.registeredCourses[0], "B2", "Vi tri 0 bay gio la B2");
    CHECK_STR(sv.registeredCourses[1], "C3", "Vi tri 1 bay gio la C3");

    Course last = makeCourse("C3", "C", 3, 50, 10, "x");
    beginCapture();
    bool ok2 = huyMon(&sv, &last);
    endCapture();
    CHECK(ok2, "Huy phan tu cuoi (C3) thanh cong");
    CHECK_INT(sv.regCount, 1, "regCount = 1");
    CHECK_STR(sv.registeredCourses[0], "B2", "Con lai duy nhat B2");
}

static void test_cancel_all() {
    SECTION("5. Huy lan luot toan bo mon");
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "A1"); giveCourse(&sv, "B2"); giveCourse(&sv, "C3");
    Course a = makeCourse("A1", "A", 3, 50, 5, "x");
    Course b = makeCourse("B2", "B", 3, 50, 6, "x");
    Course c = makeCourse("C3", "C", 3, 50, 7, "x");
    beginCapture();
    huyMon(&sv, &c);
    huyMon(&sv, &a);
    huyMon(&sv, &b);
    endCapture();
    CHECK_INT(sv.regCount, 0, "Sau khi huy het, regCount = 0");
    CHECK_INT(a.currentEnrolled, 4, "A1: 5 -> 4");
    CHECK_INT(b.currentEnrolled, 5, "B2: 6 -> 5");
    CHECK_INT(c.currentEnrolled, 6, "C3: 7 -> 6");

    beginCapture();
    bool again = huyMon(&sv, &a);
    endCapture();
    CHECK(!again, "Huy lan nua khi danh sach rong -> false");
    CHECK_INT(a.currentEnrolled, 4, "Si so khong bi giam them");
}

static void test_cancel_twice() {
    SECTION("6. Huy cung mot mon hai lan");
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "CS101");
    Course c = makeCourse("CS101", "Mon", 3, 50, 10, "x");
    beginCapture();
    bool first = huyMon(&sv, &c);
    bool second = huyMon(&sv, &c);
    endCapture();
    CHECK(first, "Lan 1 thanh cong");
    CHECK(!second, "Lan 2 that bai (da huy roi)");
    CHECK_INT(c.currentEnrolled, 9, "Si so chi giam 1 lan (10 -> 9)");
}

static void test_similar_ids() {
    SECTION("7. Ma mon gan giong nhau khong bi nham");
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "CS1010");
    giveCourse(&sv, "CS10");
    Course c = makeCourse("CS10", "Mon CS10", 3, 50, 10, "x");
    beginCapture();
    bool ok = huyMon(&sv, &c);
    endCapture();
    CHECK(ok, "Huy CS10 thanh cong");
    CHECK_INT(sv.regCount, 2, "regCount = 2");
    CHECK(hasCourse(&sv, "CS101"), "CS101 van con (khong bi huy nham)");
    CHECK(hasCourse(&sv, "CS1010"), "CS1010 van con (khong bi huy nham)");
    CHECK(!hasCourse(&sv, "CS10"), "CS10 da bi xoa");
}

static void test_register_after_cancel() {
    SECTION("8. Huy roi dang ky lai");
    PriorityQueue pq; initPQ(&pq);
    Student sv = makeStudent("S", "SV", 1);   // chi duoc 1 mon
    giveCourse(&sv, "CS101");
    Course c = makeCourse("CS101", "Mon", 3, 50, 10, "x");
    Course d = makeCourse("MA101", "Mon2", 3, 50, 10, "x");

    beginCapture();
    bool blocked = dangKyMon(&sv, &d, &pq, 1);     // dang day -> bi chan
    huyMon(&sv, &c);                               // giai phong 1 cho
    bool nowOk = dangKyMon(&sv, &d, &pq, 1);       // gio dang ky duoc
    endCapture();
    CHECK(!blocked, "Truoc khi huy: dang ky mon moi bi chan (da day maxCourses)");
    CHECK(nowOk, "Sau khi huy: dang ky mon moi thanh cong");
    CHECK(hasCourse(&sv, "MA101"), "Sinh vien co mon moi");
    CHECK(!hasCourse(&sv, "CS101"), "Sinh vien khong con mon cu");
}

static void test_waitlist_not_promoted() {
    SECTION("9. Han che: huy mon khong tu dong day Waitlist len");
    PriorityQueue pq; initPQ(&pq);
    Course c = makeCourse("CS201", "Mon", 3, 2, 2, "x");     // dang day 2/2
    Student a = makeStudent("A", "A", 5); giveCourse(&a, "CS201");
    Student w = makeStudent("W", "W", 5);
    beginCapture();
    dangKyMon(&w, &c, &pq, 1);       // W vao waitlist
    huyMon(&a, &c);                  // A huy -> co cho trong
    endCapture();
    CHECK_INT(c.currentEnrolled, 1, "Sau khi A huy: si so 1/2 (con 1 cho trong)");
    NOTE(pq.size == 0 && w.regCount == 1,
         "Sinh vien trong Waitlist chua duoc tu dong chuyen len khi co cho trong (chua cai dat)");
}

static void test_system_level() {
    SECTION("10. heThong_HuyMon (tich hop he thong + Undo)");
    setupHeThong(&g_ht);
    addCourseToSystem(&g_ht, makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "x"));
    addCourseToSystem(&g_ht, makeCourse("CS201", "Cau truc du lieu", 3, 60, 60, "x"));
    Student s = makeStudent("24110001", "Nguyen Van 1", 5);
    giveCourse(&s, "CS101");
    giveCourse(&s, "CS201");
    Student* sv = addStudentToSystem(&g_ht, s);

    beginCapture();
    heThong_HuyMon(&g_ht, "24110001", "CS101");
    endCapture();
    CHECK(capturedHas("Huy thanh cong"), "In thong bao 'Huy thanh cong'");
    CHECK(!hasCourse(sv, "CS101"), "Mon CS101 da bi go");
    CHECK(hasCourse(sv, "CS201"), "Mon CS201 van con");
    CHECK_INT(sv->regCount, 1, "regCount = 1");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled, 44, "Si so trong bang bam giam 45 -> 44");
    CHECK_INT(g_ht.lichSu.top, 0, "Hanh dong duoc ghi vao lich su Undo");
    CHECK_STR(g_ht.lichSu.arr[0].type, "HUY", "  Loai hanh dong = HUY");
    CHECK_STR(g_ht.lichSu.arr[0].courseId, "CS101", "  Ma mon dung");
    CHECK_STR(g_ht.lichSu.arr[0].studentId, "24110001", "  Ma sinh vien dung");

    // Huy that bai: khong ghi undo
    beginCapture();
    heThong_HuyMon(&g_ht, "24110001", "CS101");   // da huy roi
    endCapture();
    CHECK(capturedHas("chua dang ky mon nay"), "Huy mon da huy -> bao loi");
    CHECK_INT(g_ht.lichSu.top, 0, "  Huy that bai KHONG ghi them Undo");

    // Sinh vien / mon khong ton tai: khong crash, khong ghi undo
    beginCapture();
    heThong_HuyMon(&g_ht, "00000000", "CS201");
    heThong_HuyMon(&g_ht, "24110001", "NOPE99");
    endCapture();
    CHECK_INT(g_ht.lichSu.top, 0, "SV/mon khong ton tai: khong crash, khong ghi Undo");
    CHECK_INT(sv->regCount, 1, "  Du lieu khong bi thay doi");
    NOTE(capturedHas("Khong tim thay"),
         "heThong_HuyMon im lang khi SV/mon khong ton tai (khong in thong bao loi nhu heThong_DangKyMon)");
}

static void test_real_data() {
    SECTION("11. Tich hop du lieu that");
    setupHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    docFileSinhVien(&g_ht, "data/students.json");
    for (int i = 0; i < g_ht.soLuongMon; i++) insertCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i]);

    // 24110004: CS101, CS102, MA101
    int before = searchCourseHT(&g_ht.bangBam, "CS102")->currentEnrolled;
    beginCapture();
    heThong_HuyMon(&g_ht, "24110004", "CS102");
    endCapture();
    CHECK(capturedHas("Huy thanh cong"), "SV 24110004 huy CS102 thanh cong");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS102")->currentEnrolled, before - 1, "CS102: si so giam 1");

    beginCapture();
    heThong_XemTKB(&g_ht, "24110004");
    endCapture();
    CHECK(capturedHas("2/5"), "TKB cua SV 24110004 con 2/5 mon");
    CHECK(!capturedHas("CS102 |"), "TKB khong con CS102");
    CHECK(capturedHas("CS101 |") && capturedHas("MA101 |"), "TKB van con CS101 va MA101");

    // 24110003 khong co mon nao
    beginCapture();
    heThong_HuyMon(&g_ht, "24110003", "CS101");
    endCapture();
    CHECK(capturedHas("chua dang ky mon nay"), "SV 24110003 (khong co mon) huy -> bao loi");
}

int main() {
    TEST_BEGIN("CHUC NANG 4 - HUY DANG KY HOC PHAN (Huy)");
    test_cancel_success();
    test_cancel_not_registered();
    test_shift_middle();
    test_shift_first_last();
    test_cancel_all();
    test_cancel_twice();
    test_similar_ids();
    test_register_after_cancel();
    test_waitlist_not_promoted();
    test_system_level();
    test_real_data();
    return testSummary();
}
