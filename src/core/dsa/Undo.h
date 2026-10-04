#pragma once

struct Action {
    char type[20];       // "DANG_KI" hoặc "HUY"
    char studentId[20];
    char courseId[20];
};

// Stack cài đặt bằng mảng tĩnh
struct UndoStack {
    Action arr[100];
    int top;
};

void initStack(UndoStack* s);
void pushUndo(UndoStack* s, const char* type, const char* svId, const char* cId);
bool popUndo(UndoStack* s, Action* outAction);