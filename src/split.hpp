#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define WRITE_CHUNK_SIZE 4000
#define MAX_FILESIZE 4293918720   // Bytes max per file

#define min(a, b) (a < b ? a : b)

int split(int argc, char** argv) {
    if (argc <= 2) {
        printf(
            "Add the filepath to the end of the command (base filename, without the number at "
            "the end!)\n");
        return 0;
    }

    printf("Splitting %s...\n", argv[2]);

    FILE* inputFile = fopen(argv[2], "rb");

    if (inputFile == nullptr) {
        printf("Can't open input file\n");
        return 0;
    }

    char filenameBuffer[1024];
    uint32_t filecount = 0;

    uint8_t writeBuffer[WRITE_CHUNK_SIZE];

    bool eof = false;
    while (!eof) {
        sprintf(filenameBuffer, "%s.%i", argv[2], filecount);

        FILE* filePtr = fopen(filenameBuffer, "wb");

        if (filePtr == nullptr) {
            if (filecount == 0) {
                printf("Can't open output file\n");
                break;
            }
        }

        printf("%s...\n", filenameBuffer);

        size_t leftFilesize = MAX_FILESIZE;

        while (leftFilesize && !eof) {
            size_t bytesToRead = min(WRITE_CHUNK_SIZE, leftFilesize);

            size_t gotBytes = fread(writeBuffer, 1, bytesToRead, inputFile);

            if (ferror(inputFile)) {
                printf("Read error\n");
                return 0;
            }

            if (feof(inputFile)) {
                eof = true;
            }

            if (gotBytes != bytesToRead) {
                if (!eof) {
                    printf("Error while reading (size missmatch)! %i : %i\n", gotBytes,
                           bytesToRead);
                    fclose(filePtr);
                    goto exit;
                }
            }

            size_t writtenBytes = fwrite(writeBuffer, 1, gotBytes, filePtr);

            if (writtenBytes != gotBytes) {
                printf("Write error! Size missmatch: %i : %i\n", writtenBytes, gotBytes);
                fclose(filePtr);
                goto exit;
            }

            leftFilesize -= writtenBytes;
        }

        fclose(filePtr);
        ++filecount;
    }

    printf("Done!\n");

exit:
    fclose(inputFile);
    return 0;
}
