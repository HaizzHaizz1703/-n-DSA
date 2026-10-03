#include "HeThong.h"
#include <iostream>
#include <cstring>

using namespace std;

void initHeThong(HeThong* ht) {
    ht->soLuongSV = 0;
    ht->soLuongMon = 0;

    initTraCuu(&(ht->bangBam));
    initPQ(&(ht->waitlist));
    initStack(&(ht->lichSu));
    ht->cayTienTo = createNode();
}

Course* heThong_TraCuuMon(HeThong* ht, const char* maHP) {
    Course* kq = searchCourseHT(&(ht->bangBam), maHP);
    if (kq != nullptr) {
        // Thêm đoạn in kết quả này ra màn hình
        cout << "\n--- THONG TIN HOC PHAN ---" << endl;
        cout << "Ma HP: " << kq->id << endl;
        cout << "Ten HP: " << kq->name << endl;
        cout << "So tin chi: " << kq->credits << endl;
        cout << "So luong: " << kq->currentEnrolled << "/" << kq->maxCapacity << endl;
        cout << "Lich hoc: " << kq->lichHoc << endl;
        cout << "---------------------------\n";
    }
    else {
        cout << "Khong tim thay mon hoc: " << maHP << "\n";
    }
    return kq;
}

void heThong_XemTKB(HeThong* ht, const char* maSV) {
    Student* svTimThay = nullptr;
    for (int i = 0; i < ht->soLuongSV; i++) {
        if (strcmp(ht->dsSinhVien[i].id, maSV) == 0) {
            svTimThay = &(ht->dsSinhVien[i]);
            break;
        }
    }
    if (svTimThay != nullptr) {
        inThoiKhoaBieu(svTimThay, &(ht->bangBam));
    }
    else {
        cout << "Loi: Khong tim thay sinh vien " << maSV << "\n";
    }
}

void heThong_DangKyMon(HeThong* ht, const char* maSV, const char* maHP) {
    Student* svTimThay = nullptr;
    for (int i = 0; i < ht->soLuongSV; i++) {
        if (strcmp(ht->dsSinhVien[i].id, maSV) == 0) { svTimThay = &(ht->dsSinhVien[i]); break; }
    }
    Course* monTimThay = searchCourseHT(&(ht->bangBam), maHP);

    if (svTimThay == nullptr) { cout << "Loi: Khong tim thay sinh vien!\n"; return; }
    if (monTimThay == nullptr) { cout << "Loi: Khong tim thay mon hoc!\n"; return; }

    bool thanhCong = dangKyMon(svTimThay, monTimThay, &(ht->waitlist), 1);
    if (thanhCong) {
        cout << "Dang ky thanh cong mon " << monTimThay->name << "!\n";
        pushUndo(&(ht->lichSu), "DANG_KI", maSV, maHP);
    }
}

void heThong_HuyMon(HeThong* ht, const char* maSV, const char* maHP) {
    Student* svTimThay = nullptr;
    for (int i = 0; i < ht->soLuongSV; i++) {
        if (strcmp(ht->dsSinhVien[i].id, maSV) == 0) { svTimThay = &(ht->dsSinhVien[i]); break; }
    }
    Course* monTimThay = searchCourseHT(&(ht->bangBam), maHP);

    if (svTimThay == nullptr || monTimThay == nullptr) return;

    if (huyMon(svTimThay, monTimThay)) {
        cout << "Huy thanh cong mon " << monTimThay->name << "!\n";
        pushUndo(&(ht->lichSu), "HUY", maSV, maHP);
    }
}

void heThong_HoanTac(HeThong* ht) {
    Action hanhDongCuoi;
    if (popUndo(&(ht->lichSu), &hanhDongCuoi)) {
        cout << "Dang hoan tac buoc: " << hanhDongCuoi.type << " mon " << hanhDongCuoi.courseId << "...\n";
        if (strcmp(hanhDongCuoi.type, "DANG_KI") == 0) {
            heThong_HuyMon(ht, hanhDongCuoi.studentId, hanhDongCuoi.courseId);
        }
        else {
            heThong_DangKyMon(ht, hanhDongCuoi.studentId, hanhDongCuoi.courseId);
        }
    }
    else {
        cout << "Khong co thao tac nao de hoan tac!\n";
    }
}

void heThong_GoiYMon(HeThong* ht, const char* tienTo) {
    cout << "Cac mon hoc bat dau bang '" << tienTo << "':\n";
    int count = 0;
    for (int i = 0; i < ht->soLuongMon; i++) {
        if (strncmp(ht->dsMonHoc[i].id, tienTo, strlen(tienTo)) == 0) {
            cout << "- " << ht->dsMonHoc[i].id << " | " << ht->dsMonHoc[i].name << "\n";
            count++;
        }
    }
    if (count == 0) cout << "(Khong co mon hoc nao phu hop)\n";
}