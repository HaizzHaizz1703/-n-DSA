#include "Undo.h"
#include <cstring>

void initStack(UndoStack* s) {
    s->top = -1;
}

void pushUndo(UndoStack* s, const char* type, const char* svId, const char* cId) {
    if (s->top >= 99) return; // Stack đầy
    s->top++;
    strcpy(s->arr[s->top].type, type);
    strcpy(s->arr[s->top].studentId, svId);
    strcpy(s->arr[s->top].courseId, cId);
}

bool popUndo(UndoStack* s, Action* outAction) {
    if (s->top == -1) return false; // Stack rỗng
    *outAction = s->arr[s->top];
    s->top--;
    return true;
}