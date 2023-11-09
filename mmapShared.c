#include <stdio.h>
#include <windows.h>
#include <time.h>

#define BUF_SIZE 1024
#define ITERATIONS 100000

int mmap_shared(DWORD mappingType, DWORD dwDesiredAccess, int shouldWrite) {
    HANDLE hMapFile;
    LPCTSTR pBuf;
    LARGE_INTEGER frequency, start, end;
    double latency, throughput;

    HANDLE hFile = CreateFile(
        L"mapping.txt",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        mappingType,
        0,
        BUF_SIZE,
        NULL);

    if (hMapFile == NULL) {
        printf("Could not create file mapping object (%d).\n", GetLastError());
        return 1;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // Handle to map object
        dwDesiredAccess, // Read/write permission
        0,
        0,
        BUF_SIZE);

    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    char buffer[BUF_SIZE] = "";
    for (int i = 0; i < BUF_SIZE; i += 1) {
        buffer[i] = "T";
    }

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    for (int i = 0; i < ITERATIONS; i++) {
        //printf("Iteration number %i\n", i + 1);
        if (shouldWrite) {
            CopyMemory((PVOID)pBuf, buffer, BUF_SIZE);
        }
        else {
            CopyMemory(buffer, pBuf, BUF_SIZE);
        }
    }

    QueryPerformanceCounter(&end);

    latency = ((double)(end.QuadPart - start.QuadPart) * 1000) / frequency.QuadPart / ITERATIONS;
    throughput = ((double)ITERATIONS * BUF_SIZE) / ((double)(end.QuadPart - start.QuadPart) / frequency.QuadPart);

    //printf("Latency: %f ms\n", latency);
    //printf("Throughput: %f bytes/s\n\n", throughput);

    printf("%f, %f\n", latency, throughput);

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return 0;
}
