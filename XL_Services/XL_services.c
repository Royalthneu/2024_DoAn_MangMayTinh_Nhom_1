void listServices() {
    system("service --status-all");  // Liệt kê tất cả các services (Linux)
}

void startService(const char* serviceName) {
    char command[256];
    snprintf(command, sizeof(command), "service %s start", serviceName);
    system(command);  // Bắt đầu một service (Linux)
}

void stopService(const char* serviceName) {
    char command[256];
    snprintf(command, sizeof(command), "service %s stop", serviceName);
    system(command);  // Dừng một service (Linux)
}

int main() {
    listServices();
    startService("nginx"); // Ví dụ khởi động service nginx
    stopService("nginx");  // Ví dụ dừng service nginx
    return 0;
}
