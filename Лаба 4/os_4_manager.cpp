#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int NUM_READERS = 16;
const int NUM_WRITERS = 16;
const int NUM_PAGES = 16;
const int WORK_DURATION_MS = 120000;
const int PAGE_SIZE = 4096;

struct Page {
    HANDLE writeSemaphore;
    int pageNumber;
};

struct ProcessInfo {
    PROCESS_INFORMATION pi;
    wstring name;
};

bool startProcess(const wstring& path, ProcessInfo& procInfo) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&procInfo.pi, sizeof(procInfo.pi));

    wstring commandLine = path;

    if (!CreateProcess(NULL, const_cast<LPWSTR>(commandLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &procInfo.pi)) {
        wcerr << L"Failed to start process: " << path << endl;
        return false;
    }

    procInfo.name = path;
    return true;
}

void terminateProcess(const ProcessInfo& procInfo) {
    if (!TerminateProcess(procInfo.pi.hProcess, 0)) {
        wcerr << L"Failed to terminate process: " << procInfo.name << L" Error: " << GetLastError() << endl;
    }
    CloseHandle(procInfo.pi.hProcess);
    CloseHandle(procInfo.pi.hThread);
}

int main() {
    vector<ProcessInfo> processes;
    vector<Page> pages(NUM_PAGES);

    wstring readerPath = L"C:\\Users\\pipop\\source\\repos\\os_4_reader\\x64\\Debug\\os_4_reader.exe";
    wstring writerPath = L"C:\\Users\\pipop\\source\\repos\\os_4_writer\\x64\\Debug\\os_4_writer.exe";

    const int BUFFER_SIZE = PAGE_SIZE * NUM_PAGES;

    HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, L"Global\\SharedBuffer");
    if (fileMapping == nullptr) {
        cerr << "Failed to create file mapping. Error: " << GetLastError() << endl;
        return 1;
    }

    LPVOID buffer = MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (buffer == nullptr) {
        cerr << "Failed to map view of file. Error: " << GetLastError() << endl;
        return 1;
    }

    if (!VirtualLock(buffer, BUFFER_SIZE)) {
        cerr << "Failed to lock buffer in memory. Error: " << GetLastError() << endl;
        return 1;
    }

    HANDLE readSemaphore = CreateSemaphore(NULL, 0, 1, (L"Global\\ReadSemaphore"));


    for (int i = 0; i < NUM_PAGES; ++i) {
        pages[i].pageNumber = i;
        pages[i].writeSemaphore = CreateSemaphore(NULL, 1, 1, (L"Global\\WriteSemaphore" + to_wstring(i)).c_str());
        if (pages[i].writeSemaphore == nullptr || readSemaphore == nullptr) {
            cerr << "Failed to create semaphores for page " << i << ". Error: " << GetLastError() << endl;
            return 1;
        }
    }

    HANDLE pageNumberMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), L"Global\\PageNumber");
    if (pageNumberMapping == nullptr) {
        cerr << "Failed to create page number mapping. Error: " << GetLastError() << endl;
        return 1;
    }

    int* sharedPageNumber = (int*)MapViewOfFile(pageNumberMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    if (sharedPageNumber == nullptr) {
        cerr << "Failed to map view of page number. Error: " << GetLastError() << endl;
        return 1;
    }

    *sharedPageNumber = -1;

    for (int i = 0; i < NUM_WRITERS; ++i) {
        ProcessInfo procInfo;
        if (startProcess(writerPath, procInfo)) {
            processes.push_back(procInfo);
        }
    }

    for (int i = 0; i < NUM_READERS; ++i) {
        ProcessInfo procInfo;
        if (startProcess(readerPath, procInfo)) {
            processes.push_back(procInfo);
        }
    }
    wcout << L"Started " << NUM_READERS << L" readers and " << NUM_WRITERS << L" writers." << endl;

    Sleep(WORK_DURATION_MS);

    for (const auto& procInfo : processes) {
        terminateProcess(procInfo);
    }

    wcout << L"Terminated all processes." << endl;

    VirtualUnlock(buffer, BUFFER_SIZE);
    for (auto& page : pages) {
        CloseHandle(page.writeSemaphore);
    }
    CloseHandle(fileMapping);
    UnmapViewOfFile(buffer);
    UnmapViewOfFile(sharedPageNumber);
    CloseHandle(pageNumberMapping);
    CloseHandle(readSemaphore);

    return 0;
}
