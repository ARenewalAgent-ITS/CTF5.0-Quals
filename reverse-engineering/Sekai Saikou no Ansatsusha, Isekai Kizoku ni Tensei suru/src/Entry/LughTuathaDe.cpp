#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "cryptlib.h"
#include "hex.h"
#include "filters.h"
#include "aes.h"
#include "ccm.h"
#define TIMEOUT 60000 // 60s

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
const WCHAR MaliciousDLLName[] = L"TotallyNotMalicious.dll";
const CHAR YT[] = "https://www.youtube.com/watch?v=UvgCaAiWo_g&list=RDMM&index=27";
const SHORT obscure[] = { 0x39, 0x1c, 0x4, 0x0, 0x5, 0x2, 0xd, 0x60, 0xa, 0xc, 0x28, 0x61 }; // L"mspaint.exe";
const char himitsu[] = "\x59\xDE\x86\xB5\x51\xD7\x3E\x1B\x8F\x72\x39\x84\x0F\xE7\x3D\xDA\xA3\xFB\x5C\x93\x6C\xD9\x37\x65\xD0\x25\x2B\xDC\x04\x02\x7D\x02\x58\xB3\x46\xEA\x0B\x85\xF8\x70\x8E\x7C\xA1\x92\xC5\x32\x49\xA4\xE7\xB7\x5B\x9B\xA0\xF6\xBC\x6A\x5E\xB1\x53\x9E\x77\xAA\x0A\xB1";

WCHAR Harem[] = { 0x0, 0xe, 0x6, 0x15, 0x9, 0x21, 0x18, 0x26, 0xe, 0x22, 0x24, 0x4, 0x7, 0x6, 0xd, 0xc, 0x2b, 0x1c, 0x12 }; // L"TarteMahaViekoneDia";
WCHAR VictimProcessName[sizeof(obscure)] = {}; // L"mspaint.exe";

wstring MaliciousDLLPath;
DWORD VictimProcessId;
DWORD InjectedProcessId;
HANDLE hVictimProcess;
HANDLE hSharedFile;

// https://www.ired.team/miscellaneous-reversing-forensics/aes-encryption-example-using-cryptopp-.lib-in-visual-studio-c++
string Encrypt(string plain)
{
    BYTE key[AES::MAX_KEYLENGTH] = {0};
    memcpy(key, YT, AES::MAX_KEYLENGTH);

    BYTE iv[AES::BLOCKSIZE] = {0};
    memcpy(iv, Harem, AES::BLOCKSIZE);

    //string encoded;

    //encoded.clear();
    //StringSource(iv, sizeof(iv), true,
    //    new HexEncoder(
    //        new StringSink(encoded)
    //    )
    //);
    //cout << "iv: " << encoded << endl;

    //encoded.clear();
    //StringSource(key, sizeof(key), true,
    //    new HexEncoder(
    //        new StringSink(encoded)
    //    ) 
    //);
    //cout << "key: " << encoded << endl;

    //wcout << "Harem: " << Harem << endl;

    string cipher;

    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, sizeof(key), iv);
        StringSource s(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            )
        );
    }
    catch (const CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    //encoded.clear();
    //StringSource(cipher, true,
    //    new HexEncoder(
    //        new StringSink(encoded)
    //    )
    //);
    //cout << "cipher text: " << encoded << endl;

    return cipher;
}

string Decrypt()
{
    BYTE key[AES::MAX_KEYLENGTH];
    memcpy(key, YT, AES::MAX_KEYLENGTH);
    BYTE iv[AES::BLOCKSIZE];
    memcpy(iv, Harem, AES::BLOCKSIZE);

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

    cout << "plain: " << plain << endl;
    return plain;
}

BOOL isVictim(const WCHAR name[])
{
    if (0x6d != name[0]) return false;
    if (0x73 != name[1]) return false;
    if (0x70 != name[2]) return false;
    if (0x61 != name[3]) return false;
    if (0x69 != name[4]) return false;
    if (0x6e != name[5]) return false;
    if (0x74 != name[6]) return false;
    if (0x2e != name[7]) return false;
    if (0x65 != name[8]) return false;
    if (0x78 != name[9]) return false;
    if (0x65 != name[10]) return false;
    return true;
}

BOOL Validate(wstring input)
{
    SIZE_T size = input.length();
    if (size != 25 || input.substr(0, 5) != L"ARA5{")
    {
        MessageBoxW(NULL, L"Wrong Code", L"Ooopsie Doopsie", NULL);
        exit(1);
    }

    memset(VictimProcessName, 0, sizeof(VictimProcessName));
    input = input.substr(5, 19);

    for (int i = 0; i < input.length(); i++)
    {
        Harem[i] = Harem[i] ^ MaliciousDLLName[i];
        VictimProcessName[i] = (WCHAR) (MaliciousDLLName[i] ^ obscure[i]);

        if (input[i] != Harem[i])
        {
            MessageBoxW(NULL, L"Wrong Code", L"Ooopsie Doopsie", NULL);
            exit(1);
		}
	}
    
    MessageBoxW(NULL, L"Yeay Correct !!! :3", L":D", NULL);
    return true;
}

DWORD SearchVictimPID(const WCHAR* processName)
{
    // create a snapshot of all currently running process
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return NULL;
    }

    // initialize the process entry structure
    LPPROCESSENTRY32 processEntry = (LPPROCESSENTRY32)(&PROCESSENTRY32());
    processEntry->dwSize = sizeof(PROCESSENTRY32);
    if (0 == Process32First(hSnapshot, processEntry))
    {
        return NULL;
    }

    // iterate through the snapshot and find the victim process
    DWORD ret = NULL;
    do
    {
        wstring name(processEntry->szExeFile);

        if (isVictim(name.c_str())) // found the victim process
        {
            ret = processEntry->th32ProcessID;
            break;
        }
    } while (NULL != Process32Next(hSnapshot, processEntry));

    CloseHandle(hSnapshot);
    return ret;
}

wstring LocateDLLInModulePath(const WCHAR* DLLName)
{
    // get current execution path
    WCHAR path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);

    // remove the executable name from the path
    wstring::size_type pos = wstring(path).find_last_of(L"\\");
    wstring directory = wstring(path).substr(0, pos) + L"\\*";

    // list files to find the DLL
    WIN32_FIND_DATA data;
    HANDLE hDirList = FindFirstFileW(directory.c_str(), &data);
    if (INVALID_HANDLE_VALUE == hDirList || !hDirList)
    {
        return wstring(L"");
    }

    wstring ret(L"");
    do {
        wstring name(data.cFileName);
        if (!wcscmp(name.c_str(), DLLName)) // found the DLL
        {
            ret = wstring(path).substr(0, pos) + L"\\" + name;
            break;
        }
    } while (NULL != FindNextFileW(hDirList, &data));

    CloseHandle(hDirList);
    return ret;
}

HANDLE _OpenProcess(DWORD pid)
{
    // open handle to victim process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (INVALID_HANDLE_VALUE == hProcess || !hProcess)
    {
        return NULL;
    }

    return hProcess;
}

BOOL InjectDLL(HANDLE hProcess, const WCHAR* DLLPath)
{
    DWORD ret = WAIT_FAILED;
    SIZE_T size = (wcslen(DLLPath) + 1) * sizeof(DLLPath[0]);

    // allocate a virtual memory space in the victim process
    LPVOID lpVirtualAddr = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!lpVirtualAddr)
    {
        return false;
    }

    // writing the name of the dll to the allocated memory space
    if (WriteProcessMemory(hProcess, lpVirtualAddr, DLLPath, size, NULL))
    {
        HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
        if (!hKernel32)
        {
            return false;
        }

        FARPROC fpLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryW");
        if (!fpLoadLibrary)
        {
            return false;
        }

        // Inject to the remote process
        HANDLE hInjectedThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)fpLoadLibrary, lpVirtualAddr, NULL, NULL);
        if (hInjectedThread)
        {
            ret = WaitForSingleObject(hInjectedThread, TIMEOUT);
            CloseHandle(hInjectedThread);
            hInjectedThread = NULL;
        }
    }

    // cleanup
    VirtualFreeEx(hProcess, lpVirtualAddr, 0, MEM_RELEASE);
    
    if (WAIT_OBJECT_0 == ret)
    {
		return true;
	}

    return false;
}

// https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory
BOOL ShareHimitsu()
{
    LONG BufferSize = 0x100;
    LPTSTR pBuffer;

    hSharedFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SharedMemoryName);
    if (hSharedFile == NULL)
    {
        hSharedFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BufferSize, SharedMemoryName);
        if (hSharedFile == NULL)
        {
            return false;
        }
    }
    
    pBuffer = (LPTSTR) MapViewOfFile(hSharedFile, FILE_MAP_ALL_ACCESS, 0, 0, BufferSize);
    if (pBuffer == NULL)
    {
		CloseHandle(hSharedFile);
		return false;
	}

    CopyMemory( (PVOID) pBuffer, Harem, AES::BLOCKSIZE);
    UnmapViewOfFile(pBuffer);

    return true;
}

void wmain(int argc, WCHAR* argv[])
{
    wstring input;
    MessageBoxW(NULL, L"Ooops seems this game is not verified yet", L"Totally not sus lmao", NULL);
    printf("Enter your code: ");
    getline(wcin, input);

    if (Validate(input))
    {
        MaliciousDLLPath = LocateDLLInModulePath(MaliciousDLLName);
        if (MaliciousDLLPath.empty())
        {
            return;
        }

        VictimProcessId = SearchVictimPID(VictimProcessName);
        if (NULL == VictimProcessId)
        {
			return;
		}

        if (!ShareHimitsu())
        {
            return; 
		}

        hVictimProcess = _OpenProcess(VictimProcessId);
        if (NULL == hVictimProcess)
        {
			return;
		}

        if (InjectDLL(hVictimProcess, MaliciousDLLPath.c_str()))
        {
		    printf("Injected\n");
        }

        Sleep(10000); // 10s
        CloseHandle(hSharedFile);
    }
}