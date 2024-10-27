#include "LIST_START_STOP_APP_SERVER.h"
#include <windows.h>    // Thư viện chính cho Windows API
#include <tlhelp32.h>   // Thư viện để làm việc với tiến trình (Process)
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <string>       // Thư viện cho std::string và std::wstring
#include <fstream>      // Thư viện để kiểm tra sự tồn tại của file
#include <set>         // Thư viện cho std::set

const std::set<std::string> allowedApplications = {
    "notepad.exe",       // Ví dụ ứng dụng
    "calc.exe",         // Ví dụ ứng dụng
    // Thêm các ứng dụng khác vào đây
};

// Hàm để kiểm tra xem ứng dụng có trong danh sách cho phép không
bool isApplicationAllowed(const std::string& appName) {
    return allowedApplications.find(appName) != allowedApplications.end();
}

// Hàm để kiểm tra xem ứng dụng có tồn tại không
bool applicationExists(const std::string& appName) {
    std::wstring wAppName(appName.begin(), appName.end());
    std::ifstream file(wAppName);
    return file.good();
}

void listRunningApplications(SOCKET client_socket) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        const char errorMsg[] = "Failed to create process snapshot.\n";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        return;
    }

    if (Process32First(snapshot, &entry)) {
        do {
            char processInfo[256];
            snprintf(processInfo, sizeof(processInfo), "PID: %d - %s\n", entry.th32ProcessID, entry.szExeFile);
            send(client_socket, processInfo, strlen(processInfo), 0);
        } while (Process32Next(snapshot, &entry));
    }
    else {
        const char errorMsg[] = "Failed to retrieve process information.\n";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
    }

    CloseHandle(snapshot);
}

void listNotRunningApplications(SOCKET client_socket) {
    const char notRunningInfo[] = "Feature not fully implemented.\n";
    send(client_socket, notRunningInfo, strlen(notRunningInfo), 0);
}

void startApplication(SOCKET client_socket, const std::string& appName) {
    if (!isApplicationAllowed(appName)) {
        const char errorMsg[] = "Application is not allowed to start.\n";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        return;
    }

    if (!applicationExists(appName)) {
        const char errorMsg[] = "Application does not exist.\n";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        return;
    }

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring wAppName(appName.begin(), appName.end());

    if (CreateProcessW(NULL, &wAppName[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        char successMsg[256];
        snprintf(successMsg, sizeof(successMsg), "Started application: %s with PID %d\n", appName.c_str(), pi.dwProcessId);
        send(client_socket, successMsg, strlen(successMsg), 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        const char errorMsg[] = "Failed to start application.\n";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
    }
}

void stopApplication(SOCKET client_socket, int pid) {
    HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    
    if (!process) {
        const char errorMsg[] = "Failed to stop application. Application may not be running.\n";
        send(client_socket, errorMsg, strlen(errorMsg), 0);
        return;
    }

    TerminateProcess(process, 0);
    CloseHandle(process);
    char successMsg[256];
    snprintf(successMsg, sizeof(successMsg), "Stopped application with PID %d\n", pid);
    send(client_socket, successMsg, strlen(successMsg), 0);
}