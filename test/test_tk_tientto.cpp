// ============================================================
//  TEST CHUC NANG 6: GOI Y / TIM KIEM THEO TIEN TO (Trie)
//  File nguon: src/core/dsa/TK_TienTo.cpp (+ heThong_GoiYMon)
// ============================================================
#include "test_helper.h"
#include "../src/persistence/DataLoad.h"

static HeThong g_ht;

// Kiem tra node co chua ma mon nay khong
static bool nodeHas(TrieNode* n, const char* id) {
    if (n == nullptr) return false;
    for (int i = 0; i < n->idCount; i++)
        if (strcmp(n->courseIds[i], id) == 0) return true;
    return false;
}

static void test_create_node() {
    SECTION("1. Tao node (createNode)");
    TrieNode* n = createNode();
    CHECK(n != nullptr, "createNode tra ve con tro hop le");
    CHECK_INT(n->idCount, 0, "idCount = 0");
    bool allNull = true;
    for (int i = 0; i < 128; i++) if (n->children[i] != nullptr) allNull = false;
    CHECK(allNull, "Tat ca 128 con tro con deu = nullptr");
    delete n;
}

static void test_insert_search_basic() {
    SECTION("2. Them va tim tien to co ban");
    TrieNode* root = createNode();
    insertTrie(root, "CS101", "CS101");

    TrieNode* n1 = searchPrefixTrie(root, "C");
    CHECK(n1 != nullptr, "Tim tien to 'C' -> co");
    CHECK(nodeHas(n1, "CS101"), "  Node 'C' chua CS101");

    TrieNode* n2 = searchPrefixTrie(root, "CS");
    CHECK(n2 != nullptr && nodeHas(n2, "CS101"), "Tim tien to 'CS' -> chua CS101");

    TrieNode* n3 = searchPrefixTrie(root, "CS10");
    CHECK(n3 != nullptr && nodeHas(n3, "CS101"), "Tim tien to 'CS10' -> chua CS101");

    TrieNode* full = searchPrefixTrie(root, "CS101");
    CHECK(full != nullptr && nodeHas(full, "CS101"), "Tim chinh xac ma day du 'CS101' -> chua CS101");
    CHECK_INT(full->idCount, 1, "  Node cuoi chi co 1 ma");
    CHECK_INT(root->idCount, 0, "Root khong luu ma mon nao");
}

static void test_not_found() {
    SECTION("3. Tien to khong ton tai");
    TrieNode* root = createNode();
    insertTrie(root, "CS101", "CS101");
    insertTrie(root, "SE201", "SE201");

    CHECK(searchPrefixTrie(root, "XX") == nullptr, "'XX' -> nullptr");
    CHECK(searchPrefixTrie(root, "CS2") == nullptr, "'CS2' (nhanh re sai o ky tu cuoi) -> nullptr");
    CHECK(searchPrefixTrie(root, "CS1011") == nullptr, "'CS1011' (dai hon ma ton tai) -> nullptr");
    CHECK(searchPrefixTrie(root, "cs") == nullptr, "'cs' (phan biet hoa thuong) -> nullptr");
    CHECK(searchPrefixTrie(root, "S1") == nullptr, "'S1' (khong phai tien to) -> nullptr");
}

static void test_empty_prefix() {
    SECTION("4. Tien to rong");
    TrieNode* root = createNode();
    insertTrie(root, "CS101", "CS101");
    TrieNode* r = searchPrefixTrie(root, "");
    CHECK(r == root, "Tien to rong tra ve chinh root");
    CHECK_INT(r->idCount, 0, "Root khong co ma nao (khong goi y gi)");

    TrieNode* empty = createNode();
    CHECK(searchPrefixTrie(empty, "A") == nullptr, "Trie rong: tim 'A' -> nullptr");
    insertTrie(empty, "", "X");   // chen chuoi rong khong duoc crash
    CHECK_INT(empty->idCount, 0, "Chen chuoi rong khong lam thay doi gi");
}

static void test_shared_prefix() {
    SECTION("5. Nhieu ma dung chung tien to");
    TrieNode* root = createNode();
    insertTrie(root, "CS101", "CS101");
    insertTrie(root, "CS102", "CS102");
    insertTrie(root, "CS201", "CS201");
    insertTrie(root, "SE201", "SE201");
    insertTrie(root, "MA101", "MA101");

    TrieNode* cs = searchPrefixTrie(root, "CS");
    CHECK_INT(cs->idCount, 3, "'CS' -> 3 mon");
    CHECK(nodeHas(cs, "CS101") && nodeHas(cs, "CS102") && nodeHas(cs, "CS201"), "  Dung 3 mon CS101, CS102, CS201");
    CHECK(!nodeHas(cs, "SE201") && !nodeHas(cs, "MA101"), "  Khong lan mon SE/MA");

    TrieNode* cs1 = searchPrefixTrie(root, "CS1");
    CHECK_INT(cs1->idCount, 2, "'CS1' -> 2 mon");
    CHECK(nodeHas(cs1, "CS101") && nodeHas(cs1, "CS102") && !nodeHas(cs1, "CS201"), "  Dung CS101, CS102 (khong co CS201)");

    TrieNode* cs2 = searchPrefixTrie(root, "CS2");
    CHECK_INT(cs2->idCount, 1, "'CS2' -> 1 mon");
    CHECK(nodeHas(cs2, "CS201"), "  Dung CS201");

    CHECK_INT(searchPrefixTrie(root, "SE")->idCount, 1, "'SE' -> 1 mon");
    CHECK_INT(searchPrefixTrie(root, "MA")->idCount, 1, "'MA' -> 1 mon");
    CHECK_INT(searchPrefixTrie(root, "CS101")->idCount, 1, "'CS101' day du -> 1 mon");
}

static void test_order() {
    SECTION("6. Thu tu ma mon theo thu tu them vao");
    TrieNode* root = createNode();
    insertTrie(root, "CS301", "CS301");
    insertTrie(root, "CS101", "CS101");
    insertTrie(root, "CS201", "CS201");
    TrieNode* cs = searchPrefixTrie(root, "CS");
    CHECK_STR(cs->courseIds[0], "CS301", "Vi tri 0: CS301 (them dau tien)");
    CHECK_STR(cs->courseIds[1], "CS101", "Vi tri 1: CS101");
    CHECK_STR(cs->courseIds[2], "CS201", "Vi tri 2: CS201");
}

static void test_limit_10() {
    SECTION("7. Gioi han toi da 10 goi y moi node");
    TrieNode* root = createNode();
    char id[20];
    for (int i = 0; i < 15; i++) {
        sprintf(id, "CS%03d", 100 + i);
        insertTrie(root, id, id);
    }
    TrieNode* cs = searchPrefixTrie(root, "CS");
    CHECK_INT(cs->idCount, 10, "Node 'CS' chi luu toi da 10 ma (du chen 15)");
    CHECK_STR(cs->courseIds[0], "CS100", "10 ma dau tien duoc giu: dau = CS100");
    CHECK_STR(cs->courseIds[9], "CS109", "10 ma dau tien duoc giu: cuoi = CS109");
    CHECK(!nodeHas(cs, "CS110"), "Ma thu 11 tro di khong duoc luu o node chung");

    // Nhung ma bi cat o node chung van tim duoc o node sau hon
    TrieNode* deep = searchPrefixTrie(root, "CS114");
    CHECK(deep != nullptr && nodeHas(deep, "CS114"), "Ma thu 15 (CS114) van tim thay khi go day du");
    TrieNode* mid = searchPrefixTrie(root, "CS11");
    CHECK(mid != nullptr && mid->idCount == 5, "'CS11' -> 5 mon (CS110..CS114)");
}

static void test_duplicate_insert() {
    SECTION("8. Them trung ma");
    TrieNode* root = createNode();
    insertTrie(root, "CS101", "CS101");
    insertTrie(root, "CS101", "CS101");
    TrieNode* n = searchPrefixTrie(root, "CS101");
    NOTE(n->idCount == 1, "Them cung ma 2 lan -> goi y xuat hien 2 lan (chua chan trung)");
}

static void test_independent_tries() {
    SECTION("9. Hai Trie doc lap");
    TrieNode* a = createNode();
    TrieNode* b = createNode();
    insertTrie(a, "CS101", "CS101");
    insertTrie(b, "MA101", "MA101");
    CHECK(searchPrefixTrie(a, "MA") == nullptr, "Trie A khong chua du lieu cua B");
    CHECK(searchPrefixTrie(b, "CS") == nullptr, "Trie B khong chua du lieu cua A");
}

static void test_special_chars() {
    SECTION("10. Ky tu dac biet trong ASCII");
    TrieNode* root = createNode();
    insertTrie(root, "IS-301_A", "IS-301_A");
    insertTrie(root, "IS 302", "IS 302");
    CHECK(searchPrefixTrie(root, "IS-") != nullptr, "Tim 'IS-' (co dau gach noi)");
    CHECK(searchPrefixTrie(root, "IS ") != nullptr, "Tim 'IS ' (co dau cach)");
    CHECK(nodeHas(searchPrefixTrie(root, "IS-301_"), "IS-301_A"), "Tim 'IS-301_' -> thay IS-301_A");
    CHECK_INT(searchPrefixTrie(root, "IS")->idCount, 2, "'IS' -> 2 mon");
}

static void test_real_data() {
    SECTION("11. Tich hop du lieu that (data/courses.csv)");
    setupHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    for (int i = 0; i < g_ht.soLuongMon; i++) {
        insertCourseHT(&(g_ht.bangBam), g_ht.dsMonHoc[i]);
        insertTrie(g_ht.cayTienTo, g_ht.dsMonHoc[i].id, g_ht.dsMonHoc[i].id);
    }
    CHECK(g_ht.soLuongMon > 0, "Nap du lieu mon hoc thanh cong");

    // 1) Moi ma mon day du phai tim duoc trong Trie
    bool allFound = true;
    for (int i = 0; i < g_ht.soLuongMon; i++) {
        TrieNode* n = searchPrefixTrie(g_ht.cayTienTo, g_ht.dsMonHoc[i].id);
        if (n == nullptr || n->idCount == 0) allFound = false;
    }
    CHECK(allFound, "Moi ma mon trong CSV deu tim thay trong Trie");

    // 2) Doi chieu Trie voi quet tuyen tinh cho cac tien to thuc te
    const char* prefixes[] = { "C", "CS", "CS1", "CS2", "SE", "SE2", "MA", "IS", "LL", "PH", "XX", "Z" };
    int n = sizeof(prefixes) / sizeof(prefixes[0]);
    bool allMatch = true;
    for (int k = 0; k < n; k++) {
        int linear = 0;
        size_t len = strlen(prefixes[k]);
        for (int i = 0; i < g_ht.soLuongMon; i++)
            if (strncmp(g_ht.dsMonHoc[i].id, prefixes[k], len) == 0) linear++;
        int expected = (linear > 10) ? 10 : linear;      // Trie gioi han 10 goi y
        TrieNode* node = searchPrefixTrie(g_ht.cayTienTo, prefixes[k]);
        int actual = (node == nullptr) ? 0 : node->idCount;
        if (actual != expected) {
            allMatch = false;
            printf("         Tien to '%s': Trie=%d, mong doi=%d\n", prefixes[k], actual, expected);
        }
    }
    CHECK(allMatch, "Ket qua Trie khop voi quet tuyen tinh (co tinh gioi han 10) cho 12 tien to");

    // 3) Gia tri cu the tu du lieu mau
    TrieNode* cs = searchPrefixTrie(g_ht.cayTienTo, "CS");
    CHECK(cs != nullptr && cs->idCount == 10, "'CS' (20 mon trong CSV) -> Trie tra ve toi da 10 goi y");
    TrieNode* cs10 = searchPrefixTrie(g_ht.cayTienTo, "CS10");
    CHECK(cs10 != nullptr && nodeHas(cs10, "CS101") && nodeHas(cs10, "CS102"), "'CS10' -> co CS101 va CS102");
}

static void test_goiymon_function() {
    SECTION("12. heThong_GoiYMon (ham goi y ma menu dang dung)");
    setupHeThong(&g_ht);
    addCourseToSystem(&g_ht, makeCourse("CS101", "Nhap mon Lap trinh", 3, 50, 45, "x"));
    addCourseToSystem(&g_ht, makeCourse("CS102", "Ky thuat lap trinh", 3, 50, 40, "x"));
    addCourseToSystem(&g_ht, makeCourse("SE201", "Lap trinh Web", 3, 40, 40, "x"));
    addCourseToSystem(&g_ht, makeCourse("MA101", "Giai tich 1", 3, 80, 10, "x"));

    beginCapture();
    heThong_GoiYMon(&g_ht, "CS");
    endCapture();
    CHECK(capturedHas("CS101") && capturedHas("Nhap mon Lap trinh"), "Tien to 'CS': hien CS101 + ten mon");
    CHECK(capturedHas("CS102") && capturedHas("Ky thuat lap trinh"), "Tien to 'CS': hien CS102 + ten mon");
    CHECK(!capturedHas("SE201") && !capturedHas("MA101"), "Tien to 'CS': khong hien mon SE/MA");

    beginCapture();
    heThong_GoiYMon(&g_ht, "SE2");
    endCapture();
    CHECK(capturedHas("SE201"), "Tien to 'SE2': hien SE201");
    CHECK(!capturedHas("CS101"), "Tien to 'SE2': khong hien CS101");

    beginCapture();
    heThong_GoiYMon(&g_ht, "ZZ");
    endCapture();
    CHECK(capturedHas("Khong co mon hoc nao phu hop"), "Tien to khong khop -> 'Khong co mon hoc nao phu hop'");

    beginCapture();
    heThong_GoiYMon(&g_ht, "CS101");
    endCapture();
    CHECK(capturedHas("CS101") && !capturedHas("CS102"), "Go day du ma 'CS101' -> chi hien CS101");

    // Quet tren du lieu that
    setupHeThong(&g_ht);
    docFileMonHoc(&g_ht, "data/courses.csv");
    beginCapture();
    heThong_GoiYMon(&g_ht, "SE");
    endCapture();
    CHECK(capturedHas("SE201") && capturedHas("SE202") && capturedHas("SE203"), "Du lieu that: 'SE' hien SE201, SE202, SE203");
    CHECK(!capturedHas("CS101"), "Du lieu that: 'SE' khong lan CS101");

    // Ghi chu thiet ke
    NOTE(false, "heThong_GoiYMon quet tuyen tinh dsMonHoc va KHONG dung cayTienTo (Trie) -> Trie chua duoc dung trong menu");
}

int main() {
    TEST_BEGIN("CHUC NANG 6 - GOI Y THEO TIEN TO (TK_TienTo / Trie)");
    test_create_node();
    test_insert_search_basic();
    test_not_found();
    test_empty_prefix();
    test_shared_prefix();
    test_order();
    test_limit_10();
    test_duplicate_insert();
    test_independent_tries();
    test_special_chars();
    test_real_data();
    test_goiymon_function();
    return testSummary();
}
