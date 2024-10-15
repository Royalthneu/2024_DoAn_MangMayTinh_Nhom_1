#include <stdio.h>
#include <stdlib.h>

void listApplications() {
    system("ps aux");  // Liệt kê tất cả các process đang chạy (Linux)
}

void startApplication(const char* appName) {
    char command[256];
    snprintf(command, sizeof(command), "%s &", appName);
    system(command);  // Khởi chạy ứng dụng (Linux)
}

void stopApplication(int pid) {
    char command[64];
    snprintf(command, sizeof(command), "kill %d", pid);
    system(command);  // Dừng ứng dụng theo PID (Linux)
}

int main() {
    listApplications();
    startApplication("firefox"); // Ví dụ khởi động Firefox
    stopApplication(1234);       // Ví dụ dừng ứng dụng với PID 1234
    return 0;
}
