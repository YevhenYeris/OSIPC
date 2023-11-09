#include <stdio.h>
#include <windows.h>

#define BUF_SIZE 1024
#define ITERATIONS 1000

int file_open_read_write() {
	HANDLE hFile;
	char data[] = "Test message writing to a file\n";
	char buffer[256] = { 0 };
	LARGE_INTEGER frequency, start, end;
	double latency, throughput;

	hFile = CreateFile(
		L"readWrite.txt",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		printf("Error creating the file for writing. Error %d\n", GetLastError());
		return 1;
	}

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	DWORD bytesWritten;
	for (int i = 0; i < ITERATIONS; i++) {
		if (!WriteFile(hFile, data, strlen(data), &bytesWritten, NULL)) {
			printf("Error writing to file. Error %d\n", GetLastError());
			CloseHandle(hFile);
			return 1;
		}
	}

	QueryPerformanceCounter(&end);

	latency = ((double)(end.QuadPart - start.QuadPart) * 1000) / frequency.QuadPart / ITERATIONS;
	throughput = ((double)ITERATIONS * strlen(data)) / ((double)(end.QuadPart - start.QuadPart) / frequency.QuadPart);

	//printf("Write Latency: %f ms\n", latency);
	//printf("Write Throughput: %f bytes/s\n\n", throughput);

	printf("File Write, %f, %f\n", latency, throughput);

	CloseHandle(hFile);

	hFile = CreateFile(
		L"readWrite.txt",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		printf("Error opening the file for reading. Error %d\n", GetLastError());
		return 1;
	}

	QueryPerformanceCounter(&start);

	DWORD bytesRead;
	for (int i = 0; i < ITERATIONS; i++) {
		if (!ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL)) {
			printf("Error reading from file. Error %d\n", GetLastError());
			CloseHandle(hFile);
			return 1;
		}
	}

	QueryPerformanceCounter(&end);

	// Calculate latency and throughput for read operation
	latency = ((double)(end.QuadPart - start.QuadPart) * 1000) / frequency.QuadPart / ITERATIONS;
	throughput = ((double)ITERATIONS * sizeof(buffer)) / ((double)(end.QuadPart - start.QuadPart) / frequency.QuadPart);

	//printf("Read Latency: %f ms\n", latency);
	//printf("Read Throughput: %f bytes/s\n\n", throughput);

	//printf("Data read from the file: %s\n", buffer);

	printf("File Read, %f, %f\n", latency, throughput);

	CloseHandle(hFile);

	return 0;
}
