void copyFile(const char* src, const char* dest) {
    char command[256];
    snprintf(command, sizeof(command), "cp %s %s", src, dest);
    system(command);  // Copy file (Linux)
}

int main() {    
    copyFile("/path/to/source.txt", "/path/to/destination.txt");
    return 0;
}
