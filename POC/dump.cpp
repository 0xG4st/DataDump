#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <psapi.h>
#include <sstream>

using namespace std;

int main()
{
    DWORD processID;
    cout << "Enter process ID: ";
    cin >> processID;

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL)
    {
        cout << "Could not open process." << endl;
        return 1;
    }

    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        ofstream file("modules-file.txt");
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                file << "Module name: " << szModName << endl;
                MODULEINFO modInfo;
                if (GetModuleInformation(hProcess, hMods[i], &modInfo, sizeof(modInfo)))
                {   
                    file << "Base address: " << modInfo.lpBaseOfDll << endl;
                    file << "Module size: 0x" << modInfo.SizeOfImage << endl;
                }
                file << endl;
            }
        }
        file.close();
    }
    else
    {
        cout << "Could not enumerate process modules." << endl;
    }

    CloseHandle(hProcess);
    return 0;
}
