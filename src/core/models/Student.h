#pragma once

struct Student {
    char id[20];
    char name[50];
    char registeredCourses[20][20]; // Tối đa 20 môn, mỗi mã 20 ký tự
    int regCount;
    int maxCourses;
};