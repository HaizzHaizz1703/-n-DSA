#include "DataLoad.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

void docFileMonHoc(HeThong* ht, const char* filename) {
    ht->soLuongMon = 0;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << endl;
        return;
    }

    char line[500];
    file.getline(line, 500); // Bỏ qua dòng tiêu đề CSV

    while (file.getline(line, 500) && ht->soLuongMon < 100) {
        int i = ht->soLuongMon;
        if (strlen(line) == 0) continue;

        // 1. Mã môn học
        char* token = strtok(line, ",");
        if (token) {
            while (*token == ' ') token++;
            strcpy(ht->dsMonHoc[i].id, token);
        }

        // 2. Tên môn học
        token = strtok(nullptr, ",");
        if (token) {
            while (*token == ' ') token++;
            strcpy(ht->dsMonHoc[i].name, token);
        }

        // 3. Số tín chỉ
        token = strtok(nullptr, ",");
        if (token) ht->dsMonHoc[i].credits = atoi(token);

        // 4. Sĩ số tối đa (Max Capacity)
        token = strtok(nullptr, ",");
        if (token) ht->dsMonHoc[i].maxCapacity = atoi(token);

        // 5. Sĩ số hiện tại (Current Enrolled)
        token = strtok(nullptr, ",");
        if (token) ht->dsMonHoc[i].currentEnrolled = atoi(token);

        // 6. Lịch học
        token = strtok(nullptr, "\r\n");
        if (token) {
            while (*token == ' ') token++;
            strcpy(ht->dsMonHoc[i].lichHoc, token);
        }

        ht->soLuongMon++;
    }
    file.close();
}

void docFileSinhVien(HeThong* ht, const char* filename) {
    ht->soLuongSV = 0;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << endl;
        return;
    }

    char line[1000];
    char studentBlock[2048] = "";
    bool readingStudent = false;

    while (file.getline(line, 1000)) {
        // Phát hiện bắt đầu một sinh viên mới
        if (strstr(line, "{") != nullptr) {
            studentBlock[0] = '\0';
            readingStudent = true;
        }

        if (readingStudent) {
            strcat(studentBlock, line);
            strcat(studentBlock, "\n");
        }

        // Phát hiện kết thúc một sinh viên (dấu })
        if (strstr(line, "}") != nullptr && readingStudent) {
            readingStudent = false;

            if (ht->soLuongSV < 200) {
                int n = ht->soLuongSV;
                ht->dsSinhVien[n].id[0] = '\0';
                ht->dsSinhVien[n].name[0] = '\0';
                ht->dsSinhVien[n].maxCourses = 5;
                ht->dsSinhVien[n].regCount = 0;

                // 1. Đọc ID sinh viên trong khối block
                char* ptr = strstr(studentBlock, "\"id\"");
                if (ptr != nullptr) {
                    ptr = strchr(ptr, ':');
                    if (ptr != nullptr) {
                        ptr = strchr(ptr, '"');
                        if (ptr != nullptr) {
                            ptr++;
                            int i = 0;
                            while (*ptr != '"' && *ptr != '\0') {
                                ht->dsSinhVien[n].id[i++] = *ptr++;
                            }
                            ht->dsSinhVien[n].id[i] = '\0';
                        }
                    }
                }

                if (strlen(ht->dsSinhVien[n].id) > 0) {
                    // 2. Đọc Tên sinh viên
                    ptr = strstr(studentBlock, "\"name\"");
                    if (ptr != nullptr) {
                        ptr = strchr(ptr, ':');
                        if (ptr != nullptr) {
                            ptr = strchr(ptr, '"');
                            if (ptr != nullptr) {
                                ptr++;
                                int i = 0;
                                while (*ptr != '"' && *ptr != '\0') {
                                    ht->dsSinhVien[n].name[i++] = *ptr++;
                                }
                                ht->dsSinhVien[n].name[i] = '\0';
                            }
                        }
                    }

                    // 3. Đọc MaxCourses
                    ptr = strstr(studentBlock, "\"maxCourses\"");
                    if (ptr != nullptr) {
                        ptr = strchr(ptr, ':');
                        if (ptr != nullptr) {
                            ht->dsSinhVien[n].maxCourses = atoi(ptr + 1);
                        }
                    }

                    // 4. Đọc danh sách môn học đã đăng ký sẵn ("courses")
                    ptr = strstr(studentBlock, "\"courses\"");
                    if (ptr != nullptr) {
                        ptr = strchr(ptr, ':');
                        if (ptr != nullptr) {
                            ptr = strchr(ptr, '"');
                            if (ptr != nullptr) {
                                ptr++;
                                char dsMonHoc[200];
                                int i = 0;
                                while (*ptr != '"' && *ptr != '\0') {
                                    dsMonHoc[i++] = *ptr++;
                                }
                                dsMonHoc[i] = '\0';

                                char* mon = strtok(dsMonHoc, ",");
                                int count = 0;
                                while (mon != nullptr && count < 20) {
                                    while (*mon == ' ') mon++; // Xóa khoảng trắng đầu
                                    int len = strlen(mon);
                                    while (len > 0 && mon[len - 1] == ' ') { // Xóa khoảng trắng cuối
                                        mon[len - 1] = '\0';
                                        len--;
                                    }
                                    if (strlen(mon) > 0) {
                                        strcpy(ht->dsSinhVien[n].registeredCourses[count], mon);
                                        count++;
                                    }
                                    mon = strtok(nullptr, ",");
                                }
                                ht->dsSinhVien[n].regCount = count;
                            }
                        }
                    }

                    ht->soLuongSV++;
                }
            }
        }
    }
    file.close();
}