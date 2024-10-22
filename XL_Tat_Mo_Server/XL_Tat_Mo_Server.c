#include <iostream>
#include <cstdlib>   // Thư viện cho system() để gọi lệnh hệ thống
#include <winsock2.h> // Thư viện cho socket (Chỉ dành cho Windows)

#pragma comment(lib, "ws2_32.lib") // Thư viện socket cho Windows

using namespace std;

void shutdownServer() {
    // Thực hiện lệnh tắt máy (shutdown)
    system("shutdown /s /f /t 0");
}

void resetServer() {
    // Thực hiện lệnh khởi động lại máy (reset)
    system("shutdown /r /f /t 0");
}
int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Khởi tạo Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Khong the khoi tao Winsock!" << endl;
        return 1;
    }

    // Tạo socket server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Khong the tao socket!" << endl;
        WSACleanup();
        return 1;
    }

    // Cấu hình địa chỉ server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888); // Sử dụng cổng 8888

    // Liên kết socket với địa chỉ
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind that bai!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Nghe kết nối từ client
    listen(serverSocket, 3);

    cout << "Dang cho ket noi tu client..." << endl;

    // Chấp nhận kết nối từ client
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Khong the chap nhan ket noi!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Da ket noi voi client." << endl;

    char buffer[1024];
    int recvSize;

    while ((recvSize = recv(clientSocket, buffer, 1024, 0)) != SOCKET_ERROR) {
        buffer[recvSize] = '\0'; // Kết thúc chuỗi nhận được
        cout << "Lenh nhan duoc: " << buffer << endl;

        if (strcmp(buffer, "shutdown") == 0) {
            shutdownServer();
        } else if (strcmp(buffer, "reset") == 0) {
            resetServer();
        } else if (strcmp(buffer, "screenshot") == 0) {
            sendScreenshot(clientSocket);
        } else {
            string errorMsg = "Lenh khong hop le!";
            send(clientSocket, errorMsg.c_str(), errorMsg.length(), 0);
        }
    }

    // Đóng kết nối và giải phóng Winsock
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
