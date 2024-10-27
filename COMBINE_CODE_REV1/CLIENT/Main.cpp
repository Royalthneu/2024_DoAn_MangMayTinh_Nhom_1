#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "LIST_START_STOP_APP_CLIENT.h"

#pragma comment(lib, "ws2_32.lib")

bool isValidIPAddress(const char* ip) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result != 0; // Kết quả khác 0 nghĩa là địa chỉ IP không hợp lệ
}

bool isValidPort(int port) {
    return port > 0 && port <= 65535; // Cổng phải trong khoảng từ 1 đến 65535
}

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_address;
    char server_ip[INET_ADDRSTRLEN];
    int port;

    while (true) {
        // Nhập địa chỉ IP từ người dùng
        printf("Enter the server IP address: ");
        scanf_s("%s", server_ip, (unsigned)_countof(server_ip)); // Cập nhật ở đây

        // Kiểm tra tính hợp lệ của địa chỉ IP
        if (!isValidIPAddress(server_ip)) {
            printf("Invalid IP address. Please try again.\n");
            continue; // Yêu cầu người dùng nhập lại địa chỉ IP
        }

        // Nhập cổng từ người dùng
        printf("Enter the server port: ");
        scanf_s("%d", &port); // Cập nhật ở đây

        // Kiểm tra tính hợp lệ của cổng
        if (!isValidPort(port)) {
            printf("Invalid port. Please enter a port between 1 and 65535.\n");
            continue; // Yêu cầu người dùng nhập lại cổng
        }

        // Khởi tạo Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("WSAStartup failed\n");
            return -1;
        }

        // Tạo socket cho client
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
            printf("Socket creation error\n");
            WSACleanup();
            return -1;
        }

        // Thiết lập địa chỉ server
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        inet_pton(AF_INET, server_ip, &server_address.sin_addr);

        // Kết nối tới server
        if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
            printf("Connection failed. Please check if the server is running and the IP and port are correct.\n");
            closesocket(client_socket);
            WSACleanup();
            continue; // Yêu cầu người dùng nhập lại địa chỉ IP và cổng
        }

        printf("Connected to the server successfully!\n");
        break; // Kết nối thành công, thoát vòng lặp
    }

    // Menu chính
    while (1) {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Application Processing\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf_s("%d", &choice); // Cập nhật ở đây

        switch (choice) {
        case 1:
            LIST_START_STOP_APP(client_socket);
            break;
        case 2:
            closesocket(client_socket);
            printf("Disconnected from server.\n");
            WSACleanup();
            return 0;
        default:
            printf("Invalid choice, try again.\n");
        }
    }
}
