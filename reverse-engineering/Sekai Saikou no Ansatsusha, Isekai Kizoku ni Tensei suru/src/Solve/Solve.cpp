#include <Windows.h>
#include <iostream>
#define SIZE 0x100

using namespace std;

const WCHAR FlagMemoryName[] = L"Global\\Flag";

void main()
{
    HANDLE hSharedFile;
    LPTSTR pBuffer;
    CHAR FlagBuf[SIZE + 1] = { 0 };

    hSharedFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, FlagMemoryName);
    if (hSharedFile == NULL)
    {
        cout << "Flag file mapping not found" << endl;
        return;
    }

    pBuffer = (LPTSTR)MapViewOfFile(hSharedFile, FILE_MAP_ALL_ACCESS, 0, 0, SIZE);
    if (pBuffer == NULL)
    {
        CloseHandle(hSharedFile);
        cout << "Failed to map view of file" << endl;
        return;
    }

    memcpy(FlagBuf, (PVOID)pBuffer, SIZE);
    UnmapViewOfFile(pBuffer);

    cout << "Flag: " << FlagBuf << endl;
    return;
}
