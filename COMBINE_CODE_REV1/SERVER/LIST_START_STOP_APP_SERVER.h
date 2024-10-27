#ifndef LIST_START_STOP_APP_SERVER_H
#define LIST_START_STOP_APP_SERVER_H

#include <winsock2.h>
#include <string> // ?? s? d?ng std::string

// Hàm ?? li?t kê các ?ng d?ng ?ang ch?y
void listRunningApplications(SOCKET client_socket);

// Hàm ?? li?t kê các ?ng d?ng không ch?y
void listNotRunningApplications(SOCKET client_socket);

// Hàm ?? kh?i ??ng m?t ?ng d?ng
void startApplication(SOCKET client_socket, const std::string& appName);

// Hàm ?? d?ng m?t ?ng d?ng theo PID
void stopApplication(SOCKET client_socket, int pid);

#endif // LIST_START_STOP_APP_SERVER_H
