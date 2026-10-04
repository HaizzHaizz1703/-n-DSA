#pragma once
#include "../models/Course.h"

#define HASH_SIZE 200 

struct TraCuu {
    Course table[HASH_SIZE];
    bool isOccupied[HASH_SIZE];
    int count;
};

void initTraCuu(TraCuu* tc);
int hashCourseId(const char* id);
void insertCourseHT(TraCuu* tc, Course c);
Course* searchCourseHT(TraCuu* tc, const char* id);