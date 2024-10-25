#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char command[256];

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        printf("\n--- CLIENT MENU ---\n");
        printf("1. List running applications\n");
        printf("2. Start an application\n");
        printf("3. Stop an application by PID\n");
        printf("4. Delete a file\n");
        printf("5. Copy a file\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                strcpy(command, "LIST");
                send(client_socket, command, strlen(command), 0);
                printf("Applications running on the server:\n");
                while (read(client_socket, buffer, 1024)) {
                    printf("%s", buffer);
                    memset(buffer, 0, sizeof(buffer));
                    if (buffer[0] == '\0') break;
                }
                break;
            case 2:
                printf("Enter application name to start: ");
                scanf("%s", command);
                snprintf(buffer, sizeof(buffer), "START %s", command);
                send(client_socket, buffer, strlen(buffer), 0);
                read(client_socket, buffer, 1024);
                printf("%s", buffer);
                break;
            case 3:
                printf("Enter PID to stop: ");
                int pid;
                scanf("%d", &pid);
                snprintf(buffer, sizeof(buffer), "STOP %d", pid);
                send(client_socket, buffer, strlen(buffer), 0);
                read(client_socket, buffer, 1024);
                printf("%s", buffer);
                break;
            case 4:
                printf("Enter file path to delete: ");
                scanf("%s", command);
                snprintf(buffer, sizeof(buffer), "DELETE %s", command);
                send(client_socket, buffer, strlen(buffer), 0);
                read(client_socket, buffer, 1024);
                printf("%s", buffer);
                break;
            case 5:
                printf("Enter source file path to copy: ");
                char sourcePath[256];
                scanf("%s", sourcePath);
                printf("Enter destination path: ");
                char destinationPath[256];
                scanf("%s", destinationPath);
                snprintf(buffer, sizeof(buffer), "COPY %s %s", sourcePath, destinationPath);
                send(client_socket, buffer, strlen(buffer), 0);
                read(client_socket, buffer, 1024);
                printf("%s", buffer);
                break;
            case 6:
                strcpy(command, "EXIT");
                send(client_socket, command, strlen(command), 0);
                close(client_socket);
                printf("Disconnected from server.\n");
                return 0;
            default:
                printf("Invalid choice, try again.\n");
        }
    }

    return 0;
}
