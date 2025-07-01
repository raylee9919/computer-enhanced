#include <windows.h>
#include <psapi.h>

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint8_t  u8;

#include "log.h"

u64 read_os_page_fault_count(HANDLE process_handle) {
    PROCESS_MEMORY_COUNTERS_EX memory_counters = {};
    memory_counters.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
    GetProcessMemoryInfo(process_handle, (PROCESS_MEMORY_COUNTERS *)&memory_counters, sizeof(memory_counters));

    u64 result = memory_counters.PageFaultCount;
    return result;
}

int main(int argc, char **argv) {
    HANDLE process_handle = GetCurrentProcess();

    if (argc == 1) {
        log(LOG_INFO, "Usage: %s [# of 4k pages to allocate]\n", argv[0]);
        return 1;
    } else if (argc != 2) {
        log(LOG_ERROR, "Invalid argument count.");
        log(LOG_INFO, "Usage: %s [# of 4k pages to allocate]\n", argv[0]);
        return 1;
    }

    u64 page_size  = 4096;
    u64 page_count = atoi(argv[1]);
    u64 total_size = page_size * page_count;

    printf("Page Count, Touch Count, Fault Count, Extra Faults\n");

    for (u64 touch_count = 1; touch_count <= page_count; ++touch_count) {
        u64 touch_size = page_size * touch_count;

        u8 *data = (u8 *)VirtualAlloc(0, total_size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

        if (!data) {
            log(LOG_ERROR, "Allocation failed.");
            return 1;
        }

        u64 start_fault_count = read_os_page_fault_count(process_handle);
        for (u64 index = 0; index < touch_size; ++index) {
            data[index] = (u8)index;
        }
        u64 end_fault_count = read_os_page_fault_count(process_handle);

        u64 fault_count = end_fault_count - start_fault_count;

        printf("%llu, %llu, %llu, %lld\n", page_count, touch_count, fault_count, (fault_count - touch_count));

        VirtualFree(data, 0, MEM_RELEASE);
    }

    log(LOG_INFO, "Execution successful.");
}
