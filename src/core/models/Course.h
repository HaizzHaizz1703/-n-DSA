#pragma once
#include <string>

using namespace std;

struct Course {
    char id[20];
    char name[100];
    int credits;
    int maxCapacity;
    int currentEnrolled;
    char lichHoc[50];
};