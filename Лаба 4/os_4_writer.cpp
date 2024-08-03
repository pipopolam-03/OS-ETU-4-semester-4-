#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <ctime>
#include <random>
#include <thread>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#pragma comment(lib, "winmm.lib")

const int PAGE_SIZE = 4096;
const int NUM_PAGES = 16;
const int BUFFER_SIZE = PAGE_SIZE * NUM_PAGES;

int rand_delay() {
    static default_random_engine engine(static_cast<unsigned>(time(0)));
    static uniform_int_distribution<int> distribution(500, 1500);
    return distribution(engine);
}

void log(const string& filename, const string& message) {
    ofstream logFile(filename, ios::app);
    if (logFile.is_open()) {
        DWORD currentTime = timeGetTime();
        logFile << currentTime << " " << message << endl;
        logFile.close();
    }
}

string getLogFilename(DWORD processId) {
    string logFilename = "writer_log_" + to_string(processId) + ".txt";
    return logFilename;
}

int main(int argc, char* argv[]) {
    DWORD processId = GetCurrentProcessId();
    string logFilename = getLogFilename(processId);

    HANDLE fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Global\\SharedBuffer");
    if (fileMapping == nullptr) {
        cerr << "Failed to open file mapping. Error: " << GetLastError() << endl;
        return 1;
    }

    LPVOID buffer = MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (buffer == nullptr) {
        cerr << "Failed to map view of file. Error: " << GetLastError() << endl;
        return 1;
    }

    HANDLE pageNumberMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Global\\PageNumber");
    if (pageNumberMapping == nullptr) {
        cerr << "Failed to open page number mapping. Error: " << GetLastError() << endl;
        return 1;
    }

    int* sharedPageNumber = (int*)MapViewOfFile(pageNumberMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    if (sharedPageNumber == nullptr) {
        cerr << "Failed to map view of page number. Error: " << GetLastError() << endl;
        return 1;
    }

    HANDLE writeSemaphore;
    HANDLE readSemaphore;

    int cnt = 2;

    while (true) {
        srand(processId * cnt);

        log(logFilename, "Writer " + to_string(processId) + " has_started");

        int page = rand() % NUM_PAGES;

        writeSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (L"Global\\WriteSemaphore" + to_wstring(page)).c_str());
        readSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (L"Global\\ReadSemaphore"));

        log(logFilename, "Writer " + to_string(processId) + " is_waiting_for_write_semaphore_of_page " + to_string(page));
        WaitForSingleObject(writeSemaphore, INFINITE);

        log(logFilename, "Writer " + to_string(processId) + " is_writing_to_page " + to_string(page));
        this_thread::sleep_for(chrono::milliseconds(rand_delay()));

        log(logFilename, "Writer " + to_string(processId) + " has_written_to_page " + to_string(page));

        *sharedPageNumber = page;

        log(logFilename, "Writer " + to_string(processId) + " has_released_read_semaphore");
        ReleaseSemaphore(readSemaphore, 1, NULL);

        this_thread::sleep_for(chrono::milliseconds(rand_delay()));

        cnt++;
    }

    UnmapViewOfFile(buffer);
    UnmapViewOfFile(sharedPageNumber);
    CloseHandle(fileMapping);
    CloseHandle(pageNumberMapping);
    CloseHandle(writeSemaphore);
    CloseHandle(readSemaphore);

    return 0;
}
