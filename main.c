#include <stdio.h>
#include <windows.h>

int main() {
    file_open_read_write();

    printf("PAGE_EXECUTE_READ, ");
    mmap_shared(PAGE_EXECUTE_READ, FILE_MAP_EXECUTE | FILE_MAP_READ, 0);
    printf("PAGE_EXECUTE_READWRITE, ");
    mmap_shared(PAGE_EXECUTE_READWRITE, FILE_MAP_ALL_ACCESS, 1);
    printf("PAGE_EXECUTE_WRITECOPY, ");
    mmap_shared(PAGE_EXECUTE_WRITECOPY, FILE_MAP_COPY, 1);
    printf("PAGE_READONLY, ");
    mmap_shared(PAGE_READONLY, FILE_MAP_READ, 0);
    printf("PAGE_READWRITE, ");
    mmap_shared(PAGE_READWRITE, FILE_MAP_ALL_ACCESS, 1);
    printf("PAGE_WRITECOPY, ");
    mmap_shared(PAGE_WRITECOPY, FILE_MAP_COPY, 1);
    printf("PAGE_READWRITE | SEC_COMMIT, ");
    mmap_shared(PAGE_READWRITE | SEC_COMMIT, FILE_MAP_ALL_ACCESS, 1);
    printf("PAGE_READONLY | SEC_COMMIT, ");
    mmap_shared(PAGE_READONLY | SEC_COMMIT, FILE_MAP_READ, 0);
    printf("PAGE_EXECUTE_READ | SEC_COMMIT, ");
    mmap_shared(PAGE_EXECUTE_READ | SEC_COMMIT, FILE_MAP_EXECUTE | FILE_MAP_READ, 0);

    return 0;
}
