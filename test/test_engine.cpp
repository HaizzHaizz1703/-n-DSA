#include "../src/core/RegistrationSystem.h"
#include <iostream>

using namespace std;

int main() {
    RegistrationSystem sys;
    initSystem(sys);

    Course c1 = { "DASA230180", "Giai thuat", 3, 40, 40 }; // Lớp đã đầy
    addCourseToSystem(sys, c1);

    cout << "Test Dang ky vao lop day (Vao waitlist):\n";
    processRegistration(sys, "25110327", "DASA230180");

    cout << "\nTest Hoan tac (Undo):\n";
    undoAction(sys);

    return 0;
}