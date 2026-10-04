// ============================================================
//  TEST CHUC NANG 3: DANG KY HOC PHAN + WAITLIST (Priority Queue)
//  File nguon: src/core/dsa/DangKi.cpp (+ heThong_DangKyMon)
// ============================================================
#include "test_helper.h"
#include "../src/persistence/DataLoad.h"

static PriorityQueue g_pq;
static HeThong g_ht;

static WaitlistNode makeNode(const char* sv, const char* course, int pri) {
    WaitlistNode n;
    memset(&n, 0, sizeof(n));
    strcpy(n.studentId, sv);
    strcpy(n.courseId, course);
    n.priority = pri;
    return n;
}

// ---------------- Priority Queue ----------------
static void test_pq_basic() {
    SECTION("1. Priority Queue: khoi tao va chen co ban");
    memset(&g_pq, 0xFF, sizeof(g_pq));
    initPQ(&g_pq);
    CHECK_INT(g_pq.size, 0, "size = 0 sau khi init");

    pushPQ(&g_pq, makeNode("SV1", "CS101", 3));
    CHECK_INT(g_pq.size, 1, "Chen 1 phan tu -> size = 1");
    CHECK_STR(g_pq.arr[0].studentId, "SV1", "Phan tu dau la SV1");
    CHECK_STR(g_pq.arr[0].courseId, "CS101", "Ma mon dung");
    CHECK_INT(g_pq.arr[0].priority, 3, "Do uu tien dung");
}

static void test_pq_order() {
    SECTION("2. Priority Queue: sap xep giam dan theo do uu tien");
    initPQ(&g_pq);
    pushPQ(&g_pq, makeNode("A", "C", 1));
    pushPQ(&g_pq, makeNode("B", "C", 5));
    pushPQ(&g_pq, makeNode("C", "C", 3));
    pushPQ(&g_pq, makeNode("D", "C", 4));
    pushPQ(&g_pq, makeNode("E", "C", 2));

    CHECK_INT(g_pq.size, 5, "size = 5");
    CHECK_STR(g_pq.arr[0].studentId, "B", "Vi tri 0: B (uu tien 5)");
    CHECK_STR(g_pq.arr[1].studentId, "D", "Vi tri 1: D (uu tien 4)");
    CHECK_STR(g_pq.arr[2].studentId, "C", "Vi tri 2: C (uu tien 3)");
    CHECK_STR(g_pq.arr[3].studentId, "E", "Vi tri 3: E (uu tien 2)");
    CHECK_STR(g_pq.arr[4].studentId, "A", "Vi tri 4: A (uu tien 1)");

    bool sorted = true;
    for (int i = 0; i + 1 < g_pq.size; i++)
        if (g_pq.arr[i].priority < g_pq.arr[i + 1].priority) sorted = false;
    CHECK(sorted, "Mang luon giam dan (dinh luat bat bien)");
}

static void test_pq_fifo() {
    SECTION("3. Priority Queue: cung do uu tien -> vao truoc ra truoc (FIFO)");
    initPQ(&g_pq);
    pushPQ(&g_pq, makeNode("X1", "C", 2));
    pushPQ(&g_pq, makeNode("X2", "C", 2));
    pushPQ(&g_pq, makeNode("X3", "C", 2));
    CHECK_STR(g_pq.arr[0].studentId, "X1", "X1 (vao dau tien) o dau hang doi");
    CHECK_STR(g_pq.arr[1].studentId, "X2", "X2 o giua");
    CHECK_STR(g_pq.arr[2].studentId, "X3", "X3 o cuoi");

    pushPQ(&g_pq, makeNode("HI", "C", 9));
    pushPQ(&g_pq, makeNode("LO", "C", 1));
    CHECK_STR(g_pq.arr[0].studentId, "HI", "Uu tien cao chen vao thi len dau");
    CHECK_STR(g_pq.arr[4].studentId, "LO", "Uu tien thap chen vao thi xuong cuoi");
    CHECK_STR(g_pq.arr[1].studentId, "X1", "Thu tu FIFO cua nhom X van giu nguyen");
}

static void test_pq_full() {
    SECTION("4. Priority Queue: hang doi day (500 phan tu)");
    initPQ(&g_pq);
    char id[20];
    for (int i = 0; i < 500; i++) {
        sprintf(id, "SV%03d", i);
        pushPQ(&g_pq, makeNode(id, "C", i % 7));
    }
    CHECK_INT(g_pq.size, 500, "size = 500 khi day");
    pushPQ(&g_pq, makeNode("OVER", "C", 100));
    CHECK_INT(g_pq.size, 500, "Chen them khi day -> bi bo qua, size van = 500");
    CHECK(strcmp(g_pq.arr[0].studentId, "OVER") != 0, "Phan tu tran khong lot vao hang doi");
    bool sorted = true;
    for (int i = 0; i + 1 < g_pq.size; i++)
        if (g_pq.arr[i].priority < g_pq.arr[i + 1].priority) sorted = false;
    CHECK(sorted, "500 phan tu van giu dung thu tu giam dan");
}

// ---------------- dangKyMon ----------------
static void test_register_success() {
    SECTION("5. Dang ky thanh cong (con cho)");
    initPQ(&g_pq);
    Student sv = makeStudent("24110001", "Nguyen Van 1", 5);
    Course c = makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "Thu 2");

    beginCapture();
    bool ok = dangKyMon(&sv, &c, &g_pq, 1);
    endCapture();

    CHECK(ok, "Tra ve true");
    CHECK_INT(sv.regCount, 1, "regCount tang len 1");
    CHECK_STR(sv.registeredCourses[0], "CS101", "Mon duoc ghi vao danh sach cua sinh vien");
    CHECK_INT(c.currentEnrolled, 46, "Si so mon hoc tang 45 -> 46");
    CHECK_INT(g_pq.size, 0, "Waitlist khong bi them gi");
}

static void test_register_last_slot() {
    SECTION("6. Dang ky vao cho cuoi cung (sat bien)");
    initPQ(&g_pq);
    Student sv = makeStudent("S", "SV", 5);
    Course c = makeCourse("CS101", "Mon", 3, 50, 49, "x");
    beginCapture();
    bool ok = dangKyMon(&sv, &c, &g_pq, 1);
    endCapture();
    CHECK(ok, "Cho cuoi cung (49/50) van dang ky duoc");
    CHECK_INT(c.currentEnrolled, 50, "Si so len 50/50");
    CHECK_INT(g_pq.size, 0, "Khong vao waitlist");
}

static void test_register_duplicate() {
    SECTION("7. Dang ky trung mon");
    initPQ(&g_pq);
    Student sv = makeStudent("S", "SV", 5);
    giveCourse(&sv, "CS101");
    Course c = makeCourse("CS101", "Mon", 3, 50, 10, "x");

    beginCapture();
    bool ok = dangKyMon(&sv, &c, &g_pq, 1);
    endCapture();

    CHECK(!ok, "Tra ve false");
    CHECK(capturedHas("da dang ky mon nay"), "In thong bao 'da dang ky mon nay roi'");
    CHECK_INT(sv.regCount, 1, "regCount khong doi");
    CHECK_INT(c.currentEnrolled, 10, "Si so khong doi");
    CHECK_INT(g_pq.size, 0, "Khong vao waitlist");
}

static void test_register_over_limit() {
    SECTION("8. Vuot so mon toi da");
    initPQ(&g_pq);
    Student sv = makeStudent("S", "SV", 2);
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "CS102");
    Course c = makeCourse("MA101", "Mon", 3, 50, 10, "x");

    beginCapture();
    bool ok = dangKyMon(&sv, &c, &g_pq, 1);
    endCapture();

    CHECK(!ok, "Tra ve false");
    CHECK(capturedHas("vuot qua so mon toi da"), "In thong bao 'vuot qua so mon toi da'");
    CHECK_INT(sv.regCount, 2, "regCount van = 2");
    CHECK(!hasCourse(&sv, "MA101"), "Mon moi KHONG duoc them");
    CHECK_INT(c.currentEnrolled, 10, "Si so khong doi");

    // Dang ky tuan tu cho den khi cham tran
    Student sv2 = makeStudent("S2", "SV2", 3);
    Course a = makeCourse("A1", "A", 1, 99, 0, "x");
    Course b = makeCourse("B1", "B", 1, 99, 0, "x");
    Course d = makeCourse("D1", "D", 1, 99, 0, "x");
    Course e = makeCourse("E1", "E", 1, 99, 0, "x");
    beginCapture();
    bool r1 = dangKyMon(&sv2, &a, &g_pq, 1);
    bool r2 = dangKyMon(&sv2, &b, &g_pq, 1);
    bool r3 = dangKyMon(&sv2, &d, &g_pq, 1);
    bool r4 = dangKyMon(&sv2, &e, &g_pq, 1);
    endCapture();
    CHECK(r1 && r2 && r3, "3 mon dau (bang maxCourses = 3) deu thanh cong");
    CHECK(!r4, "Mon thu 4 bi tu choi");
    CHECK_INT(sv2.regCount, 3, "regCount dung o 3");
    CHECK_INT(e.currentEnrolled, 0, "Mon bi tu choi khong bi tang si so");
}

static void test_register_full_waitlist() {
    SECTION("9. Mon da day -> vao Waitlist");
    initPQ(&g_pq);
    Student sv = makeStudent("24110002", "Le Thi 2", 5);
    Course c = makeCourse("CS201", "Cau truc du lieu", 3, 60, 60, "x");

    beginCapture();
    bool ok = dangKyMon(&sv, &c, &g_pq, 7);
    endCapture();

    CHECK(!ok, "Tra ve false (chua duoc vao hoc chinh thuc)");
    CHECK(capturedHas("Waitlist"), "In thong bao da duoc dua vao Waitlist");
    CHECK_INT(g_pq.size, 1, "Waitlist co 1 phan tu");
    CHECK_STR(g_pq.arr[0].studentId, "24110002", "Waitlist: dung ma sinh vien");
    CHECK_STR(g_pq.arr[0].courseId, "CS201", "Waitlist: dung ma mon");
    CHECK_INT(g_pq.arr[0].priority, 7, "Waitlist: dung do uu tien truyen vao");
    CHECK_INT(sv.regCount, 0, "Sinh vien CHUA co mon trong danh sach chinh thuc");
    CHECK_INT(c.currentEnrolled, 60, "Si so van 60/60 (khong vuot suc chua)");
}

static void test_waitlist_priority() {
    SECTION("10. Waitlist sap xep theo do uu tien nhieu sinh vien");
    initPQ(&g_pq);
    Course c = makeCourse("CS201", "Mon day", 3, 10, 10, "x");
    Student a = makeStudent("A", "A", 5);
    Student b = makeStudent("B", "B", 5);
    Student d = makeStudent("D", "D", 5);
    beginCapture();
    dangKyMon(&a, &c, &g_pq, 1);
    dangKyMon(&b, &c, &g_pq, 5);   // sinh vien nam cuoi - uu tien cao
    dangKyMon(&d, &c, &g_pq, 3);
    endCapture();
    CHECK_INT(g_pq.size, 3, "Waitlist co 3 sinh vien");
    CHECK_STR(g_pq.arr[0].studentId, "B", "Dau hang doi: B (uu tien 5)");
    CHECK_STR(g_pq.arr[1].studentId, "D", "Tiep theo: D (uu tien 3)");
    CHECK_STR(g_pq.arr[2].studentId, "A", "Cuoi hang doi: A (uu tien 1)");
}

static void test_validation_order() {
    SECTION("11. Thu tu kiem tra dieu kien");
    // (a) Sinh vien da cham tran VA mon da day -> bi tu choi, KHONG vao waitlist
    initPQ(&g_pq);
    Student full = makeStudent("S", "SV", 1);
    giveCourse(&full, "CS101");
    Course fullCourse = makeCourse("MA101", "Mon", 3, 10, 10, "x");
    beginCapture();
    bool ok = dangKyMon(&full, &fullCourse, &g_pq, 1);
    endCapture();
    CHECK(!ok, "SV cham tran + mon day -> tu choi");
    CHECK(capturedHas("vuot qua so mon toi da"), "  Bao loi vuot so mon (khong phai thong bao waitlist)");
    CHECK_INT(g_pq.size, 0, "  Khong bi dua vao waitlist");

    // (b) Sinh vien da dang ky mon nay VA mon day -> bao trung, KHONG vao waitlist
    initPQ(&g_pq);
    Student dup = makeStudent("S", "SV", 5);
    giveCourse(&dup, "MA101");
    beginCapture();
    ok = dangKyMon(&dup, &fullCourse, &g_pq, 1);
    endCapture();
    CHECK(!ok, "SV da dang ky + mon day -> tu choi");
    CHECK(capturedHas("da dang ky mon nay"), "  Bao loi trung mon");
    CHECK_INT(g_pq.size, 0, "  Khong bi dua vao waitlist");
}

static void test_waitlist_duplicate() {
    SECTION("12. Han che: cung sinh vien vao waitlist nhieu lan");
    initPQ(&g_pq);
    Student sv = makeStudent("S", "SV", 5);
    Course c = makeCourse("CS201", "Mon day", 3, 10, 10, "x");
    beginCapture();
    dangKyMon(&sv, &c, &g_pq, 1);
    dangKyMon(&sv, &c, &g_pq, 1);
    endCapture();
    NOTE(g_pq.size == 1, "Dang ky mon day 2 lan -> waitlist co 2 ban ghi trung (chua chan trung)");
}

// ---------------- heThong_DangKyMon ----------------
static void test_system_level() {
    SECTION("13. heThong_DangKyMon (tich hop he thong + Undo)");
    setupHeThong(&g_ht);
    addCourseToSystem(&g_ht, makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "Thu 2"));
    addCourseToSystem(&g_ht, makeCourse("CS201", "Cau truc du lieu", 3, 60, 60, "Thu 3"));
    Student* sv = addStudentToSystem(&g_ht, makeStudent("24110001", "Nguyen Van 1", 5));

    // Thanh cong
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "CS101");
    endCapture();
    CHECK(capturedHas("Dang ky thanh cong"), "In thong bao 'Dang ky thanh cong'");
    CHECK(hasCourse(sv, "CS101"), "Sinh vien co mon CS101");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled, 46, "Si so trong bang bam tang 45 -> 46");
    CHECK_INT(g_ht.lichSu.top, 0, "Hanh dong duoc ghi vao lich su Undo");
    CHECK_STR(g_ht.lichSu.arr[0].type, "DANG_KI", "  Loai hanh dong = DANG_KI");
    CHECK_STR(g_ht.lichSu.arr[0].courseId, "CS101", "  Ma mon dung");

    // Mon day -> waitlist, KHONG ghi undo
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "CS201");
    endCapture();
    CHECK(!capturedHas("Dang ky thanh cong"), "Mon day: khong bao 'Dang ky thanh cong'");
    CHECK(capturedHas("Waitlist"), "Mon day: bao da vao Waitlist");
    CHECK_INT(g_ht.waitlist.size, 1, "Waitlist cua he thong co 1 phan tu");
    CHECK_INT(g_ht.lichSu.top, 0, "Vao waitlist KHONG ghi them vao lich su Undo");
    CHECK(!hasCourse(sv, "CS201"), "Sinh vien khong co mon CS201 chinh thuc");

    // Trung mon -> khong ghi undo
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "CS101");
    endCapture();
    CHECK_INT(g_ht.lichSu.top, 0, "Dang ky trung: khong ghi them Undo");
    CHECK_INT(sv->regCount, 1, "Dang ky trung: regCount van = 1");

    // Sinh vien khong ton tai
    beginCapture();
    heThong_DangKyMon(&g_ht, "00000000", "CS101");
    endCapture();
    CHECK(capturedHas("Khong tim thay sinh vien"), "SV khong ton tai -> bao loi");
    CHECK_INT(g_ht.lichSu.top, 0, "  Khong ghi Undo");

    // Mon khong ton tai
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "NOPE99");
    endCapture();
    CHECK(capturedHas("Khong tim thay mon hoc"), "Mon khong ton tai -> bao loi");
    CHECK_INT(g_ht.lichSu.top, 0, "  Khong ghi Undo");
    CHECK_INT(sv->regCount, 1, "  regCount khong doi");
}

static void test_real_data() {
    SECTION("14. Tich hop du lieu that (students.json + courses.csv)");
    setupHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    docFileSinhVien(&g_ht, "data/students.json");
    for (int i = 0; i < g_ht.soLuongMon; i++) insertCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i]);

    // 24110003 co 0 mon, maxCourses = 6; CS101 con cho (45/50)
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110003", "CS101");
    endCapture();
    CHECK(capturedHas("Dang ky thanh cong"), "SV 24110003 dang ky CS101 (45/50) thanh cong");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled, 46, "CS101: 45 -> 46");

    // CS201 da day 60/60
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110003", "CS201");
    endCapture();
    CHECK(capturedHas("Waitlist"), "SV 24110003 dang ky CS201 (60/60) -> vao Waitlist");
    CHECK_INT(g_ht.waitlist.size, 1, "Waitlist co 1 phan tu");

    // 24110004 da co CS101 -> dang ky lai se bi bao trung
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110004", "CS101");
    endCapture();
    CHECK(capturedHas("da dang ky mon nay"), "SV 24110004 (da co CS101) dang ky lai -> bao trung");
}

int main() {
    TEST_BEGIN("CHUC NANG 3 - DANG KY HOC PHAN + WAITLIST (DangKi / Priority Queue)");
    test_pq_basic();
    test_pq_order();
    test_pq_fifo();
    test_pq_full();
    test_register_success();
    test_register_last_slot();
    test_register_duplicate();
    test_register_over_limit();
    test_register_full_waitlist();
    test_waitlist_priority();
    test_validation_order();
    test_waitlist_duplicate();
    test_system_level();
    test_real_data();
    return testSummary();
}
