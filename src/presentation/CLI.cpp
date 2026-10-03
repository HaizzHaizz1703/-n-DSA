#include "CLI.h"
#include <iostream>

using namespace std;

void inMenu() {
    cout << "\n===== HE THONG DANG KY HOC PHAN =====" << endl;
    cout << "1. Tra cuu hoc phan (MC1)" << endl;
    cout << "2. Xem thoi khoa bieu sinh vien" << endl;
    cout << "3. Dang ky hoc phan (Waitlist)" << endl;
    cout << "4. Huy dang ky hoc phan" << endl;
    cout << "5. Goi y mon hoc theo tien to" << endl;
    cout << "6. Hoan tac buoc vua roi (Undo)" << endl;
    cout << "0. Thoat chuong trinh" << endl;
    cout << "=====================================" << endl;
    cout << "Nhap lua chon: ";
}

void chayGiaoDienMenu(HeThong* ht) {
    int chon;
    char inputSV[50], inputMon[50];

    while (true) {
        inMenu();
        cin >> chon;
        cin.ignore();

        switch (chon) {
        case 1:
            cout << "Nhap ma hoc phan can tra (VD: CS101): ";
            cin.getline(inputMon, 50);
            heThong_TraCuuMon(ht, inputMon);
            break;
        case 2:
            cout << "Nhap ma sinh vien (VD: 24110001): ";
            cin.getline(inputSV, 50);
            heThong_XemTKB(ht, inputSV);
            break;
        case 3:
            cout << "Nhap ma sinh vien: "; cin.getline(inputSV, 50);
            cout << "Nhap ma mon muon dang ky: "; cin.getline(inputMon, 50);
            heThong_DangKyMon(ht, inputSV, inputMon);
            break;
        case 4:
            cout << "Nhap ma sinh vien: "; cin.getline(inputSV, 50);
            cout << "Nhap ma mon muon huy: "; cin.getline(inputMon, 50);
            heThong_HuyMon(ht, inputSV, inputMon);
            break;
        case 5:
            cout << "Nhap tien to ma mon (VD: CS, SE, MA): ";
            cin.getline(inputMon, 50);
            heThong_GoiYMon(ht, inputMon);
            break;
        case 6:
            heThong_HoanTac(ht);
            break;
        case 0:
            cout << "Tam biet!" << endl;
            return;
        default:
            cout << "Lua chon khong hop le!" << endl;
        }
    }
}