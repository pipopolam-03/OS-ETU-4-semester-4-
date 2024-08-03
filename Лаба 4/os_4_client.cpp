#include <windows.h>
#include <iostream>

using namespace std;

HANDLE hPipe;
OVERLAPPED overlapped = { 0 };
char buffer[1024];

// Завершенная процедура для асинхронного чтения
VOID CALLBACK readCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    if (dwErrorCode == 0) {
        cout << "Прочитанные данные: " << buffer << endl;
    }
    else {
        cerr << "Ошибка чтения: " << dwErrorCode << endl;
    }
}

void connectPipe() {
    // Подключение к существующему именованному каналу
    hPipe = CreateFile(
        L"\\\\.\\pipe\\my_pipe",
        GENERIC_READ,          // режим доступа
        0,                     // режим совместного доступа
        NULL,                  // атрибуты безопасности
        OPEN_EXISTING,         // как открыть
        FILE_FLAG_OVERLAPPED,  // атрибуты
        NULL);                 // шаблон файла

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка подключения: " << GetLastError() << endl;
        return;
    }
}

void readData() {
    DWORD bytesRead;

    // Создание события для асинхронной операции
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (overlapped.hEvent == NULL) {
        cerr << "CreateEvent failed: " << GetLastError() << endl;
        return;
    }

    // Асинхронное чтение данных из канала
    BOOL result = ReadFileEx(
        hPipe,
        buffer,
        sizeof(buffer),
        &overlapped,
        readCompletionRoutine);

    if (!result) {
        cerr << "Ошибка чтения : " << GetLastError() << endl;
    }

    // Ожидание завершения асинхронной операции
    SleepEx(INFINITE, TRUE);
}

void menu() {
    int choice;
    do {
        cout << "\nВыберите действие клиента: \n";
        cout << "1. Установить соединение\n";
        cout << "2. Прочитать данные\n";
        cout << "3. Завершить работу\n";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            connectPipe();
            break;
        case 2:
            readData();
            break;
        case 3:
            // Закрытие дескриптора канала
            CloseHandle(hPipe);
            break;
        default:
            cout << "Недопустимое значение выбора\n";
        }
    } while (choice != 3);
}

int main() {
    setlocale(LC_ALL, "Russian");
    menu();
    return 0;
}
