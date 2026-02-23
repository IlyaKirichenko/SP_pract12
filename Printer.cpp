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

    wchar_t appName[] = L"C:\\Users\\Your PC\\Documents\\Pract_12_SP\\Printer\\x64\\Debug\\Client.exe";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(appName,NULL,  NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cout << "Процесс не был запущен" << endl;
        return GetLastError();
    }
    else {
        cout << "ПРИНТЕР: Клиент запущен " << endl;
    }

    while (true) {
        DWORD result = WaitForSingleObject(PrinterMutex, 0);
        if (result == WAIT_OBJECT_0) {
            ReleaseMutex(PrinterMutex);
        }
        else if (result == WAIT_TIMEOUT) {
            WaitForSingleObject(PrinterMutex, INFINITE);

            int waitTime = rand() % 10000 + 5000;
            int timer = 0;
            bool errorPrint = false;

            cout << "ПРИНТЕР: Начинаю печатать" << endl;

            while (timer < waitTime) {
                Sleep(100);
                timer += 100;

                if (timer == 10000 && waitTime > 10000) {
                    cout << "ПРИНТЕР: Ошибка печати!" << endl;
                    cout << endl;
                    errorPrint = true;
                    break;
                }
            }
            if (!errorPrint) {
                cout << "ПРИНТЕР: Закончил печать" << endl;
                cout << endl;
            }
            ReleaseMutex(PrinterMutex);
            
        }
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(PrinterMutex);
    cout << "ПРИНТЕР: Выключен " << endl;
    return 0;
}