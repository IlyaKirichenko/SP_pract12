#include <iostream>
#include <Windows.h>
#include <ctime>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    cout << "=== ПРИНТЕР === " << endl;

    HANDLE PrinterMutex = CreateMutex(NULL, FALSE, L"PrinterMutex");

    if (PrinterMutex == NULL) {
        cout << "Ошибка создания мьютекса! " << endl;
        return GetLastError();
    }

    cout << "Принтер ждет клиентов " << endl;

    wchar_t appName[] = L"C:\\Users\\st310-02\\Documents\\Kirichenko_PR-31\\Pract_12_SP\\Pract_12_SP\\Printer\\x64\\Debug\\Client.exe";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(NULL, appName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cout << "Процесс не был запущен" << endl;
        return GetLastError();
    }
    else {
        cout << "ПРИНТЕР: Клиент запущен " << endl;
    }

    while (true) {
        int closePrinter = WaitForSingleObject(PrinterMutex, 60000);

        if (closePrinter == WAIT_TIMEOUT) {
            cout << "ПРИНТЕР: Ошибка печати " << endl;
            break; 
        }

        if (closePrinter == WAIT_OBJECT_0) {
            cout << "ПРИНТЕР: Печатаю... " << endl;

            int printTime = 5000 + rand() % 5000;
            Sleep(printTime);

            cout << "ПРИНТЕР: Закончил печать " << endl;

            ReleaseMutex(PrinterMutex);
        }
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(PrinterMutex);
    cout << "ПРИНТЕР: Выключен " << endl;
    return 0;
}