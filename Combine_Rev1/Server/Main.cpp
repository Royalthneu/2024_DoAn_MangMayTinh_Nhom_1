// Server.cpp

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "LIST_START_STOP_APP_SERVER.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")
#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[1024] = { 0 };

    // Hỏi người dùng xác nhận mở cổng 8080
    std::string response;
    printf("Do you want to open port 8080? (y/n): ");
    std::getline(std::cin, response);
    if (response != "y" && response != "Y") {
        printf("Port 8080 will not be opened. Exiting program.\n");
        return 0;
    }

    // Khởi tạo Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return -1;
    }

    // Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Khởi tạo địa chỉ IP
    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    struct addrinfo hints, * res;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(hostname, nullptr, &hints, &res);
    if (result != 0) {
        printf("getaddrinfo failed: %d\n", result);
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    // Tìm địa chỉ IP hợp lệ
    char server_ip[INET_ADDRSTRLEN] = { 0 }; // Đổi lại kích thước mảng
    for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            struct sockaddr_in* addr = (struct sockaddr_in*)p->ai_addr;
            inet_ntop(AF_INET, &addr->sin_addr, server_ip, sizeof(server_ip));
            break; // Chọn địa chỉ IP đầu tiên hợp lệ
        }
    }

    // In địa chỉ IP thật của server
    printf("Server is listening on IP: %s, port: %d\n", server_ip, PORT);

    // Bind socket với địa chỉ IP thật
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf("Invalid address / Address not supported\n");
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    if (listen(server_socket, 3) < 0) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    // Chấp nhận kết nối từ client
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed\n");
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer)); //Xóa buffer
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        }

        buffer[bytes_received] = '\0'; // Đảm bảo buffer là chuỗi kết thúc bằng null
        printf("Received command: %s\n", buffer);

        if (strncmp(buffer, "LIST_RUNNING", 12) == 0) {
            listRunningApplications(client_socket);
        }
        else if (strncmp(buffer, "LIST_NOT_RUNNING", 16) == 0) {
            listNotRunningApplications(client_socket);
        }
        else if (strncmp(buffer, "START", 5) == 0) {
            char appName[256];
            scanf_s(buffer + 6, "%s", appName);
            startApplication(client_socket, appName);
        }
        else if (strncmp(buffer, "STOP", 4) == 0) {
            int pid;
            scanf_s(buffer + 5, "%d", &pid);
            stopApplication(client_socket, pid);
        }
        else if (strncmp(buffer, "GO BACK MENU LIST", 17) == 0) {
            printf("Client requested to return to menu.\n");
            break;
        }

        strcpy_s(buffer, "Command received");
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Xác nhận đóng cổng 8080 trước khi kết thúc chương trình
    printf("Do you want to close port 8080? (y/n): ");
    std::getline(std::cin, response);
    if (response == "y" || response == "Y") {
        // Đóng kết nối và làm sạch
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        printf("Port 8080 has been closed. Server stopped.\n");
    }
    else {
        printf("Server stopping without closing port 8080.\n");
    }

    return 0;
}
