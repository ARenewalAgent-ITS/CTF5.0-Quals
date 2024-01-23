#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <windef.h>
#include <psapi.h>
#include <TlHelp32.h>
#include "cryptlib.h"
#include "hex.h"
#include "filters.h"
#include "aes.h"
#include "ccm.h"
#define INJECTOREXE L"LughTuathaDe.exe"
#define KEYOFFSET 0x40348
#define CIPHEROFFSET 0x403A0
#define HIMITSUSIZE 65

using namespace std;

using CryptoPP::Exception;
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AES;
using CryptoPP::CBC_Mode;

const WCHAR SharedMemoryName[] = L"Global\\Harem";

// was about to use this to read those values but something 
// went wrong, and I don't have the time to fix it atm
// src: https://github.com/PierreCiholas/GetBaseAddress
DWORD GetBaseAddress(const HANDLE hProcess) {
    if (hProcess == NULL)
        return NULL; // No access to the process

    HMODULE lphModule[1048]; // Array that receives the list of module handles
    DWORD Needed(NULL); // Output of EnumProcessModules, giving the number of bytes requires to store all modules handles in the lphModule array

    if (!EnumProcessModulesEx(hProcess, lphModule, sizeof(lphModule), &Needed, LIST_MODULES_64BIT))
        return NULL; // Impossible to read modules

    WCHAR ModuleName[MAX_PATH];
    if (!GetModuleFileNameEx(hProcess, lphModule[0], ModuleName, sizeof(ModuleName) / sizeof(WCHAR)))
        return NULL; // Impossible to get module info

    return (DWORD)lphModule[0]; // Module 0 is apparently always the EXE itself, returning its address
}

BOOL GetKeyAndCipher(BYTE* Key, CHAR* Cipher)
{
    // create a snapshot of all currently running process
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        cout << "Failed to create snapshot" << endl;
        return false;
    }

    // initialize the process entry structure
    LPPROCESSENTRY32 processEntry = (LPPROCESSENTRY32)(&PROCESSENTRY32());
    processEntry->dwSize = sizeof(PROCESSENTRY32);
    if (0 == Process32First(hSnapshot, processEntry))
    {
        cout << "Failed to get first process" << endl;
        CloseHandle(hSnapshot);
        return false;
    }

    // iterate through the snapshot and find the injector
    DWORD InjectorPID = NULL;
    do
    {
        wstring name(processEntry->szExeFile);

        if (!wcscmp(name.c_str(), INJECTOREXE)) // found the injector process
        {
            InjectorPID = processEntry->th32ProcessID;
            break;
        }
    } while (NULL != Process32Next(hSnapshot, processEntry));
    CloseHandle(hSnapshot);

    // open handle to injector process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, InjectorPID);
    if (INVALID_HANDLE_VALUE == hProcess || !hProcess)
    {
        cout << "Failed to open process" << endl;
        return false;
    }

    // get module path of the injector
    CHAR ModulePath[MAX_PATH];
    if (!GetModuleFileNameExA(hProcess, NULL, ModulePath, MAX_PATH))
    {
        cout << "Failed to get module details" << endl;
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hProcess);

    // open handle to to the injector binary
    // though the function named create file, this is set to only read the file
    HANDLE hInjectorExe = CreateFileA(ModulePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hInjectorExe || !hInjectorExe)
    {
        cout << "Failed to open injector executable" << endl;
        return false;
    }

    // https://www.ired.team/miscellaneous-reversing-forensics/aes-encryption-example-using-cryptopp-.lib-in-visual-studio-c++
    SetFilePointer(hInjectorExe, KEYOFFSET, NULL, FILE_BEGIN);
    if (!ReadFile(hInjectorExe, Key, AES::MAX_KEYLENGTH, NULL, NULL))
    {
        cout << "Failed to read key" << endl;
        CloseHandle(hInjectorExe);
        return false;
    }
    
    SetFilePointer(hInjectorExe, CIPHEROFFSET, NULL, FILE_BEGIN);
    if (!ReadFile(hInjectorExe, Cipher, HIMITSUSIZE + 1, NULL, NULL))
    {
        cout << "Failed to read cipher" << endl;
        CloseHandle(hInjectorExe);
        return false;
    }

    CloseHandle(hInjectorExe);
    return true;
}

// https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory
BOOL GetIV(BYTE* iv)
{
    LONG BufferSize = 0x100;
    HANDLE hMapFile;
    LPTSTR pBuffer;

    hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SharedMemoryName);
    if (hMapFile == NULL)
    {
        cout << "Failed to open file mapping object" << endl;
        return false;
    }

    pBuffer = (LPTSTR) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BufferSize);
    if (pBuffer == NULL)
    {
        cout << "Failed to map view of file" << endl;
        return false;
    }

    //CopyMemory( (PVOID) pBuffer, iv, AES::BLOCKSIZE);
    memcpy(iv, pBuffer, AES::BLOCKSIZE);
    
    CloseHandle(hMapFile);
    UnmapViewOfFile(pBuffer);

    return true;
}

string Decrypt()
{
    // YT
    BYTE key[AES::MAX_KEYLENGTH];
    // Harem
    BYTE iv[AES::BLOCKSIZE];
    // himitsu
    CHAR himitsu[HIMITSUSIZE + 1] = { '\x00' };
    
    if (!GetIV(iv) || !GetKeyAndCipher(key, himitsu))
    {
        return string("");
    }

    string cipher(himitsu);
    string plain;

    try 
    {
        CBC_Mode< AES >::Decryption d;
        d.SetKeyWithIV(key, sizeof(key), iv);
        StringSource s(cipher, true,
            new StreamTransformationFilter(d,
                new StringSink(plain)
            )
        );
    }
    catch (const CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    return plain;
}

void Win()
{
    AllocConsole();
    FILE* f = new FILE;
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    string flag = Decrypt();

    if (flag.empty()) {
        cerr << "Failed to decrypt flag" << endl;
        MessageBoxA(NULL, "Failed to decrypt flag. Please contact Author", "ERR", MB_ICONSTOP);
    }
    else {
        cout << flag << endl;
        MessageBoxA(NULL, "Congrats I guess?", "Totally not sus DLL", MB_ICONINFORMATION);
        getline(cin, flag);
    }

    //exit(0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxA(NULL, "Hmmmm?", "Hm?", MB_ICONINFORMATION);
        Win();
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }


    return TRUE;
}

