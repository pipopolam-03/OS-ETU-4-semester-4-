#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <atlbase.h>
#include <atlconv.h>
#include <string>
#include <vector>

using namespace std;

void disk_choice();
void choice();
void show_disks();
void new_folder();

FILETIME StringToFileTime(const string& timeStr) {
    FILETIME fileTime;
    SYSTEMTIME systemTime;
    sscanf_s(timeStr.c_str(), "%d:%d:%d", &systemTime.wHour, &systemTime.wMinute, &systemTime.wSecond);
    SystemTimeToFileTime(&systemTime, &fileTime);
    return fileTime;
}

wstring string_to_wstring(const string& str)
{
    wstring wStr(str.begin(), str.end());

    return wStr;
}

string trimToLastDelimiter(const string& str) {
    size_t lastDelimiterPos = str.find_last_of('\\');
    if (lastDelimiterPos != string::npos) {
        return str.substr(0, lastDelimiterPos);
    }
    return str;
}


void show_disk_info(int disk) {
    string path;
    wstring wpath;

    if (disk == 1) {
        path = "C:\\";
    } else {
        path = "D:\\";
    }

    wpath = string_to_wstring(path);
    LPWSTR volume_name = new WCHAR[MAX_PATH + 1];
    DWORD VolumeSerialNum, MaxCompomtLen, SystemFlags;
    UINT drive_type = GetDriveType(wpath.c_str());

    system("cls");
    cout << "Тип диска: ";
    if (drive_type == DRIVE_REMOVABLE)
        cout << "REMOVABLE\n";
    else if (drive_type == DRIVE_FIXED)
        cout << "FIXED\n";
    else if (drive_type == DRIVE_REMOTE)
        cout << "REMOTE\n";
    else if (drive_type == DRIVE_CDROM)
        cout << "CD-ROM\n";
    else if (drive_type == DRIVE_RAMDISK)
        cout << "RAMDISK\n";
    else
        cout << "НЕИЗВЕСТНЫЙ_ТИП\n";

    if (GetVolumeInformation(
        wpath.c_str(),
        volume_name,
        MAX_PATH + 1,
        &VolumeSerialNum,
        &MaxCompomtLen,
        &SystemFlags,
        NULL,
        0
    )) {
        wcout << L"Имя тома: " << volume_name << endl;
        wcout << L"Серийный номер тома: " << VolumeSerialNum << endl;
        wcout << L"Максимальная длина компонента: " << MaxCompomtLen << endl;
        wcout << L"Флаги файловой системы: " << endl;
        if (SystemFlags & FILE_CASE_SENSITIVE_SEARCH) {
            wcout << L"Файловая система поддерживает поиск с учетом регистра.\n";
        }
        if (SystemFlags & FILE_CASE_PRESERVED_NAMES) {
            wcout << L"Файловая система сохраняет регистр имен файлов.\n";
        }
        if (SystemFlags & FILE_UNICODE_ON_DISK) {
            wcout << L"Файловая система поддерживает имена файлов в формате Unicode на диске.\n";
        }
        if (SystemFlags & FILE_PERSISTENT_ACLS) {
            wcout << L"Файловая система поддерживает постоянные списки управления доступом (ACL).\n";
        }
        if (SystemFlags & FILE_FILE_COMPRESSION) {
            wcout << L"Файловая система поддерживает сжатие файлов.\n";
        }
        if (SystemFlags & FILE_VOLUME_QUOTAS) {
            wcout << L"Файловая система поддерживает квоты на объем тома.\n";
        }
        if (SystemFlags & FILE_SUPPORTS_SPARSE_FILES) {
            wcout << L"Файловая система поддерживает разреженные файлы.\n";
        }
        if (SystemFlags & FILE_SUPPORTS_REPARSE_POINTS) {
            wcout << L"Файловая система поддерживает точки повторной обработки.\n";
        }

    }
    else {
        cout << "Ошибка при получении информации о томе." << endl;
    }

    if (drive_type == DRIVE_FIXED) {
        ULARGE_INTEGER FreeBytesAvailable, TotalNumberOfBytes, TotalNumberOfFreeBytes;

        if (GetDiskFreeSpaceEx(
            wpath.c_str(),
            &FreeBytesAvailable,
            &TotalNumberOfBytes,
            &TotalNumberOfFreeBytes
        )) {
            cout << "\nВсего на диске: " << TotalNumberOfBytes.QuadPart << " байт (" << TotalNumberOfBytes.QuadPart / (1024 * 1024 * 1024) << " Гб)\n";
            cout << "Свободно на диске: " << TotalNumberOfFreeBytes.QuadPart << " байт (" << TotalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024) << " Гб)\n";
        }
        else {
            cout << "Ошибка при получении информации о свободном месте на диске." << endl;
        }
    }


    cout << "\n1 - Назад\n0 - Завершить программу\n";
    int flag = -1;
    cin >> flag;
    while (flag != 1 || flag != 0)
    {
        if (flag == 1)
        {
            choice();
        }
        else if (flag == 0)
        {
            exit(0);
        }
    }
}

void disk_choice()
{
    system("cls");
    int disk = -1;
    cout << "1 - Вывести информацию о диске С\n";
    cout << "2 - Вывести информацию о диске D\n";
    cout << "0 - Назад\n";

    cin >> disk;
    while (disk < -1 || disk > 3)
    {
        cout << "Ошибка. Выберите существующий диск (1 - С, 2 - D, 0 - назад)\n";
        cin >> disk;
    }

    if (disk != 0)
    {
        show_disk_info(disk);
    }
    else
    {
        choice();
    }
}

void show_disks()
{
    system("cls");
    int flag = -1;
    DWORD drives = GetLogicalDrives();
    cout << "Список доступных дисков:" << endl;
    for (int i = 0; i < 26; ++i) {
        if (drives & (1 << i)) {
            cout << static_cast<char>('A' + i) << "\n";
        }
    }
    cout << endl << "0 - Назад\n";

    flag = -1;
    cin >> flag;
    while (flag != 0)
    {
        cout << "Введите корректное число (0)\n";
        cin >> flag;
    }
    choice();


}

void new_folder()
{
    system("cls");
    string path;
    cout << "0 - назад: \n";
    cout << "Введите полный путь к новой директории: \n";
    cin >> path;
    if (path != "0")
    {
        if (CreateDirectory(string_to_wstring(path).c_str(), NULL))
        {
            cout << "Директория создана\n" << endl;
            Sleep(2000);
            choice();
        }
        else
        {
            DWORD dwError = GetLastError();
            cout << "Ошибка создания директории: " << dwError << endl;
            Sleep(2000);
            choice();

        }
    }
    else
    {
        choice();
    }

}

void delete_folder()
{
    system("cls");
    string path;
    cout << "0 - назад: \n";
    cout << "Введите полный путь к удаляемой директории: \n";
    cin >> path;
    if (path != "0")
    {
        if (RemoveDirectory(string_to_wstring(path).c_str()))
        {
            cout << "Директория удалена\n" << endl;
            Sleep(2000);
            choice();
        }
        else
        {
            DWORD dwError = GetLastError();
            cout << "Ошибка удаления директории: " << dwError << endl;
            Sleep(2000);
            choice();

        }
    }
    else
    {
        choice();
    }
}

void new_file()
{
    system("cls");
    string path;
    cout << "0 - назад: \n";
    cout << "Введите полный путь к новому файлу: \n";
    cin >> path;
    if (path != "0")
    {
        HANDLE hFile = CreateFile(
            string_to_wstring(path).c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
            cout << "Файл создан\n";
            Sleep(2000);
            choice();
        }
        else {
            DWORD dwError = GetLastError();
            cout << "Ошибка создания файла: " << dwError << endl;
            Sleep(2000);
            choice();
        }
    }
    else
    {
        choice();
    }
}



void copy_file() {
    system("cls");
    string path, name, trim_path;
    cout << "0 - назад: \n";
    if (path != "0") 
    {
        cout << "Введите полный путь к копируемому файлу: \n";
        cin >> path;
        cout << "Введите название копии с расширением: \n";
        cin >> name;
        trim_path = trimToLastDelimiter(path) + "\\" + name;
        if (!CopyFile(string_to_wstring(path).c_str(), string_to_wstring(trim_path).c_str(), true)) 
        {
            DWORD dwError = GetLastError();
            cout << "Ошибка " << dwError << endl;
            Sleep(2000);
            choice();
        }
        else
        {
            cout << "Файл успешно копировант\n";
            Sleep(2000);
            choice();
        }
    }
    else 
    {
        choice();
    }
}

void move_file() {
    system("cls");
    string source_path, destination_path;
    cout << "0 - назад: \n";
    if (source_path != "0") {
        cout << "Введите полный путь к перемещаемому файлу: \n";
        cin >> source_path;
        cout << "Введите новый полный путь с названием (можно выбрать новое) \n";
        cin >> destination_path;

        if (PathFileExists(string_to_wstring(destination_path).c_str())) {
            cout << "Файл с таким именем уже существует. Заменить? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (!MoveFileEx(string_to_wstring(source_path).c_str(), string_to_wstring(destination_path).c_str(), MOVEFILE_REPLACE_EXISTING)) {
                    DWORD dwError = GetLastError();
                    cout << "Ошибка " << dwError << endl;
                }
                else {
                    cout << "Файл успешно перемещен\n";
                }
            }
            else {
                cout << "Файл не был перемещен\n";
            }
        }
        else {
            if (!MoveFile(string_to_wstring(source_path).c_str(), string_to_wstring(destination_path).c_str())) {
                DWORD dwError = GetLastError();
                cout << "Ошибка " << dwError << endl;
                Sleep(2000);
                choice();
            }
            else {
                cout << "Файл успешно перемещен\n";
            }
        }
        Sleep(2000);
        choice();
    }
    else {
        choice();
    }
}

string path_input()
{
    string path;
    system("cls");
    cout << "0 - назад\nВведите полное имя файла: \n";
    cin >> path;
    return path;
}

void show_file_attr(string path) {
    wstring wpath;
    int flag;
    if (path != "0") {
        wpath = string_to_wstring(path);
        DWORD attributes = GetFileAttributes(wpath.c_str());
        if (attributes != INVALID_FILE_ATTRIBUTES) {
            cout << "0 - назад\nИнформация о файле: \n";
            cout << "Атрибуты файла:\n";
            if (attributes & FILE_ATTRIBUTE_ARCHIVE)
                cout << "Архивный\n";
            if (attributes & FILE_ATTRIBUTE_COMPRESSED)
                cout << "Сжатый\n";
            if (attributes & FILE_ATTRIBUTE_DIRECTORY)
                cout << "Директория\n";
            if (attributes & FILE_ATTRIBUTE_HIDDEN)
                cout << "Скрытый\n";
            if (attributes & FILE_ATTRIBUTE_NORMAL)
                cout << "Обычный\n";
            if (attributes & FILE_ATTRIBUTE_READONLY)
                cout << "Только для чтения\n";
            if (attributes & FILE_ATTRIBUTE_SYSTEM)
                cout << "Системный\n";

            HANDLE hFile = CreateFile(wpath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                BY_HANDLE_FILE_INFORMATION fileInformation;
                if (GetFileInformationByHandle(hFile, &fileInformation)) {
                    FILETIME ftCreate, ftAccess, ftWrite;
                    SYSTEMTIME stUTC, stLocal;

                    ftCreate = fileInformation.ftCreationTime;
                    ftAccess = fileInformation.ftLastAccessTime;
                    ftWrite = fileInformation.ftLastWriteTime;

                    FileTimeToSystemTime(&ftCreate, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    cout << "Время создания: " << stLocal.wYear << "/" << stLocal.wMonth << "/" << stLocal.wDay << " " << stLocal.wHour << ":" << stLocal.wMinute << ":" << stLocal.wSecond << endl;

                    FileTimeToSystemTime(&ftAccess, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    cout << "Время последнего доступа: " << stLocal.wYear << "/" << stLocal.wMonth << "/" << stLocal.wDay << " " << stLocal.wHour << ":" << stLocal.wMinute << ":" << stLocal.wSecond << endl;

                    FileTimeToSystemTime(&ftWrite, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    cout << "Время последнего изменения: " << stLocal.wYear << "/" << stLocal.wMonth << "/" << stLocal.wDay << " " << stLocal.wHour << ":" << stLocal.wMinute << ":" << stLocal.wSecond << endl;
                }
                CloseHandle(hFile);
            }
            else {
                DWORD dwError = GetLastError();
                cout << "Ошибка при открытии файла: " << dwError << endl;
                Sleep(2000);
                choice();
            }
        }
        else {
            DWORD dwError = GetLastError();
            cout << "Ошибка " << dwError << endl;
            Sleep(2000);
            choice();
        }

        cin >> flag;
        while (flag != 0)
        {
            cout << "Введите корректное значение (0): \n";
            cin >> flag;
        }
        choice();
    }

    else {
        choice();
    }
}

void edit_file_attr()
{
    system("cls");
    string path = path_input();
    wstring wpath = string_to_wstring(path);
    int flag=-1;
    cout << "Какой атрибут вы хотите добавить/убрать?\n";
    cout << "0 - Вернуться в меню\n";
    cout << "1 - Архивный\n";
    cout << "2 - Скрытый\n";
    cout << "3 - Обычный\n";
    cout << "4 - Сжатый\n";
    cout << "5 - Директория\n";
    cout << "6 - Системный\n";
    cout << "7 - Только для чтения\n";
    cout << "8 - Время создания\n";
    cout << "9 - Время последнего изменения\n";
    cout << "10 - Время последнего доступа\n";
    cin >> flag;

    DWORD fileAttributes = GetFileAttributes(wpath.c_str());

    switch (flag) {

    case 0:

        choice();

    case 1:

        if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE)
            fileAttributes &= ~FILE_ATTRIBUTE_ARCHIVE;
        else
            fileAttributes |= FILE_ATTRIBUTE_ARCHIVE;
        break;

    case 2:

        if (fileAttributes & FILE_ATTRIBUTE_HIDDEN)
            fileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
        else
            fileAttributes |= FILE_ATTRIBUTE_HIDDEN;
        break;

    case 3:

        break;

    case 4:

        if (fileAttributes & FILE_ATTRIBUTE_COMPRESSED)
            fileAttributes &= ~FILE_ATTRIBUTE_COMPRESSED;
        else
            fileAttributes |= FILE_ATTRIBUTE_COMPRESSED;
        break;

    case 5:

        cout << "Директорию изменить таким способом нельзя, выполните перемещение из соответствующего пункта\n";
        Sleep(2000);
        choice();

    case 6:

        if (fileAttributes & FILE_ATTRIBUTE_SYSTEM)
            fileAttributes &= ~FILE_ATTRIBUTE_SYSTEM;
        else
            fileAttributes |= FILE_ATTRIBUTE_SYSTEM;
        break;

    case 7:

        if (fileAttributes & FILE_ATTRIBUTE_READONLY)
            fileAttributes &= ~FILE_ATTRIBUTE_READONLY;
        else
            fileAttributes |= FILE_ATTRIBUTE_READONLY;
        break;

    case 8:
    {
        string newCreationDateStr, newCreationTimeStr;
        cout << "Введите новую дату создания файла в формате ГГГГ-MM-ДД: ";
        cin >> newCreationDateStr;
        cout << "Введите новое время создания файла в формате ЧЧ:ММ:СС: ";
        cin >> newCreationTimeStr;

        SYSTEMTIME creationSystemTime;
        sscanf_s(newCreationDateStr.c_str(), "%d-%d-%d", &creationSystemTime.wYear, &creationSystemTime.wMonth, &creationSystemTime.wDay);
        sscanf_s(newCreationTimeStr.c_str(), "%d:%d:%d", &creationSystemTime.wHour, &creationSystemTime.wMinute, &creationSystemTime.wSecond);

        FILETIME creationFileTime;
        SystemTimeToFileTime(&creationSystemTime, &creationFileTime);

        HANDLE hFile = CreateFile(
            wpath.c_str(),
            FILE_WRITE_ATTRIBUTES,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Ошибка при открытии файла для изменения времени создания." << endl;
            Sleep(2000);
            choice();
        }

        if (!SetFileTime(hFile, &creationFileTime, NULL, NULL)) {
            cout << "Ошибка при изменении времени создания файла." << endl;
            CloseHandle(hFile);
            Sleep(2000);
            choice();
        }
        else {
            cout << "Время создания файла успешно изменено." << endl;
            CloseHandle(hFile);
            Sleep(2000);
            choice();
        }
        break;
    }
    case 9:
    {
        string newWriteDateStr, newWriteTimeStr;
        cout << "Введите новую дату изменения файла в формате ГГГГ-MM-ДД: ";
        cin >> newWriteDateStr;
        cout << "Введите новое время изменения файла в формате ЧЧ:MM:СС: ";
        cin >> newWriteTimeStr;

        SYSTEMTIME writeSystemTime;
        sscanf_s(newWriteDateStr.c_str(), "%d-%d-%d", &writeSystemTime.wYear, &writeSystemTime.wMonth, &writeSystemTime.wDay);
        sscanf_s(newWriteTimeStr.c_str(), "%d:%d:%d", &writeSystemTime.wHour, &writeSystemTime.wMinute, &writeSystemTime.wSecond);

        FILETIME writeFileTime;
        SystemTimeToFileTime(&writeSystemTime, &writeFileTime);

        HANDLE hFile = CreateFile(
            wpath.c_str(),
            FILE_WRITE_ATTRIBUTES, 
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Ошибка при открытии файла для изменения времени изменения." << endl;
            Sleep(2000);
            choice();
        }

        if (!SetFileTime(hFile, NULL, NULL, &writeFileTime)) {
            cout << "Ошибка при изменении времени изменения файла." << endl;
            CloseHandle(hFile);
            Sleep(2000);
            choice();
        }
        else {
            cout << "Время изменения файла успешно изменено." << endl;
            CloseHandle(hFile);
            Sleep(2000);
            choice();
        }
        break;
    }

    case 10:
    {

        string newAccessDateStr, newAccessTimeStr;
        cout << "Введите новую дату последнего доступа к файлу в формате ГГГГ-MM-ДД: ";
        cin >> newAccessDateStr;
        cout << "Введите новое время последнего доступа к файлу в формате ЧЧ:MM:СС: ";
        cin >> newAccessTimeStr;

        SYSTEMTIME accessSystemTime;
        sscanf_s(newAccessDateStr.c_str(), "%d-%d-%d", &accessSystemTime.wYear, &accessSystemTime.wMonth, &accessSystemTime.wDay);
        sscanf_s(newAccessTimeStr.c_str(), "%d:%d:%d", &accessSystemTime.wHour, &accessSystemTime.wMinute, &accessSystemTime.wSecond);

        FILETIME accessFileTime;
        SystemTimeToFileTime(&accessSystemTime, &accessFileTime);

        HANDLE hFile = CreateFile(
            wpath.c_str(),
            FILE_WRITE_ATTRIBUTES,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Ошибка при открытии файла для изменения времени последнего доступа." << endl;
            Sleep(2000);
            choice();
        }

        if (!SetFileTime(hFile, NULL, &accessFileTime, NULL)) {
            cout << "Ошибка при изменении времени последнего доступа к файлу." << endl;
            CloseHandle(hFile);
            Sleep(2000);
            choice();
        }
        else {
            cout << "Время последнего доступа к файлу успешно изменено." << endl;
            CloseHandle(hFile);
            Sleep(2000);
            choice();
        }
        break;
    }



    default:

        cout << "Некорректный выбор атрибута." << endl;
        while (flag < 1 || flag > 7)
        {
            cout << "Введите корректный номер выбора (0-9):\n";
            cin >> flag;
        }
    }

    if (!SetFileAttributes(wpath.c_str(), fileAttributes)) {
        cout << "Ошибка при изменении атрибутов файла." << endl;
        Sleep(2000);
        choice();
    }
    else {
        cout << "Атрибуты файла успешно изменены." << endl;
        Sleep(2000);
        choice();
    }
}


void choice()
{
    system("cls");
    int choice=-1;
    cout << "1 - Вывести список дисков\n";
    cout << "2 - Вывести информацию о диске\n";
    cout << "3 - Создать каталог\n";
    cout << "4 - Удалить каталог\n";
    cout << "5 - Создать файл\n";
    cout << "6 - Копировать файл\n";
    cout << "7 - Переместить файл\n";
    cout << "8 - Вывести атрибуты файла\n";
    cout << "9 - Изменить атрибуты файла\n";
    cout << "0 - Завершить программу\n";

    cin >> choice;

    while (choice < -1 || choice > 9)
    {
        cout << "Ошибка. Повторите ввод\n";
        cin >> choice;
    }

    switch (choice)
    {
        case 0: exit(0);
        case 1: show_disks();
        case 2: disk_choice();
        case 4: delete_folder();
        case 3: new_folder();
        case 5: new_file();
        case 6: copy_file();
        case 7: move_file();
        case 8: show_file_attr(path_input());
        case 9: edit_file_attr();
    }
}


int main()
{

    setlocale(LC_ALL, "Russian");

    choice();

    return 0;
}
