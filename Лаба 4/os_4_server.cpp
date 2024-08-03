#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

HANDLE hPipe, hEvent;
OVERLAPPED overlapped = { 0 };
char buffer[1024];

void createPipe() {
    hPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\my_pipe",
        PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, // режим открытия
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, // режим канала
        PIPE_UNLIMITED_INSTANCES, // максимальное количество экземпляров
        1024,                   // размер выходного буфера
        1024,                   // размер входного буфера
        0,                      // тайм-аут 
        NULL);                  // атрибуты безопасности

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка создания канала: " << GetLastError() << endl;
        return;
    }

    // Создание события
    hEvent = CreateEvent(
        NULL,   // атрибуты безопасности
        TRUE,   // ручной сброс (manual reset)
        FALSE,  // начальное состояние - не сигнальное (nonsignaled)
        NULL);  // имя события

    if (hEvent == NULL) {
        cerr << "CreateEvent failed, GLE=" << GetLastError() << endl;
        return;
    }
}

void connectPipe() {
    // Подключение к существующему именованному каналу
    hPipe = CreateFile(
        L"\\\\.\\pipe\\my_pipe",
        GENERIC_WRITE,          // режим доступа
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

void disconnectPipe() {
    if (!DisconnectNamedPipe(hPipe)) {
        cerr << "Ошибка отключения: " << GetLastError() << endl;
    }
}

void writeData() {
    cout << "Введите данные для записи: ";
    string data;
    getline(cin, data);

    DWORD bytesWritten;
    overlapped.hEvent = hEvent;

    cout << "Записано: " << data << endl;

    // Попытка записать данные в канал асинхронно
    BOOL result = WriteFile(
        hPipe,
        data.c_str(),
        data.length(),
        &bytesWritten,
        &overlapped);

    if (!result && GetLastError() == ERROR_IO_PENDING) {
        // Если операция асинхронная, ожидаем, пока событие станет сигнальным
        WaitForSingleObject(hEvent, INFINITE);
        // После завершения асинхронной операции состояние события станет сигнальным
    }

}

void menu() {
    int choice;
    do {
        cout << "\nВыберите действие сервера: \n";
        cout << "1. Создать именованный канал\n";
        cout << "2. Установить соединение\n";
        cout << "3. Записать данные\n";
        cout << "4. Разорвать соединение\n";
        cout << "5. Завершить работу\n";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            createPipe();
            break;
        case 2:
            connectPipe();
            break;
        case 3:
            writeData();
            break;
        case 4:
            disconnectPipe();
            break;
        case 5:
            CloseHandle(hPipe);
            CloseHandle(hEvent);
            break;
        default:
            cout << "Недопустимое значения выбора\n";
        }
    } while (choice != 5);
}

int main() {
    setlocale(LC_ALL, "Russian");
    menu();
    return 0;
}
