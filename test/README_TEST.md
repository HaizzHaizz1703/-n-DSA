# Bo test cho 6 chuc nang trong `src/core/dsa/`

Chay bang cach **double-click** file `.bat` (can cai san `g++`, giong `Chay_Menu.bat`).
Co the chay tu bat ky dau, file se tu chuyen ve thu muc goc du an.

| File bat | Chuc nang | Module | Cau truc du lieu |
|---|---|---|---|
| `Test_1_TraCuu.bat`  | Tra cuu hoc phan      | `TraCuu.cpp`    | Hash Table (Linear Probing) |
| `Test_2_TKB.bat`     | Xem thoi khoa bieu    | `TKB.cpp`       | (dung Hash Table de tra cuu) |
| `Test_3_DangKi.bat`  | Dang ky + Waitlist    | `DangKi.cpp`    | Priority Queue (mang) |
| `Test_4_Huy.bat`     | Huy dang ky           | `Huy.cpp`       | Mang + don phan tu |
| `Test_5_Undo.bat`    | Hoan tac              | `Undo.cpp`      | Stack (mang) |
| `Test_6_TienTo.bat`  | Goi y theo tien to    | `TK_TienTo.cpp` | Trie |
| `Test_TatCa.bat`     | Chay ca 6 va tong ket |                 |      |

## Cach doc ket qua
- `[PASS]` dat | `[FAIL]` khong dat (code goc co loi) | `[NOTE]` han che da biet, khong tinh la loi.
- Moi test thoat voi ma 0 neu khong co FAIL.
- File `.exe` sinh ra nam trong `test/bin/` (da nam trong `.gitignore` nho `*.exe`).

## Cau truc
- `test_helper.h`  : khung test dung chung (CHECK, bat output cout, ham tao du lieu mau).
- `test_*.cpp`     : 6 file test, moi file gom test don vi + test tich hop voi `data/*.csv|json`.

## Luu y
Cac file test cu (`test_engine.cpp`, `test_hash_table.cpp`, `test_priority_queue.cpp`, `test_trie.cpp`)
da bi thay the vi chung include cac header khong con ton tai (`HashTable.h`, `Trie.h`, `RegistrationSystem.h`...).
