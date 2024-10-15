#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

// Function declarations
void listApplications(int client_socket);
void startApplication(int client_socket, char* appName);
void stopApplication(int client_socket, int pid);
void deleteFile(int client_socket, char* filePath);

// Main server logic
int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept connection from client
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(client_socket, buffer, sizeof(buffer));

        // Handle client requests
        if (strncmp(buffer, "LIST", 4) == 0) {
            listApplications(client_socket);
        } else if (strncmp(buffer, "START", 5) == 0) {
            char appName[256];
            sscanf(buffer + 6, "%s", appName);
            startApplication(client_socket, appName);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            int pid;
            sscanf(buffer + 5, "%d", &pid);
            stopApplication(client_socket, pid);
        } else if (strncmp(buffer, "DELETE", 6) == 0) {
            char filePath[256];
            sscanf(buffer + 7, "%s", filePath);
            deleteFile(client_socket, filePath);
        } else if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Client disconnected.\n");
            break;
        }
    }

    close(client_socket);
    close(server_fd);
    return 0;
}

// Function implementations
void listApplications(int client_socket) {
    system("ps aux > temp.txt");
    FILE* fp = fopen("temp.txt", "r");
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        send(client_socket, line, strlen(line), 0);
    }
    fclose(fp);
}

void startApplication(int client_socket, char* appName) {
    char command[256];
    snprintf(command, sizeof(command), "%s &", appName);
    system(command);
    char message[] = "Application started successfully.\n";
    send(client_socket, message, strlen(message), 0);
}

void stopApplication(int client_socket, int pid) {
    char command[64];
    snprintf(command, sizeof(command), "kill %d", pid);
    system(command);
    char message[] = "Application stopped successfully.\n";
    send(client_socket, message, strlen(message), 0);
}

void deleteFile(int client_socket, char* filePath) {
    char command[256];
    snprintf(command, sizeof(command), "rm %s", filePath);
    system(command);
    char message[] = "File deleted successfully.\n";
    send(client_socket, message, strlen(message), 0);
}
