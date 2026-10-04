# Đồ án Hệ thống Đăng ký Học phần & Waitlist (C++)

Đồ án Môn học Cấu trúc Dữ liệu & Giải thuật. Hệ thống được lập trình theo mô hình C-style Procedural (Không sử dụng Lập trình hướng đối tượng OOP, không dùng `std::vector` hay `std::string`).

## ⚙️ Kiến trúc Hệ thống
Dự án được chia thành 3 tầng (3-Tier Architecture):
1. **Core (DSA):** Chứa dữ liệu (`struct Course`, `struct Student`) và các cấu trúc dữ liệu tự cài đặt (Hash Table, Priority Queue, Stack, Trie).
2. **Persistence:** Đọc dữ liệu từ file văn bản thuần (CSV, JSON phẳng) thông qua con trỏ và mảng tĩnh.
3. **Presentation:** Giao tiếp với người dùng qua Command Line Interface (CLI).

## 🚀 Hướng dẫn sử dụng
* **Cách 1 (Dễ nhất):** Click đúp vào file `Chay_Menu.bat` trên Windows để tự động biên dịch và chạy.
* **Cách 2 (Visual Studio):** Mở thư mục bằng Visual Studio, hệ thống sẽ tự động nhận diện file `CMakeLists.txt` để build project.

## 📂 Dữ liệu mẫu (Mock Data)
Dữ liệu được đặt trong thư mục `data/`:
* `courses.csv`: Chứa thông tin 50 môn học.
* `students.json`: Chứa dữ liệu mô phỏng 100 sinh viên kèm lịch sử đăng ký.