void viewServerScreen() {
    system("x11vnc -display :0");  // Dùng VNC để xem màn hình (Linux)
}

int main() {
    viewServerScreen();
    return 0;
}
