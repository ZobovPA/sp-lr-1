#define WINVER 0x0A00
#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;
#define INFO_BUFFER_SIZE 32767
#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define WINVER 0x0502
#define MAX_KEY_LENGTH 255
#define MAX_DATA_LENGTH 16383

int main()
{

    //1.1

    OSVERSIONINFO os_vers = {0};
    os_vers.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&os_vers);
    DWORD dwMinorVersion = os_vers.dwMinorVersion;
    DWORD dwMajorVersion = os_vers.dwMajorVersion;
    DWORD dwBuild = os_vers.dwBuildNumber;

    printf("Version of OS is %d.%d.%d\n",
                dwMajorVersion,
                dwMinorVersion,
                dwBuild);


    //1.2

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE );
    printf("\nSystem directory %s", infoBuf);


    //1.3

    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    GetUserName( infoBuf, &bufCharCount);
    printf("\nUser name:%s", infoBuf);

    bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName( infoBuf, &bufCharCount );
    printf("\nComputer name: %s\n", infoBuf );



    //1.4
    char buffer[MAX_PATH];
    DWORD  cchBufferLength = MAX_PATH;
    char Names[MAX_PATH];
    __int64 total, available, free;


    HANDLE search = FindFirstVolume(buffer, sizeof(buffer));

    do {
        printf("\n%s", buffer);
        GetVolumePathNamesForVolumeName(buffer, Names, cchBufferLength, &cchBufferLength);
        printf("\n  First path: %s", Names);
        GetDiskFreeSpaceEx(
                           buffer,
                           (PULARGE_INTEGER)&available,
                           (PULARGE_INTEGER)&total,
                           (PULARGE_INTEGER)&free);

        if (GetDiskFreeSpaceEx(
                               buffer,
                               (PULARGE_INTEGER)&available,
                               (PULARGE_INTEGER)&total,
                               (PULARGE_INTEGER)&free)==0)
        {
            printf("\n  No information available.\n ");
        }
        else
        {
        cout << "Total size (bytes): " << total << endl;
        cout << "Free space (bytes): " << available << endl;
        }

    } while (FindNextVolume(search, buffer, sizeof(buffer)));
    FindVolumeClose(search);

    printf("\n------------------------\n");


    //1.5
    HKEY hKey;
    DWORD dwSize;
    TCHAR Progbuffer[MAX_KEY_LENGTH];
    DWORD In = 0;
    DWORD retCode;
    DWORD Pblen = 32767;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(Pblen);
    DWORD cbData = Pblen;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_ALL_ACCESS, &hKey) == !ERROR_SUCCESS)
    {
        cout << "Function RegOpenKeyEx() failed!" << endl;
    }
    else cout << "\Startup commands:" << endl;

    while (1) {
        dwSize = sizeof(Progbuffer);
        retCode = RegEnumValue(hKey, In, Progbuffer, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, Progbuffer, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("      %d: %s:  %s\n", In + 1, Progbuffer, PerfData);
            In++;
        }
        else break;
    }


    //2

    LARGE_INTEGER freq;
    LARGE_INTEGER t0;
    LARGE_INTEGER t;

    QueryPerformanceCounter(&t0);
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);

    double tacts = t.QuadPart - t0.QuadPart;
    double frequency = freq.QuadPart;;
    double us = 1000000 * tacts / frequency;

    printf("\nFrequency = %lld Hz\n",  freq.QuadPart);
    printf("Performance duration = %f us\n",  us);


}
