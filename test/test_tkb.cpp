// ============================================================
//  TEST CHUC NANG 2: XEM THOI KHOA BIEU
//  File nguon: src/core/dsa/TKB.cpp  (+ heThong_XemTKB trong HeThong.cpp)
// ============================================================
#include "test_helper.h"
#include "../src/persistence/DataLoad.h"

static TraCuu g_tc;
static HeThong g_ht;

static void buildCourses() {
    initTraCuu(&g_tc);
    insertCourseHT(&g_tc, makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "Thu 2 - Ca 1 - A1.101"));
    insertCourseHT(&g_tc, makeCourse("CS201", "Cau truc du lieu", 4, 60, 60, "Thu 3 - Ca 1 - A2.101"));
    insertCourseHT(&g_tc, makeCourse("MA101", "Giai tich 1", 2, 80, 10, "Thu 5 - Ca 3 - C1.101"));
}

static void test_empty_schedule() {
    SECTION("1. Sinh vien chua dang ky mon nao");
    buildCourses();
    Student sv = makeStudent("24110003", "Tran Van 3", 6);

    beginCapture();
    inThoiKhoaBieu(&sv, &g_tc);
    endCapture();

    CHECK(capturedHas("Tran Van 3"), "In ten sinh vien");
    CHECK(capturedHas("24110003"), "In ma sinh vien");
    CHECK(capturedHas("0/6"), "In so mon dang ky dang 0/6");
    CHECK(capturedHas("chua dang ky mon hoc nao"), "In thong bao 'chua dang ky mon hoc nao'");
    CHECK(!capturedHas("Lich hoc"), "Khong in dong lich hoc nao");
}

static void test_with_courses() {
    SECTION("2. Sinh vien co mon da dang ky");
    buildCourses();
    Student sv = makeStudent("24110001", "Nguyen Van 1", 5);
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "CS201");

    beginCapture();
    inThoiKhoaBieu(&sv, &g_tc);
    endCapture();

    CHECK(capturedHas("Nguyen Van 1"), "In ten sinh vien");
    CHECK(capturedHas("(24110001)"), "In ma sinh vien trong ngoac");
    CHECK(capturedHas("2/5"), "In so mon dang ky 2/5");
    CHECK(capturedHas("CS101"), "In ma mon CS101");
    CHECK(capturedHas("Nhap mon Lap trinh"), "In ten mon CS101 (lay tu bang bam)");
    CHECK(capturedHas("(3TC)"), "In so tin chi (3TC)");
    CHECK(capturedHas("Thu 2 - Ca 1 - A1.101"), "In lich hoc CS101");
    CHECK(capturedHas("CS201"), "In ma mon CS201");
    CHECK(capturedHas("Cau truc du lieu"), "In ten mon CS201");
    CHECK(capturedHas("(4TC)"), "In so tin chi (4TC) cua CS201");
    CHECK(capturedHas("Thu 3 - Ca 1 - A2.101"), "In lich hoc CS201");
    CHECK(!capturedHas("chua dang ky mon hoc nao"), "Khong in thong bao 'chua dang ky'");
    CHECK(!capturedHas("MA101"), "Khong in mon sinh vien KHONG dang ky (MA101)");
}

static void test_order() {
    SECTION("3. Thu tu hien thi theo thu tu dang ky");
    buildCourses();
    Student sv = makeStudent("S1", "Sinh Vien Thu Tu", 5);
    giveCourse(&sv, "MA101");
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "CS201");

    beginCapture();
    inThoiKhoaBieu(&sv, &g_tc);
    endCapture();

    int pMA = capturedPos("MA101 |");
    int pCS1 = capturedPos("CS101 |");
    int pCS2 = capturedPos("CS201 |");
    CHECK(pMA >= 0 && pCS1 >= 0 && pCS2 >= 0, "Ca 3 mon deu xuat hien");
    CHECK(pMA < pCS1 && pCS1 < pCS2, "Thu tu in: MA101 -> CS101 -> CS201 (dung thu tu dang ky)");
}

static void test_missing_course_info() {
    SECTION("4. Mon da dang ky nhung khong co trong bang bam");
    buildCourses();
    Student sv = makeStudent("S2", "Sinh Vien Loi", 5);
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "GHOST99"); // khong ton tai trong bang bam
    giveCourse(&sv, "MA101");

    beginCapture();
    inThoiKhoaBieu(&sv, &g_tc);   // khong duoc crash
    endCapture();

    CHECK(capturedHas("GHOST99"), "Van in ma mon bi thieu thong tin");
    CHECK(capturedHas("Khong tim thay thong tin"), "In canh bao 'Khong tim thay thong tin'");
    CHECK(capturedHas("Nhap mon Lap trinh"), "Mon dung truoc do van hien thi binh thuong");
    CHECK(capturedHas("Giai tich 1"), "Mon dung sau do van hien thi binh thuong");
}

static void test_no_side_effect() {
    SECTION("5. Xem TKB khong lam thay doi du lieu");
    buildCourses();
    Student sv = makeStudent("S3", "Sinh Vien Bat Bien", 5);
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "MA101");
    int before = g_tc.count;
    int enrolledBefore = searchCourseHT(&g_tc, "CS101")->currentEnrolled;

    beginCapture();
    inThoiKhoaBieu(&sv, &g_tc);
    inThoiKhoaBieu(&sv, &g_tc);   // goi 2 lan
    endCapture();

    CHECK_INT(sv.regCount, 2, "regCount khong doi");
    CHECK_STR(sv.registeredCourses[0], "CS101", "Mon thu 1 khong doi");
    CHECK_STR(sv.registeredCourses[1], "MA101", "Mon thu 2 khong doi");
    CHECK_INT(g_tc.count, before, "So phan tu bang bam khong doi");
    CHECK_INT(searchCourseHT(&g_tc, "CS101")->currentEnrolled, enrolledBefore, "Si so mon hoc khong doi");
}

static void test_full_schedule() {
    SECTION("6. Sinh vien dang ky day (3/3)");
    buildCourses();
    Student sv = makeStudent("S4", "Sinh Vien Day", 3);
    giveCourse(&sv, "CS101");
    giveCourse(&sv, "CS201");
    giveCourse(&sv, "MA101");

    beginCapture();
    inThoiKhoaBieu(&sv, &g_tc);
    endCapture();
    CHECK(capturedHas("3/3"), "Hien thi 3/3 khi dang ky du so mon toi da");
    CHECK(capturedHas("CS101") && capturedHas("CS201") && capturedHas("MA101"), "In du ca 3 mon");
}

static void test_system_level() {
    SECTION("7. heThong_XemTKB (tich hop voi du lieu that)");
    initHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    docFileSinhVien(&g_ht, "data/students.json");
    for (int i = 0; i < g_ht.soLuongMon; i++) insertCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i]);
    CHECK(g_ht.soLuongSV > 0 && g_ht.soLuongMon > 0, "Nap du lieu mon hoc + sinh vien thanh cong");
    printf("         (%d mon hoc, %d sinh vien)\n", g_ht.soLuongMon, g_ht.soLuongSV);

    // Sinh vien 24110001: CS101, CS201
    beginCapture();
    heThong_XemTKB(&g_ht, "24110001");
    endCapture();
    CHECK(capturedHas("Nguyen Van 1"), "SV 24110001: in dung ten");
    CHECK(capturedHas("CS101") && capturedHas("CS201"), "SV 24110001: in CS101 va CS201");
    CHECK(capturedHas("Nhap mon Lap trinh"), "SV 24110001: ten mon lay dung tu bang bam");
    CHECK(capturedHas("2/5"), "SV 24110001: 2/5 mon");

    // Sinh vien 24110003: chua dang ky gi
    beginCapture();
    heThong_XemTKB(&g_ht, "24110003");
    endCapture();
    CHECK(capturedHas("chua dang ky mon hoc nao"), "SV 24110003: chua dang ky mon nao");

    // Sinh vien 24110004: CS101, CS102, MA101
    beginCapture();
    heThong_XemTKB(&g_ht, "24110004");
    endCapture();
    CHECK(capturedHas("3/5"), "SV 24110004: 3/5 mon");
    CHECK(capturedHas("CS102") && capturedHas("MA101"), "SV 24110004: in CS102 va MA101");

    // Sinh vien khong ton tai
    beginCapture();
    heThong_XemTKB(&g_ht, "99999999");
    endCapture();
    CHECK(capturedHas("Khong tim thay sinh vien"), "Ma SV khong ton tai -> bao loi");
    CHECK(capturedHas("99999999"), "  Thong bao loi co kem ma SV da nhap");

    // Ma rong
    beginCapture();
    heThong_XemTKB(&g_ht, "");
    endCapture();
    CHECK(capturedHas("Khong tim thay sinh vien"), "Ma SV rong -> bao loi");

    // Quet toan bo 100 sinh vien: khong sinh vien nao bi 'Loi: Khong tim thay thong tin'
    bool anyMissing = false;
    int svBad = 0;
    for (int i = 0; i < g_ht.soLuongSV; i++) {
        beginCapture();
        heThong_XemTKB(&g_ht, g_ht.dsSinhVien[i].id);
        endCapture();
        if (capturedHas("Khong tim thay thong tin")) { anyMissing = true; svBad++; }
    }
    CHECK(!anyMissing, "Quet toan bo sinh vien: moi mon da dang ky deu co thong tin trong bang bam");
    if (anyMissing) printf("         (%d sinh vien co mon khong co trong courses.csv)\n", svBad);
}

int main() {
    TEST_BEGIN("CHUC NANG 2 - XEM THOI KHOA BIEU (TKB)");
    test_empty_schedule();
    test_with_courses();
    test_order();
    test_missing_course_info();
    test_no_side_effect();
    test_full_schedule();
    test_system_level();
    return testSummary();
}
