#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <atlbase.h>
#include <atlconv.h>
#include <string>
#include <vector>
#include <map>
#include <locale>

using namespace std;
#define DIV 1024
#define WIDTH 7

DWORD dwSize = 4096;

void choose(LPVOID addr);

int auto_or_not()
{
    int flag = 0;
    cout << "Do you want to choose region?\n1 - yes\n2 - do it automaticly\n";
    cin >> flag;
    return flag;
}


void system_info(LPVOID addr)
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n",
        siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: 0x%08X\n",
        siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: 0x%08X\n",
        siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: 0x%08X\n",
        siSysInfo.dwActiveProcessorMask);

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(addr);
}

void memory_status(LPVOID addr)
{
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    _tprintf(TEXT("There is  %*ld percent of memory in use.\n"),
        WIDTH, statex.dwMemoryLoad);
    _tprintf(TEXT("There are %*I64d total KB of physical memory.\n"),
        WIDTH, statex.ullTotalPhys / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of physical memory.\n"),
        WIDTH, statex.ullAvailPhys / DIV);
    _tprintf(TEXT("There are %*I64d total KB of paging file.\n"),
        WIDTH, statex.ullTotalPageFile / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of paging file.\n"),
        WIDTH, statex.ullAvailPageFile / DIV);
    _tprintf(TEXT("There are %*I64d total KB of virtual memory.\n"),
        WIDTH, statex.ullTotalVirtual / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of virtual memory.\n"),
        WIDTH, statex.ullAvailVirtual / DIV);
    _tprintf(TEXT("There are %*I64d free  KB of extended memory.\n"),
        WIDTH, statex.ullAvailExtendedVirtual / DIV);

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(addr);
}

void block_info(LPVOID addr)
{


    LPVOID reservedMemory = NULL;

    MEMORY_BASIC_INFORMATION info;
    VirtualQuery(addr, &info, sizeof(info));

    cout << "BaseAddress: 0x" << hex << info.BaseAddress << dec << endl;
    cout << "AllocationBase: 0x" << hex << info.AllocationBase << dec << endl;

    cout << "AllocationProtect: ";
    switch (info.AllocationProtect)
    {
    case PAGE_NOACCESS:
        cout << "PAGE_NOACCESS";
        break;
    case PAGE_READONLY:
        cout << "PAGE_READONLY";
        break;
    case PAGE_READWRITE:
        cout << "PAGE_READWRITE";
        break;
    case PAGE_WRITECOPY:
        cout << "PAGE_WRITECOPY";
        break;
    case PAGE_EXECUTE:
        cout << "PAGE_EXECUTE";
        break;
    case PAGE_EXECUTE_READ:
        cout << "PAGE_EXECUTE_READ";
        break;
    case PAGE_EXECUTE_READWRITE:
        cout << "PAGE_EXECUTE_READWRITE";
        break;
    case PAGE_EXECUTE_WRITECOPY:
        cout << "PAGE_EXECUTE_WRITECOPY";
        break;
    case PAGE_GUARD:
        cout << "PAGE_GUARD";
        break;
    case PAGE_NOCACHE:
        cout << "PAGE_NOCACHE";
        break;
    case PAGE_WRITECOMBINE:
        cout << "PAGE_WRITECOMBINE";
        break;
    default:
        cout << "Unknown protection: " << info.AllocationProtect;
        break;
    }
    cout << endl;

    cout << "RegionSize: 0x" << hex << info.RegionSize << dec << endl;

    cout << "State: ";
    switch (info.State)
    {
    case MEM_COMMIT:
        cout << "MEM_COMMIT";
        break;
    case MEM_FREE:
        cout << "MEM_FREE";
        break;
    case MEM_RESERVE:
        cout << "MEM_RESERVE";
        break;
    default:
        cout << "Unknown state: " << info.State;
        break;
    }
    cout << endl;

    cout << "Protect: ";
    switch (info.Protect)
    {
    case PAGE_NOACCESS:
        cout << "PAGE_NOACCESS";
        break;
    case PAGE_READONLY:
        cout << "PAGE_READONLY";
        break;
    case PAGE_READWRITE:
        cout << "PAGE_READWRITE";
        break;
    case PAGE_WRITECOPY:
        cout << "PAGE_WRITECOPY";
        break;
    case PAGE_EXECUTE:
        cout << "PAGE_EXECUTE";
        break;
    case PAGE_EXECUTE_READ:
        cout << "PAGE_EXECUTE_READ";
        break;
    case PAGE_EXECUTE_READWRITE:
        cout << "PAGE_EXECUTE_READWRITE";
        break;
    case PAGE_EXECUTE_WRITECOPY:
        cout << "PAGE_EXECUTE_WRITECOPY";
        break;
    case PAGE_GUARD:
        cout << "PAGE_GUARD";
        break;
    case PAGE_NOCACHE:
        cout << "PAGE_NOCACHE";
        break;
    case PAGE_WRITECOMBINE:
        cout << "PAGE_WRITECOMBINE";
        break;
    default:
        cout << "Unknown protection: " << info.Protect;
        break;
    }
    cout << endl;

    cout << "Type: ";
    switch (info.Type)
    {
    case MEM_IMAGE:
        cout << "MEM_IMAGE";
        break;
    case MEM_MAPPED:
        cout << "MEM_MAPPED";
        break;
    case MEM_PRIVATE:
        cout << "MEM_PRIVATE";
        break;
    default:
        cout << "Unknown type: " << info.Type;
        break;
    }
    cout << endl;

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(addr);
}

void reserve_block_without_memory(LPVOID addr)
{
    DWORD dwSize = 4096;
    DWORD dwAllocationType = MEM_RESERVE;
    DWORD dwProtect = PAGE_READWRITE;
    int mode = 0;
    LPVOID lpReservedMem=0;

    cout << "select mode:\n1 - automaticly\n2 - at a chosen beginning of address\n";
    cin >> mode;
    if (mode == 1)
    {
        lpReservedMem = VirtualAlloc(NULL, dwSize, dwAllocationType, dwProtect);
    }
    else if (mode == 2)
    {
        lpReservedMem = VirtualAlloc(addr, dwSize, dwAllocationType, dwProtect);

    }


    if (lpReservedMem == NULL)
    {
        DWORD error_code = GetLastError();
        cout << "VirtualAlloc failed with error code " << error_code << endl;
    }
    else
    {
        cout << "Virtual memory reserved successfully." << endl;
        cout << "address: " << lpReservedMem << endl;
       
    }

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(lpReservedMem);
}

void reserve_block(LPVOID addr)
{
    DWORD dwSize = 4096;
    DWORD dwAllocationType = MEM_COMMIT;
    DWORD dwProtect = PAGE_READWRITE;
    LPVOID lpReservedMem=0;
    int mode = 0;

    cout << "select mode:\n1 - automaticly\n2 - at a chosen beginning of address\n";
    cin >> mode;
    if (mode == 1)
    {
        lpReservedMem = VirtualAlloc(NULL, dwSize, dwAllocationType, dwProtect);
    }
    else if (mode == 2)
    {
        lpReservedMem = VirtualAlloc(addr, dwSize, dwAllocationType, dwProtect);
    }

    if (lpReservedMem == NULL)
    {
        DWORD error_code = GetLastError();
        cout << "VirtualAlloc failed with error code " << error_code << endl;
    }
    else
    {
        cout << "Virtual memory reserved and committed successfully." << endl;
        cout << "address: " << lpReservedMem << endl;
  
    }

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(lpReservedMem);
}


void free_region(LPVOID addr)
{


    if (VirtualFree(addr, 0, MEM_RELEASE) == 0)
    {
        DWORD error_code = GetLastError();
        cout << "VirtualFree failed with error code " << error_code << endl;
    }
    else
    {
        cout << "Memory freed successfully." << endl;
    }

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(addr);
}




void write_in_block(LPVOID addr)
{

    size_t size;
    cout << "Введите размер данных для записи: ";
    cin >> size;
    cin.ignore();

    char* buffer = new char[size + 1];

    cout << "Введите данные для записи: ";
    cin.getline(buffer, size);

    if (WriteProcessMemory(GetCurrentProcess(), addr, buffer, size, 0) == 0)
    {
        DWORD error_code = GetLastError();
        cout << "ERROR :" << error_code << endl;
        delete[] buffer;
    }
    else
    {
        cout << "Данные записаны успешно. Сейчас ячейка содержит: " << endl;

        if (ReadProcessMemory(GetCurrentProcess(), addr, buffer, size, 0) == 0)
        {
            DWORD error_code = GetLastError();
            cout << "ERROR :" << error_code << endl;
            delete[] buffer;
        }

        for (SIZE_T i = 0; i < size; i++)
        {
            cout << buffer[i] << " ";
        }
        cout << endl;

        delete[] buffer;
    }

    Sleep(2000);
    choose(addr);
}


void est_protect(LPVOID addr)
{

    MEMORY_BASIC_INFORMATION info;
    VirtualQuery(addr, &info, sizeof(info));

    cout << "Current protection attributes:" << endl;
    vector <DWORD> values = { PAGE_NOACCESS, PAGE_READONLY, PAGE_READWRITE,
        PAGE_WRITECOPY, PAGE_EXECUTE, PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE,
        PAGE_EXECUTE_WRITECOPY, PAGE_GUARD, PAGE_NOCACHE, PAGE_WRITECOMBINE };

    vector <string> names = { "PAGE_NOACCESS", "PAGE_READONLY", "PAGE_READWRITE",
        "PAGE_WRITECOPY", "PAGE_EXECUTE", "PAGE_EXECUTE_READ", "PAGE_EXECUTE_READWRITE",
        "PAGE_EXECUTE_WRITECOPY", "PAGE_GUARD", "PAGE_NOCACHE", "PAGE_WRITECOMBINE" };

    map<int, DWORD> protect_attrs =
    {
        { 1, PAGE_NOACCESS },
        { 2, PAGE_READONLY },
        { 3, PAGE_READWRITE },
        { 4, PAGE_WRITECOPY },
        { 5, PAGE_EXECUTE },
        { 6, PAGE_EXECUTE_READ },
        { 7, PAGE_EXECUTE_READWRITE },
        { 8, PAGE_EXECUTE_WRITECOPY },
        { 9, PAGE_GUARD },
        { 10, PAGE_NOCACHE },
        { 11, PAGE_WRITECOMBINE }
    };

    for (const auto& attr : protect_attrs)
    {
        cout << attr.first << " - " << names[attr.first - 1];
        if (info.Protect & attr.second)
        {
            cout << " : YES\n";
        }
        else
        {
            cout << " : NO\n";
        }
    }

    int new_protect_num = 0;
    cout << "Do you want to change attribute?\n0 - no\nn - yes (n - number of attribute)\n";
    cin >> new_protect_num;

    if (new_protect_num == 0)
    {
        choose(addr);
    }
    else if (new_protect_num < 1 || new_protect_num > 11)
    {
        cout << "Invalid attribute number. Protection attributes unchanged.\n";
    }
    else
    {
        DWORD old_protect;
        if (!VirtualProtect(addr, 4096, values[new_protect_num - 1], &old_protect)) {
            DWORD error_code = GetLastError();
            cout << "VirtualProtect failed with error code " << error_code << endl;
        }
    }

    VirtualQuery(addr, &info, sizeof(info));
    cout << "changes:\n";

    for (const auto& attr : protect_attrs)
    {
        cout << attr.first << " - " << names[attr.first - 1];
        if (info.Protect & attr.second)
        {
            cout << " : YES\n";
        }
        else
        {
            cout << " : NO\n";
        }
    }

    cout << "\nPress 0 to return to the main menu...";
    int choice = -1;
    cin >> choice;
    while (choice != 0)
    {
        cout << "Error. Please enter 0 to return to the main menu.\n";
        cin >> choice;
    }
    choose(addr);
}




void choose(LPVOID addr)
{
    system("cls");
    cout << "Chosen region: 0x" << hex << addr << dec << endl << endl;
    int choice = -1;
    cout << "1 - Output information about the computer system\n";
    cout << "2 - Output the status of virtual memory\n";
    cout << "3 - Output the state of a memory region\n";
    cout << "4 - Reserve a region\n";
    cout << "5 - Reserve a region and assign it physical memory\n";
    cout << "6 - Write data to a memory cell\n";
    cout << "7 - Set access protection for a region\n";
    cout << "8 - Return physical memory and release the region\n";
    cout << "0 - Exit the program\n";

    cin >> choice;

    while (choice < -1 || choice > 8)
    {
        cout << "Error. Please re-enter your choice\n";
        cin >> choice;
    }

    switch (choice)
    {
    case 0: exit(0);
    case 1: system_info(addr); break;
    case 2: memory_status(addr); break;
    case 3: block_info(addr); break;
    case 4: reserve_block_without_memory(addr); break;
    case 5: reserve_block(addr); break;
    case 6: write_in_block(addr); break;
    case 7: est_protect(addr); break;
    case 8: free_region(addr); break;
    }
}

int main()
{
    DWORD address = 0x000000000AF0000;
    string region;
    LPVOID addr = 0;
    int flag = auto_or_not();

    SYSTEM_INFO systemInfo;

    GetSystemInfo(&systemInfo);

    if (flag == 1)
    {
        cout << "Enter an address in the range of 0x" << systemInfo.lpMinimumApplicationAddress << " before 0x" << systemInfo.lpMaximumApplicationAddress << ": ";
        cin >> region;
        address = strtoul(region.c_str(), NULL, 16);
        addr = reinterpret_cast<LPVOID>(address);
    }
    else
    {
        addr = reinterpret_cast<LPVOID>(address);
    }

    setlocale(LC_ALL, "Russian");

    choose(addr);

    return 0;
}