void deleteFile(const char* filePath) {
    char command[256];
    snprintf(command, sizeof(command), "rm %s", filePath);
    system(command);  // Xóa file (Linux)
}

int main() {
    deleteFile("/path/to/file.txt");
    copyFile("/path/to/source.txt", "/path/to/destination.txt");
    return 0;
}
