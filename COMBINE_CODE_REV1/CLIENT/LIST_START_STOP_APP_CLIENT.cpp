#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "LIST_START_STOP_APP_CLIENT.h"

void LIST_START_STOP_APP(SOCKET client_socket) {
    char buffer[1024] = { 0 };
    char command[256];

    while (1) {
        printf("\n--- APPLICATION PROCESSING ---\n");
        printf("1. List Applications Running\n");
        printf("2. Stop Application by PID\n");
        printf("3. List Applications Not Running\n");
        printf("4. Start Application\n");
        printf("5. Go Back to Main Menu\n");
        printf("Enter your choice: ");
        int choice;
        scanf_s("%d", &choice);

        switch (choice) {
        case 1: // Liệt kê các ứng dụng đang chạy
            strcpy_s(command, "LIST_RUNNING");
            send(client_socket, command, strlen(command), 0);
            printf("Applications running on the server:\n");
            while (recv(client_socket, buffer, 1024, 0) > 0) {
                printf("%s", buffer);
                memset(buffer, 0, sizeof(buffer));
                if (buffer[0] == '\0') break;
            }
            break;
        case 2: // Dừng ứng dụng bằng PID
            printf("Enter PID to stop: ");
            int pid;
            scanf_s("%d", &pid);
            snprintf(buffer, sizeof(buffer), "STOP %d", pid);
            send(client_socket, buffer, strlen(buffer), 0);
            recv(client_socket, buffer, 1024, 0);
            printf("%s", buffer);
            break;
        case 3: // Liệt kê các ứng dụng không chạy
            strcpy_s(command, "LIST_NOT_RUNNING");
            send(client_socket, command, strlen(command), 0);
            while (recv(client_socket, buffer, 1024, 0) > 0) {
                printf("%s", buffer);
                memset(buffer, 0, sizeof(buffer));
                if (buffer[0] == '\0') break;
            }
            break;
        case 4: // Khởi động ứng dụng
            printf("Enter application name to start: ");
            scanf_s("%s", command);
            snprintf(buffer, sizeof(buffer), "START %s", command);
            send(client_socket, buffer, strlen(buffer), 0);
            recv(client_socket, buffer, 1024, 0);
            printf("%s", buffer);
            break;
        case 5: // Quay lại menu chính
            return;
        default:
            printf("Invalid choice, try again.\n");
        }
    }
}
