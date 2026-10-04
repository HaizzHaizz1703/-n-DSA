// ============================================================
//  TEST CHUC NANG 5: HOAN TAC (UNDO) - Stack
//  File nguon: src/core/dsa/Undo.cpp (+ heThong_HoanTac)
// ============================================================
#include "test_helper.h"
#include "../src/persistence/DataLoad.h"

static UndoStack g_st;
static HeThong g_ht;

// ---------------- Stack thuan tuy ----------------
static void test_stack_init() {
    SECTION("1. Khoi tao stack");
    memset(&g_st, 0xFF, sizeof(g_st));
    initStack(&g_st);
    CHECK_INT(g_st.top, -1, "top = -1 (stack rong)");

    Action out;
    strcpy(out.type, "GIU_NGUYEN");
    bool ok = popUndo(&g_st, &out);
    CHECK(!ok, "Pop stack rong -> false");
    CHECK_STR(out.type, "GIU_NGUYEN", "Pop that bai khong ghi de len bien dau ra");
    CHECK_INT(g_st.top, -1, "top van = -1 sau pop that bai");
}

static void test_stack_push_pop() {
    SECTION("2. Push va Pop mot phan tu");
    initStack(&g_st);
    pushUndo(&g_st, "DANG_KI", "24110001", "CS101");
    CHECK_INT(g_st.top, 0, "Push 1 phan tu -> top = 0");

    Action out;
    bool ok = popUndo(&g_st, &out);
    CHECK(ok, "Pop thanh cong");
    CHECK_STR(out.type, "DANG_KI", "type dung");
    CHECK_STR(out.studentId, "24110001", "studentId dung");
    CHECK_STR(out.courseId, "CS101", "courseId dung");
    CHECK_INT(g_st.top, -1, "Sau pop -> stack rong lai");
    CHECK(!popUndo(&g_st, &out), "Pop lan nua -> false");
}

static void test_stack_lifo() {
    SECTION("3. Thu tu LIFO (vao sau ra truoc)");
    initStack(&g_st);
    pushUndo(&g_st, "DANG_KI", "SV1", "A");
    pushUndo(&g_st, "HUY",     "SV2", "B");
    pushUndo(&g_st, "DANG_KI", "SV3", "C");
    CHECK_INT(g_st.top, 2, "top = 2 sau 3 lan push");

    Action out;
    popUndo(&g_st, &out);
    CHECK_STR(out.courseId, "C", "Pop lan 1 -> C (vao cuoi)");
    CHECK_STR(out.type, "DANG_KI", "  type = DANG_KI");
    popUndo(&g_st, &out);
    CHECK_STR(out.courseId, "B", "Pop lan 2 -> B");
    CHECK_STR(out.type, "HUY", "  type = HUY");
    popUndo(&g_st, &out);
    CHECK_STR(out.courseId, "A", "Pop lan 3 -> A (vao dau)");
    CHECK_INT(g_st.top, -1, "Stack rong sau 3 lan pop");
}

static void test_stack_interleave() {
    SECTION("4. Push/Pop xen ke");
    initStack(&g_st);
    Action out;
    pushUndo(&g_st, "DANG_KI", "S", "A");
    pushUndo(&g_st, "DANG_KI", "S", "B");
    popUndo(&g_st, &out);                      // lay B
    pushUndo(&g_st, "HUY", "S", "C");
    popUndo(&g_st, &out);
    CHECK_STR(out.courseId, "C", "Sau push C -> pop ra C");
    popUndo(&g_st, &out);
    CHECK_STR(out.courseId, "A", "Pop tiep ra A (B da bi lay truoc do)");
    CHECK(!popUndo(&g_st, &out), "Het phan tu");
}

static void test_stack_capacity() {
    SECTION("5. Stack day (100 phan tu)");
    initStack(&g_st);
    char cid[20];
    for (int i = 0; i < 100; i++) {
        sprintf(cid, "C%03d", i);
        pushUndo(&g_st, "DANG_KI", "SV", cid);
    }
    CHECK_INT(g_st.top, 99, "top = 99 khi day 100 phan tu");
    pushUndo(&g_st, "HUY", "SV", "TRAN");
    CHECK_INT(g_st.top, 99, "Push khi day -> bi bo qua, top van = 99");

    Action out;
    popUndo(&g_st, &out);
    CHECK_STR(out.courseId, "C099", "Phan tu tren cung van la C099 (phan tu tran khong lot vao)");

    int popped = 1;
    while (popUndo(&g_st, &out)) popped++;
    CHECK_INT(popped, 100, "Pop duoc dung 100 phan tu");
    CHECK_INT(g_st.top, -1, "Stack rong");

    // Dung lai sau khi da rong
    pushUndo(&g_st, "HUY", "SV", "MOI");
    CHECK_INT(g_st.top, 0, "Dung lai binh thuong sau khi rong");
}

// ---------------- heThong_HoanTac ----------------
static void prepareSystem() {
    setupHeThong(&g_ht);
    addCourseToSystem(&g_ht, makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "x"));
    addCourseToSystem(&g_ht, makeCourse("MA101", "Giai tich 1", 3, 50, 10, "x"));
    addStudentToSystem(&g_ht, makeStudent("24110001", "Nguyen Van 1", 5));
}

static void test_undo_empty() {
    SECTION("6. Hoan tac khi chua co thao tac nao");
    prepareSystem();
    beginCapture();
    heThong_HoanTac(&g_ht);
    endCapture();
    CHECK(capturedHas("Khong co thao tac nao de hoan tac"), "In thong bao 'Khong co thao tac nao de hoan tac'");
    CHECK_INT(g_ht.dsSinhVien[0].regCount, 0, "Du lieu khong thay doi");
}

static void test_undo_register() {
    SECTION("7. Hoan tac mot lan DANG KY");
    prepareSystem();
    Student* sv = &g_ht.dsSinhVien[0];
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "CS101");
    endCapture();
    CHECK(hasCourse(sv, "CS101"), "Tien de: da dang ky CS101");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled, 46, "Tien de: si so = 46");

    beginCapture();
    heThong_HoanTac(&g_ht);
    endCapture();
    CHECK(capturedHas("hoan tac"), "In thong bao dang hoan tac");
    CHECK(capturedHas("DANG_KI"), "  Thong bao co ghi loai thao tac DANG_KI");
    CHECK(!hasCourse(sv, "CS101"), "Sau Undo: sinh vien khong con CS101");
    CHECK_INT(sv->regCount, 0, "Sau Undo: regCount = 0");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled, 45, "Sau Undo: si so tra ve 45");
}

static void test_undo_cancel() {
    SECTION("8. Hoan tac mot lan HUY");
    prepareSystem();
    Student* sv = &g_ht.dsSinhVien[0];
    giveCourse(sv, "MA101");
    beginCapture();
    heThong_HuyMon(&g_ht, "24110001", "MA101");
    endCapture();
    CHECK(!hasCourse(sv, "MA101"), "Tien de: da huy MA101");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "MA101")->currentEnrolled, 9, "Tien de: si so = 9");

    beginCapture();
    heThong_HoanTac(&g_ht);
    endCapture();
    CHECK(capturedHas("HUY"), "Thong bao ghi loai thao tac HUY");
    CHECK(hasCourse(sv, "MA101"), "Sau Undo: MA101 duoc dang ky lai");
    CHECK_INT(sv->regCount, 1, "Sau Undo: regCount = 1");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "MA101")->currentEnrolled, 10, "Sau Undo: si so tra ve 10");
}

static void test_undo_multiple() {
    SECTION("9. Hoan tac nhieu buoc theo thu tu nguoc");
    prepareSystem();
    Student* sv = &g_ht.dsSinhVien[0];
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "CS101");   // buoc 1
    heThong_DangKyMon(&g_ht, "24110001", "MA101");   // buoc 2
    endCapture();
    CHECK_INT(sv->regCount, 2, "Tien de: da dang ky 2 mon");

    beginCapture();
    heThong_HoanTac(&g_ht);                          // go buoc 2
    endCapture();
    CHECK(hasCourse(sv, "CS101") && !hasCourse(sv, "MA101"), "Undo lan 1: go MA101 (buoc gan nhat), giu CS101");
    CHECK_INT(sv->regCount, 1, "  regCount = 1");
}

static void test_undo_history_cleanup() {
    SECTION("10. Sau Undo, lich su khong bi 'nhiem' them thao tac moi");
    prepareSystem();
    beginCapture();
    heThong_DangKyMon(&g_ht, "24110001", "CS101");
    endCapture();
    CHECK_INT(g_ht.lichSu.top, 0, "Tien de: lich su co 1 thao tac");

    beginCapture();
    heThong_HoanTac(&g_ht);
    endCapture();
    // Mong doi dung: Undo lay thao tac ra -> lich su rong (top = -1).
    // Neu HoanTac goi lai heThong_HuyMon (co pushUndo) thi top = 0 -> Undo lan 2 se "Undo cua Undo".
    CHECK_INT(g_ht.lichSu.top, -1, "Sau Undo: lich su rong (Undo khong tu ghi them thao tac moi)");

    beginCapture();
    heThong_HoanTac(&g_ht);   // Undo lan 2: mong doi 'khong co gi de hoan tac'
    endCapture();
    CHECK(capturedHas("Khong co thao tac nao de hoan tac"),
          "Undo lan 2 -> 'Khong co thao tac nao de hoan tac' (khong bi dang ky lai ngoai y muon)");
    CHECK(!hasCourse(&g_ht.dsSinhVien[0], "CS101"), "Sinh vien KHONG bi dang ky lai CS101 sau Undo lan 2");
}

static void test_undo_course_full() {
    SECTION("11. Undo HUY khi mon da bi nguoi khac lap day cho");
    setupHeThong(&g_ht);
    addCourseToSystem(&g_ht, makeCourse("CS201", "Mon", 3, 2, 2, "x"));
    Student a = makeStudent("A", "A", 5); giveCourse(&a, "CS201");
    Student b = makeStudent("B", "B", 5); giveCourse(&b, "CS201");
    Student c = makeStudent("C", "C", 5);
    Student* sa = addStudentToSystem(&g_ht, a);
    addStudentToSystem(&g_ht, b);
    Student* sc = addStudentToSystem(&g_ht, c);

    beginCapture();
    heThong_HuyMon(&g_ht, "A", "CS201");       // A huy -> con 1/2
    heThong_DangKyMon(&g_ht, "C", "CS201");    // C chiem cho -> 2/2
    // Lich su luc nay: [HUY A, DANG_KI C]; Undo 1 lan se go C. De test dung kich ban, xoa 'DANG_KI C':
    Action tmp; popUndo(&g_ht.lichSu, &tmp);
    heThong_HoanTac(&g_ht);                    // Undo HUY cua A khi mon da day 2/2
    endCapture();

    CHECK(hasCourse(sc, "CS201"), "C van giu cho cua minh");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS201")->currentEnrolled, 2, "Si so khong vuot suc chua (2/2)");
    CHECK(!hasCourse(sa, "CS201"), "A khong the vao lai vi mon da day");
    CHECK_INT(g_ht.waitlist.size, 1, "A duoc dua vao Waitlist thay vi vao thang lop");
}

static void test_real_data() {
    SECTION("12. Tich hop du lieu that: Dang ky -> Huy -> Undo -> Undo");
    setupHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    docFileSinhVien(&g_ht, "data/students.json");
    for (int i = 0; i < g_ht.soLuongMon; i++) insertCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i]);

    Student* sv = nullptr;
    for (int i = 0; i < g_ht.soLuongSV; i++)
        if (strcmp(g_ht.dsSinhVien[i].id, "24110004") == 0) sv = &g_ht.dsSinhVien[i];
    CHECK(sv != nullptr, "Tim thay SV 24110004");
    if (!sv) return;
    int startCount = sv->regCount;                       // 3 mon: CS101, CS102, MA101
    int startEnrolled = searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled;

    beginCapture();
    heThong_HuyMon(&g_ht, "24110004", "CS101");         // buoc 1: huy CS101
    heThong_DangKyMon(&g_ht, "24110004", "SE203");      // buoc 2: dang ky SE203
    endCapture();
    CHECK(!hasCourse(sv, "CS101") && hasCourse(sv, "SE203"), "Sau 2 buoc: mat CS101, co SE203");

    beginCapture();
    heThong_HoanTac(&g_ht);                              // Undo buoc 2
    endCapture();
    CHECK(!hasCourse(sv, "SE203"), "Undo 1: SE203 bi go");
    CHECK(!hasCourse(sv, "CS101"), "Undo 1: CS101 van chua quay lai");

    beginCapture();
    heThong_HoanTac(&g_ht);                              // Undo buoc 1
    endCapture();
    CHECK(hasCourse(sv, "CS101"), "Undo 2: CS101 duoc dang ky lai");
    CHECK_INT(sv->regCount, startCount, "Tro ve so mon ban dau");
    CHECK_INT(searchCourseHT(&g_ht.bangBam, "CS101")->currentEnrolled, startEnrolled, "Si so CS101 tro ve gia tri ban dau");
}

int main() {
    TEST_BEGIN("CHUC NANG 5 - HOAN TAC / UNDO (Undo / Stack)");
    test_stack_init();
    test_stack_push_pop();
    test_stack_lifo();
    test_stack_interleave();
    test_stack_capacity();
    test_undo_empty();
    test_undo_register();
    test_undo_cancel();
    test_undo_multiple();
    test_undo_history_cleanup();
    test_undo_course_full();
    test_real_data();
    return testSummary();
}
