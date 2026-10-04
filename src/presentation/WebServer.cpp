#include "httplib.h"
#include "../core/HeThong.h"
#include "../persistence/DataLoad.h"
#include "../core/dsa/DangKi.h"
#include "../core/dsa/Huy.h"
#include "../core/dsa/Undo.h"
#include <iostream>
#include <string.h>

using namespace std;

int main() {
    HeThong ht;
    initHeThong(&ht);

    initStack(&(ht.undoStack));

    docFileMonHoc(&ht, "data/courses.csv");
    docFileSinhVien(&ht, "data/students.json");

    for (int i = 0; i < ht.soLuongMon; i++) {
        insertCourseHT(&(ht.bangBam), ht.dsMonHoc[i]);
    }

    httplib::Server svr;
    svr.set_mount_point("/", "./frontend");

    // 1. API Tra cứu môn học
    svr.Get("/api/course", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id")) {
            string maHP = req.get_param_value("id");
            Course* c = searchCourseHT(&(ht.bangBam), maHP.c_str());
            if (c != nullptr) {
                string json = "{\n  \"id\": \"" + string(c->id) + "\",\n"
                    "  \"name\": \"" + string(c->name) + "\",\n"
                    "  \"credits\": " + to_string(c->credits) + ",\n"
                    "  \"maxCapacity\": " + to_string(c->maxCapacity) + ",\n"
                    "  \"currentEnrolled\": " + to_string(c->currentEnrolled) + ",\n"
                    "  \"lichHoc\": \"" + string(c->lichHoc) + "\"\n}";
                res.set_content(json, "application/json");
                return;
            }
        }
        res.set_content("{\"error\": \"Khong tim thay hoc phan!\"}", "application/json");
        });

    // 2. API Xem TKB sinh viên (Bọc an toàn chống crash)
    svr.Get("/api/student", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id")) {
            string maSV = req.get_param_value("id");
            Student* sv = nullptr;
            for (int i = 0; i < ht.soLuongSV; i++) {
                if (strcmp(ht.dsSinhVien[i].id, maSV.c_str()) == 0) {
                    sv = &(ht.dsSinhVien[i]);
                    break;
                }
            }
            if (sv != nullptr) {
                int safeRegCount = sv->regCount;
                if (safeRegCount < 0) safeRegCount = 0;
                if (safeRegCount > 50) safeRegCount = 50;

                string json = "{\n  \"id\": \"" + string(sv->id) + "\",\n"
                    "  \"name\": \"" + string(sv->name) + "\",\n"
                    "  \"regCount\": " + to_string(safeRegCount) + ",\n"
                    "  \"courses\": [";
                for (int j = 0; j < safeRegCount; j++) {
                    try {
                        json += "\"" + string(sv->registeredCourses[j]) + "\"";
                    }
                    catch (...) {
                        json += "\"Loi_Du_Lieu\"";
                    }
                    if (j < safeRegCount - 1) json += ", ";
                }
                json += "]\n}";
                res.set_content(json, "application/json");
                return;
            }
        }
        res.set_content("{\"error\": \"Khong tim thay sinh vien!\"}", "application/json");
        });

    // 3. API Gợi ý theo tiền tố (Phục vụ Menu thả xuống)
    svr.Get("/api/goiy", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("tiento")) {
            string tiento = req.get_param_value("tiento");
            string json = "{\n  \"courses\": [";
            bool first = true;
            for (int i = 0; i < ht.soLuongMon; i++) {
                if (strncmp(ht.dsMonHoc[i].id, tiento.c_str(), tiento.length()) == 0) {
                    if (!first) json += ", ";
                    json += "{\"id\": \"" + string(ht.dsMonHoc[i].id) + "\", \"name\": \"" + string(ht.dsMonHoc[i].name) + "\"}";
                    first = false;
                }
            }
            json += "]\n}";
            res.set_content(json, "application/json");
            return;
        }
        res.set_content("{\"courses\": []}", "application/json");
        });

    // 4. API Đăng ký môn
    svr.Get("/api/dangky", [&](const httplib::Request& req, httplib::Response& res) {
        string masv = req.get_param_value("masv");
        string mahp = req.get_param_value("mahp");

        heThong_DangKyMon(&ht, masv.c_str(), mahp.c_str());
        pushUndo(&(ht.undoStack), "DANG_KI", masv.c_str(), mahp.c_str());

        res.set_content("{\"message\": \"Da dang ky hoc phan " + mahp + " cho SV " + masv + "\"}", "application/json");
        });

    // 5. API Hủy môn
    svr.Get("/api/huy", [&](const httplib::Request& req, httplib::Response& res) {
        string masv = req.get_param_value("masv");
        string mahp = req.get_param_value("mahp");

        heThong_HuyMon(&ht, masv.c_str(), mahp.c_str());
        pushUndo(&(ht.undoStack), "HUY", masv.c_str(), mahp.c_str());

        res.set_content("{\"message\": \"Da huy mon hoc " + mahp + " cua SV " + masv + "\"}", "application/json");
        });

    // 6. API Undo
    svr.Get("/api/undo", [&](const httplib::Request& req, httplib::Response& res) {
        Action lastAction;

        if (popUndo(&(ht.undoStack), &lastAction)) {
            if (strcmp(lastAction.type, "DANG_KI") == 0) {
                heThong_HuyMon(&ht, lastAction.studentId, lastAction.courseId);
                string msg = "Da huy mon " + string(lastAction.courseId) + " cua SV " + string(lastAction.studentId);
                res.set_content("{\"message\": \"" + msg + "\"}", "application/json");
            }
            else if (strcmp(lastAction.type, "HUY") == 0) {
                heThong_DangKyMon(&ht, lastAction.studentId, lastAction.courseId);
                string msg = "Da dang ky lai mon " + string(lastAction.courseId) + " cho SV " + string(lastAction.studentId);
                res.set_content("{\"message\": \"" + msg + "\"}", "application/json");
            }
        }
        else {
            res.set_content("{\"message\": \"Khong co thao tac nao de hoan tac!\"}", "application/json");
        }
        });

    cout << "===================================================\n";
    cout << " WEB SERVER DA KHOI CHAY THANH CONG!\n";
    cout << " Truy cap trinh duyet tai: http://localhost:8080\n";
    cout << "===================================================\n";

    svr.listen("0.0.0.0", 8080);
    return 0;
}