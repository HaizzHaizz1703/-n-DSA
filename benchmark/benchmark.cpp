#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

// Include cac header 
#include "../src/persistence/DataLoad.h"
#include "../src/core/dsa/TraCuu.h"
#include "../src/core/dsa/DangKi.h"
#include "../src/core/dsa/Huy.h"
#include "../src/core/dsa/Undo.h"
#include "../src/core/dsa/TK_TienTo.h"
#include "../src/core/dsa/TKB.h"
#include "../src/core/HeThong.h"

using namespace std;
using namespace std::chrono;

// 1. Do thoi gian nap du lieu
void benchmarkDataLoading(HeThong* ht)
{
    auto start = high_resolution_clock::now();

    // Sua duong dan file 
    docFileMonHoc(ht, "benchmark/data/courses_large.csv");

    auto end = high_resolution_clock::now();

    // Dung microseconds de do chinh xac hon, tranh loi 0 ms
    auto dur = duration_cast<microseconds>(end - start).count();
    cout << "- Nap du lieu (courses_large.csv): " << dur / 1000.0 << " ms" << endl;
}

// 2. Do thoi gian Tra cuu chinh xac (#MC1)
void benchmarkTraCuu(TraCuu* tcTable, int numOperations)
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i) {
        searchCourseHT(tcTable, "CS101");
    }
    auto end = high_resolution_clock::now();
    auto duration_us = duration_cast<microseconds>(end - start).count();

    double avg_time = static_cast<double>(duration_us) / numOperations;

    cout << "- Tra cuu chinh xac (" << numOperations << " luot): "
        << duration_us / 1000.0 << " ms ("
        << fixed << setprecision(3) << avg_time << " us/luot)" << endl;
}

// 3. Do thoi gian Tim kiem bang tien to 
void benchmarkTienTo(TrieNode* root, int numOperations) {
    string prefixes[] = { "CS", "SE", "IS", "MA", "PH", "LL" };
    int numPrefixes = sizeof(prefixes) / sizeof(prefixes[0]);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i)
    {
        string p = prefixes[i % numPrefixes];
        searchPrefixTrie(root, p.c_str());
    }
    auto end = high_resolution_clock::now();
    auto duration_us = duration_cast<microseconds>(end - start).count();

    double avg_time = static_cast<double>(duration_us) / numOperations;

    cout << "- Tim kiem tien to (" << numOperations << " luot): "
        << duration_us / 1000.0 << " ms ("
        << fixed << setprecision(3) << avg_time << " us/luot)" << endl;
}

// 4. Do thoi gian Xu ly chuoi lenh mo phong
void benchmarkDangKy(TraCuu* tcTable, UndoStack* uStack, TrieNode* rootTrie, const string& requestsFilePath, int targetRequests)
{
    ifstream reqFile(requestsFilePath);
    if (!reqFile.is_open()) {
        cout << "- [Loi] Khong tim thay file " << requestsFilePath << "!" << endl;
        return;
    }

    PriorityQueue pq;
    initPQ(&pq);
    string line;
    int count = 0;

    auto start = high_resolution_clock::now();

    // ========================================================
    // CONG TAC CHONG SPAM MAN HINH
    // Tam thoi tat chuc nang in chu (cout) cua C++ de cac 
    // loi logic khong bi in ra lam tran man hinh
    streambuf* original_buf = cout.rdbuf();
    cout.rdbuf(NULL);
    // ========================================================

    while (count < targetRequests && getline(reqFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "REGISTER") {
            string mssv, code;
            int priority = 1;
            ss >> mssv >> code;

            Student sv;
            Course c;
            dangKyMon(&sv, &c, &pq, priority);
            pushUndo(uStack, "REGISTER", mssv.c_str(), code.c_str());
        }
        else if (cmd == "CANCEL") {
            string mssv, code;
            ss >> mssv >> code;

            Student sv;
            Course c;
            huyMon(&sv, &c);
            pushUndo(uStack, "CANCEL", mssv.c_str(), code.c_str());
        }
        else if (cmd == "UNDO") {
            Action act;
            popUndo(uStack, &act);
        }
        else if (cmd == "SEARCH") {
            string code;
            ss >> code;
            searchCourseHT(tcTable, code.c_str());
        }
        else if (cmd == "PRESET_SEARCH") {
            string prefix;
            ss >> prefix;
            searchPrefixTrie(rootTrie, prefix.c_str());
        }
        count++;
    }

    // ========================================================
    // Bat lai chuc nang in man hinh sau khi chay xong vong lap
    cout.rdbuf(original_buf);
    // ========================================================

    auto end = high_resolution_clock::now();
    auto duration_ms = duration_cast<milliseconds>(end - start).count();

    reqFile.close();

    cout << "- Xu ly tong hop tu file (" << count << " luot): " << duration_ms << " ms" << endl;
}

// 5. Do thoi gian thao tac Pop/Push Undo
void benchmarkUndo(UndoStack* uStack, int numOperations)
{
    auto start = high_resolution_clock::now();

    Action act;
    for (int i = 0; i < numOperations; ++i) {
        pushUndo(uStack, "REGISTER", "21121001", "CS101");
        popUndo(uStack, &act);
    }

    auto end = high_resolution_clock::now();
    auto duration_us = duration_cast<microseconds>(end - start).count();

    cout << "- Thao tac Undo (" << numOperations << " luot): " << duration_us / 1000.0 << " ms" << endl;
}

int main() {
    cout << "=== BENCHMARK HIEU NANG ===\n";

    // Khoi tao cac cau truc du lieu tong
    HeThong ht;
    TrieNode* rootTrie = createNode();

    TraCuu tcTable;
    initTraCuu(&tcTable);

    UndoStack uStack;
    initStack(&uStack);

    // BUOC 0: Nap Du Lieu ban dau
    benchmarkDataLoading(&ht);

    // ====================================================================
    // KICH BAN 1: EP TAI VOI 1.000 REQUESTS
    // ====================================================================
    cout << "\n[ KICH BAN 1: 1.000 HANH DONG ]\n";
    benchmarkTraCuu(&tcTable, 1000);
    benchmarkTienTo(rootTrie, 1000);
    // Sua duong dan file txt
    benchmarkDangKy(&tcTable, &uStack, rootTrie, "benchmark/data/requests.txt", 1000);
    benchmarkUndo(&uStack, 1000);

    // ====================================================================
    // KICH BAN 2: EP TAI VOI 10.000 REQUESTS
    // ====================================================================
    cout << "\n[ KICH BAN 2: 10.000 HANH DONG ]\n";
    benchmarkTraCuu(&tcTable, 10000);
    benchmarkTienTo(rootTrie, 10000);
    // Sua duong dan file txt
    benchmarkDangKy(&tcTable, &uStack, rootTrie, "benchmark/data/requests.txt", 10000);
    benchmarkUndo(&uStack, 10000);

    cout << "\n=== HOAN THANH ===\n";

    return 0;
}