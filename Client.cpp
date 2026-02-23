#include <iostream>
#include <Windows.h>
#include <ctime>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    cout << "=== КЛИЕНТ === " << endl;
    cout << "Выберите приоритет клиента: " << endl;
    cout << "1. Фоновый " << endl;
    cout << "2. Нормальный " << endl;
    cout << "3. Высокий " << endl;

    int choise;
    cin >> choise;

    if (choise > 3 || choise < 1) {
        cout << "Введите число от 1 до 3 " << endl;
        choise = 2;
    }

    HANDLE PrProc = GetCurrentProcess();

    switch (choise)
    {
    case 1:
        SetPriorityClass(PrProc, IDLE_PRIORITY_CLASS);
        cout << "Установлен приоритет: Фоновый " << endl;
        break;
    case 2:
        SetPriorityClass(PrProc, NORMAL_PRIORITY_CLASS);
        cout << "Установлен приоритет: Нормальный " << endl;
        break;
    case 3:
        SetPriorityClass(PrProc, HIGH_PRIORITY_CLASS);
        cout << "Установлен приоритет: Высокий " << endl;
        break;
    }

    HANDLE PrinterMutex = OpenMutex(SYNCHRONIZE, FALSE, L"PrinterMutex");

    if (PrinterMutex == NULL) {
        cout << "Принтер не найден" << endl;
        return GetLastError();
    }
    else {
        cout << "Принтер обнаружен " << endl;
    }

    while (true)
    {
        cout << "\nНажмите Enter для печати" << endl;
        cin.ignore();
        cin.get();

        DWORD waitResult = WaitForSingleObject(PrinterMutex, INFINITE);

        if (waitResult == WAIT_OBJECT_0) {
            cout << "Задание отправлено в очередь!" << endl;
            ReleaseMutex(PrinterMutex);
            cout << "Ожидание завершения печати принтером..." << endl;
        }
        else {
            cout << "Ошибка доступа к принтеру!" << endl;
        }
    }


    return 0;
}