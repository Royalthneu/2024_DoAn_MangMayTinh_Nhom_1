#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")

using namespace cv;
using namespace std;

// Hàm lấy ảnh từ màn hình server
Mat captureScreenMat() {
    // Lấy độ phân giải màn hình
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Tạo các DC (device context)
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);
    SelectObject(hMemoryDC, hBitmap);

    // Copy từ màn hình vào bộ nhớ
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);

    // Tạo đối tượng BITMAP để lưu ảnh
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), screenWidth, -screenHeight, 1, 32, BI_RGB };
    Mat mat(screenHeight, screenWidth, CV_8UC4);
    GetDIBits(hMemoryDC, hBitmap, 0, screenHeight, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // Giải phóng bộ nhớ
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return mat;
}

// Gửi dữ liệu khung hình qua socket
void sendFrame(SOCKET clientSocket) {
    Mat frame = captureScreenMat();  // Lấy khung hình
    vector<uchar> buf;
    imencode(".jpg", frame, buf); // Nén khung hình thành JPEG

    // Gửi kích thước của khung hình trước
    int size = buf.size();
    send(clientSocket, (char*)&size, sizeof(int), 0);

    // Gửi dữ liệu hình ảnh nén
    send(clientSocket, (char*)buf.data(), size, 0);
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Khởi tạo Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Tạo socket server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Cấu hình địa chỉ server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888);

    // Liên kết socket với địa chỉ
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 3);

    cout << "Dang cho ket noi tu client..." << endl;
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    cout << "Client da ket noi!" << endl;

    // Gửi khung hình liên tục
    while (true) {
        sendFrame(clientSocket);
        Sleep(30);  // Gửi khoảng 30 fps
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
