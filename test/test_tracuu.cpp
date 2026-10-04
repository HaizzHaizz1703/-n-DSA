// ============================================================
//  TEST CHUC NANG 1: TRA CUU HOC PHAN (Bang bam - Linear Probing)
//  File nguon: src/core/dsa/TraCuu.cpp
// ============================================================
#include "test_helper.h"
#include "../src/persistence/DataLoad.h"

// Bang bam kha lon -> de static de khong tran stack
static TraCuu g_tc;
static HeThong g_ht;

static void test_init() {
    SECTION("1. Khoi tao bang bam (initTraCuu)");
    // Lam ban bo nho truoc de chac chan init phai ghi de
    memset(&g_tc, 0xFF, sizeof(g_tc));
    initTraCuu(&g_tc);
    CHECK_INT(g_tc.count, 0, "count = 0 sau khi init");
    bool allFree = true;
    for (int i = 0; i < HASH_SIZE; i++) if (g_tc.isOccupied[i]) allFree = false;
    CHECK(allFree, "Tat ca o deu trong (isOccupied = false)");
    CHECK(searchCourseHT(&g_tc, "CS101") == nullptr, "Tim trong bang rong -> nullptr");
}

static void test_hash_function() {
    SECTION("2. Ham bam (hashCourseId)");
    // 'C'=67 'S'=83 '1'=49 '0'=48 '1'=49 -> 296 % 200 = 96
    CHECK_INT(hashCourseId("CS101"), 96, "hash(\"CS101\") = 296 % 200 = 96");
    CHECK_INT(hashCourseId("CS101"), hashCourseId("CS101"), "Ham bam on dinh (cung vao -> cung ra)");
    CHECK_INT(hashCourseId(""), 0, "Chuoi rong -> hash = 0");
    CHECK_INT(hashCourseId("AB"), hashCourseId("BA"), "Hai chuoi hoan vi co cung hash (dac tinh cua ham tong ASCII)");

    bool inRange = true;
    const char* ids[] = { "CS101", "SE201", "MA103", "IS301", "LL101", "PH102", "ZZZZZZZZZZ", "a", "~~~~~~" };
    for (int i = 0; i < 9; i++) {
        int h = hashCourseId(ids[i]);
        if (h < 0 || h >= HASH_SIZE) inRange = false;
    }
    CHECK(inRange, "Hash luon nam trong [0, HASH_SIZE)");
}

static void test_insert_search() {
    SECTION("3. Them va tim kiem co ban");
    initTraCuu(&g_tc);
    Course c1 = makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "Thu 2 - Ca 1 - A1.101");
    Course c2 = makeCourse("SE201", "Lap trinh Web", 3, 40, 40, "Thu 4 - Ca 1 - B1.201");
    insertCourseHT(&g_tc, c1);
    insertCourseHT(&g_tc, c2);

    CHECK_INT(g_tc.count, 2, "count = 2 sau khi them 2 mon");

    Course* r1 = searchCourseHT(&g_tc, "CS101");
    CHECK(r1 != nullptr, "Tim thay CS101");
    if (r1) {
        CHECK_STR(r1->id, "CS101", "  id dung");
        CHECK_STR(r1->name, "Nhap mon Lap trinh", "  ten dung");
        CHECK_INT(r1->credits, 3, "  so tin chi dung");
        CHECK_INT(r1->maxCapacity, 50, "  si so toi da dung");
        CHECK_INT(r1->currentEnrolled, 45, "  si so hien tai dung");
        CHECK_STR(r1->lichHoc, "Thu 2 - Ca 1 - A1.101", "  lich hoc dung");
    }
    Course* r2 = searchCourseHT(&g_tc, "SE201");
    CHECK(r2 != nullptr && strcmp(r2->name, "Lap trinh Web") == 0, "Tim thay SE201 voi ten dung");

    CHECK(searchCourseHT(&g_tc, "XX999") == nullptr, "Ma khong ton tai -> nullptr");
    CHECK(searchCourseHT(&g_tc, "CS10") == nullptr, "Ma la tien to cua ma khac (CS10) -> nullptr");
    CHECK(searchCourseHT(&g_tc, "CS1011") == nullptr, "Ma dai hon ma ton tai (CS1011) -> nullptr");
    CHECK(searchCourseHT(&g_tc, "cs101") == nullptr, "Phan biet hoa/thuong (cs101) -> nullptr");
    CHECK(searchCourseHT(&g_tc, "") == nullptr, "Ma rong -> nullptr");
}

static void test_pointer_semantics() {
    SECTION("4. Con tro tra ve tro thang vao bang (sua duoc du lieu)");
    initTraCuu(&g_tc);
    insertCourseHT(&g_tc, makeCourse("CS201", "Cau truc du lieu", 3, 60, 10, "Thu 3"));
    Course* p = searchCourseHT(&g_tc, "CS201");
    CHECK(p != nullptr, "Tim thay CS201");
    if (p) {
        p->currentEnrolled = 59;
        Course* p2 = searchCourseHT(&g_tc, "CS201");
        CHECK_INT(p2->currentEnrolled, 59, "Sua qua con tro -> tim lai thay gia tri moi");
        CHECK(p == p2, "Hai lan tim tra ve cung mot dia chi");
    }
    // Insert luu ban sao: sua doi tuong goc khong anh huong bang
    Course goc = makeCourse("MA101", "Giai tich 1", 4, 80, 5, "Thu 5");
    insertCourseHT(&g_tc, goc);
    goc.currentEnrolled = 999;
    CHECK_INT(searchCourseHT(&g_tc, "MA101")->currentEnrolled, 5, "Insert luu BAN SAO (sua bien goc khong anh huong bang)");
}

static void test_collision() {
    SECTION("5. Xu ly dung do (collision - Linear Probing)");
    initTraCuu(&g_tc);
    // "AB" va "BA" co cung hash
    int h = hashCourseId("AB");
    CHECK_INT(hashCourseId("BA"), h, "Tien de: AB va BA cung hash");

    insertCourseHT(&g_tc, makeCourse("AB", "Mon AB", 1, 10, 0, "x"));
    insertCourseHT(&g_tc, makeCourse("BA", "Mon BA", 2, 10, 0, "y"));
    insertCourseHT(&g_tc, makeCourse("CS101", "Dummy", 3, 10, 0, "z")); // khac hash, khong anh huong

    CHECK_INT(g_tc.count, 3, "count = 3");
    Course* a = searchCourseHT(&g_tc, "AB");
    Course* b = searchCourseHT(&g_tc, "BA");
    CHECK(a != nullptr && strcmp(a->name, "Mon AB") == 0, "Tim dung AB (vao o goc)");
    CHECK(b != nullptr && strcmp(b->name, "Mon BA") == 0, "Tim dung BA (bi day sang o ke tiep)");
    CHECK(a != b, "AB va BA nam o hai o khac nhau");
    CHECK(g_tc.isOccupied[h] && g_tc.isOccupied[(h + 1) % HASH_SIZE], "Hai o lien tiep deu bi chiem");

    // Chuoi dung do dai: 5 ma co cung hash
    initTraCuu(&g_tc);
    const char* anagram[] = { "ABC", "ACB", "BAC", "BCA", "CAB", "CBA" };
    for (int i = 0; i < 6; i++) {
        char nm[20]; sprintf(nm, "Mon %s", anagram[i]);
        insertCourseHT(&g_tc, makeCourse(anagram[i], nm, 1, 10, 0, "x"));
    }
    bool allFound = true;
    for (int i = 0; i < 6; i++) {
        Course* r = searchCourseHT(&g_tc, anagram[i]);
        char expect[20]; sprintf(expect, "Mon %s", anagram[i]);
        if (r == nullptr || strcmp(r->name, expect) != 0) allFound = false;
    }
    CHECK(allFound, "6 ma cung hash (ABC,ACB,...) deu tim dung, khong nham lan");
}

static void test_wraparound() {
    SECTION("6. Quay vong cuoi bang (wrap-around)");
    initTraCuu(&g_tc);
    // "cd" = 99+100 = 199 -> o cuoi cung; "dc" cung hash 199
    CHECK_INT(hashCourseId("cd"), 199, "Tien de: hash(\"cd\") = 199 (o cuoi bang)");
    CHECK_INT(hashCourseId("dc"), 199, "Tien de: hash(\"dc\") = 199");
    insertCourseHT(&g_tc, makeCourse("cd", "Mon cd", 1, 10, 0, "x"));
    insertCourseHT(&g_tc, makeCourse("dc", "Mon dc", 1, 10, 0, "y"));
    CHECK(g_tc.isOccupied[199], "Phan tu dau nam o o 199");
    CHECK(g_tc.isOccupied[0], "Phan tu thu hai quay vong ve o 0");
    CHECK(searchCourseHT(&g_tc, "dc") != nullptr, "Tim thay phan tu da quay vong");
    CHECK(searchCourseHT(&g_tc, "cd") != nullptr, "Tim thay phan tu o 199");
}

static void test_full_table() {
    SECTION("7. Bang day (HASH_SIZE phan tu)");
    initTraCuu(&g_tc);
    char id[20];
    for (int i = 0; i < HASH_SIZE; i++) {
        sprintf(id, "K%03d", i);
        insertCourseHT(&g_tc, makeCourse(id, "Mon day bang", 1, 10, 0, "x"));
    }
    CHECK_INT(g_tc.count, HASH_SIZE, "count = HASH_SIZE khi bang day");

    bool allFound = true;
    for (int i = 0; i < HASH_SIZE; i++) {
        sprintf(id, "K%03d", i);
        if (searchCourseHT(&g_tc, id) == nullptr) allFound = false;
    }
    CHECK(allFound, "Tim thay du 200/200 mon trong bang day");

    insertCourseHT(&g_tc, makeCourse("EXTRA", "Mon thua", 1, 10, 0, "x"));
    CHECK_INT(g_tc.count, HASH_SIZE, "Them phan tu khi bang day -> bi tu choi, count khong doi");
    // Dieu quan trong: tim ma khong ton tai trong bang day phai KET THUC (khong lap vo han)
    CHECK(searchCourseHT(&g_tc, "EXTRA") == nullptr, "Tim ma khong ton tai trong bang day -> nullptr (khong lap vo han)");
    CHECK(searchCourseHT(&g_tc, "NOPE") == nullptr, "Tim ma khac khong ton tai -> nullptr");
}

static void test_duplicate_id() {
    SECTION("8. Them trung ma hoc phan");
    initTraCuu(&g_tc);
    insertCourseHT(&g_tc, makeCourse("CS101", "Ban 1", 3, 50, 0, "x"));
    insertCourseHT(&g_tc, makeCourse("CS101", "Ban 2", 3, 50, 0, "y"));
    Course* r = searchCourseHT(&g_tc, "CS101");
    CHECK(r != nullptr, "Van tim thay CS101");
    CHECK_STR(r ? r->name : nullptr, "Ban 1", "Tim tra ve ban DAU TIEN duoc them");
    NOTE(g_tc.count == 1, "insertCourseHT chua chan trung ma (hien tai count = 2 sau khi them CS101 hai lan)");
}

static void test_real_data() {
    SECTION("9. Tich hop voi du lieu that (data/courses.csv)");
    initHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    CHECK(g_ht.soLuongMon > 0, "Doc duoc du lieu tu data/courses.csv");
    printf("         (da doc %d mon hoc)\n", g_ht.soLuongMon);

    for (int i = 0; i < g_ht.soLuongMon; i++) insertCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i]);
    CHECK_INT(g_ht.bangBam.count, g_ht.soLuongMon, "So phan tu trong bang bam = so mon doc duoc");

    bool allOk = true;
    for (int i = 0; i < g_ht.soLuongMon; i++) {
        Course* r = searchCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i].id);
        if (r == nullptr || strcmp(r->name, g_ht.dsMonHoc[i].name) != 0 ||
            r->credits != g_ht.dsMonHoc[i].credits ||
            r->maxCapacity != g_ht.dsMonHoc[i].maxCapacity) allOk = false;
    }
    CHECK(allOk, "Moi mon trong CSV deu tra cuu duoc va khop du lieu");

    Course* cs101 = searchCourseHT(&(g_ht.bangBam), "CS101");
    CHECK(cs101 != nullptr, "Tra cuu CS101 thanh cong");
    if (cs101) {
        CHECK_STR(cs101->name, "Nhap mon Lap trinh", "  CS101: ten dung");
        CHECK_INT(cs101->maxCapacity, 50, "  CS101: si so toi da = 50");
        CHECK_INT(cs101->currentEnrolled, 45, "  CS101: da dang ky = 45");
        CHECK_STR(cs101->lichHoc, "Thu 2 - Ca 1 - A1.101", "  CS101: lich hoc dung");
    }

    // Kiem tra ham heThong_TraCuuMon (co in ra man hinh)
    beginCapture();
    Course* viaSys = heThong_TraCuuMon(&g_ht, "CS201");
    endCapture();
    CHECK(viaSys != nullptr, "heThong_TraCuuMon(CS201) tra ve con tro hop le");
    CHECK(capturedHas("Cau truc du lieu"), "  Man hinh in ra ten mon hoc");
    CHECK(capturedHas("60/60"), "  Man hinh in ra si so 60/60");

    beginCapture();
    Course* none = heThong_TraCuuMon(&g_ht, "KHONGCO");
    endCapture();
    CHECK(none == nullptr, "heThong_TraCuuMon(ma sai) tra ve nullptr");
    CHECK(capturedHas("Khong tim thay mon hoc"), "  Man hinh in thong bao khong tim thay");
}

int main() {
    TEST_BEGIN("CHUC NANG 1 - TRA CUU HOC PHAN (TraCuu / Hash Table)");
    test_init();
    test_hash_function();
    test_insert_search();
    test_pointer_semantics();
    test_collision();
    test_wraparound();
    test_full_table();
    test_duplicate_id();
    test_real_data();
    return testSummary();
}
