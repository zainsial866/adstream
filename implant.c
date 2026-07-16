#include <windows.h>
#include <stdio.h>

int main(void) {
    // 1. Changed to WCHAR for Unicode compatibility
    WCHAR FSName[1024];
    WCHAR* target_stream = L"D:\\test\\alternate_data_stream\\test.txt:ads";
    BYTE data[] = "SECRET SAUCE";
    char received[1024];
    DWORD bytesOut;
    WIN32_FIND_STREAM_DATA stream_data;

    // 2. Added L prefix to "C:\\" and ensured proper types are passed
    GetVolumeInformationW(L"C:\\", NULL, 0, NULL, NULL, NULL, FSName, 1024);

    // 3. Changed to _wcsicmp because FSName is now a wide string
    if (_wcsicmp(FSName, L"NTFS") == 0) {

        HANDLE tFile = CreateFileW(target_stream,
            GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (tFile == INVALID_HANDLE_VALUE) {
            // target_stream is a wide string, so use %S (capital S) or %ls
            printf("[!] Could not open file: %S\r\n", target_stream);
            return -1;
        }

        WriteFile(tFile, data, sizeof(data), &bytesOut, NULL);

        // list the stream
        HANDLE hFind = FindFirstStreamW(target_stream, FindStreamInfoStandard, &stream_data, 0);
        if (hFind == INVALID_HANDLE_VALUE) {
            printf("[!] Could not find any streams!\n");
            return -1;
        }

        printf("ADS inside:\n");
        while (TRUE) {
            // 4. Fixed: Used .QuadPart and %I64u for the LARGE_INTEGER structure
            printf("(%I64u)\t%S\n", stream_data.StreamSize.QuadPart, stream_data.cStreamName);
            if (!FindNextStreamW(hFind, &stream_data))
                break;
        }
        FindClose(hFind);

        // 5. Fixed: Changed second parameter from NULL to 0 (expects a LONG)
        SetFilePointer(tFile, 0, NULL, FILE_BEGIN);

        // Clear memory buffer before reading to prevent trailing garbage characters
        RtlZeroMemory(received, sizeof(received));
        ReadFile(tFile, received, sizeof(received) - 1, &bytesOut, NULL);

        // and print it
        printf("[+] Data from ADS: (%d) %s\n", bytesOut, received);

        CloseHandle(tFile);
    }
    else {
        printf("[!] Nope! Not this time, buddy!\n");
    }

    return 0;
}