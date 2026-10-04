#include <iostream>
#include "core/HeThong.h"
#include "persistence/DataLoad.h"
#include "presentation/CLI.h"
#include "core/dsa/TraCuu.h"
#include "core/dsa/TK_TienTo.h"

using namespace std;

int main() {
    cout << "Dang khoi dong he thong va nap du lieu..." << endl;

    HeThong ht;
    initHeThong(&ht);
    initStack(&(ht.undoStack));

    docFileMonHoc(&ht, "../../../data/courses.csv");
    docFileSinhVien(&ht, "../../../data/students.json");

    cout << "Da nap " << ht.soLuongMon << " mon hoc va " << ht.soLuongSV << " sinh vien!\n" << endl;

    for (int i = 0; i < ht.soLuongMon; i++) {
        insertCourseHT(&(ht.bangBam), ht.dsMonHoc[i]);
        insertTrie(ht.cayTienTo, ht.dsMonHoc[i].id, ht.dsMonHoc[i].id);
    }

    chayGiaoDienMenu(&ht);

    return 0;
}