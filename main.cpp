#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_CHUNK_SIZE 4000

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf(
            "Wrong command. Use\n\t--split to split a file\n\t--stitch to stitch multiple split "
            "flies together\n");
        return 0;
    }

    if (!strcmp(argv[1], "--split")) {
        printf("Not implemented yet...\n");
        return 0;
    } else if (!strcmp(argv[1], "--stitch")) {
        if (argc <= 2) {
            printf(
                "Add the filepath to the end of the command (base filename, without the number at "
                "the end!)\n");
            return 0;
        }

        printf("Stitching %s...\n", argv[2]);

        // -> Stitch together
        char filenameBuffer[1024];
        uint32_t filecount = 0;

        FILE* outputFilePtr = fopen(argv[2], "wb");

        if (outputFilePtr == nullptr) {
            printf("Can't open output file\n");
            return 0;
        }

        uint8_t writeBuffer[READ_CHUNK_SIZE];

        while (true) {
            sprintf(filenameBuffer, "%s.%i", argv[2], filecount);

            FILE* filePtr = fopen(filenameBuffer, "rb");

            if (filePtr == nullptr) {
                if (filecount == 0) {
                    printf("No files found!\n");
                }
                break;
            } else {
                printf("%s...\n", filenameBuffer);

                // -> Read in chunks until end
                while (true) {
                    size_t gotBytes = fread(writeBuffer, 1, READ_CHUNK_SIZE, filePtr);

                    bool eof = false;
                    if (feof(filePtr)) {
                        eof = true;

                        if (ferror(filePtr)) {
                            printf("Error while reading\n");
                            fclose(filePtr);
                            goto exit;
                        }
                    }

                    if (gotBytes != READ_CHUNK_SIZE && !eof) {
                        printf("Got gotBytes missmatch without EOF\n");
                    }

                    size_t writtenBytes = fwrite(writeBuffer, 1, gotBytes, outputFilePtr);

                    if (gotBytes != writtenBytes) {
                        printf("Written Bytes missmatch: %i : %i\n", writtenBytes, gotBytes);

                        uint8_t* movedPtr = writeBuffer + writtenBytes;

                        size_t newlyWrittenBytes =
                            fwrite(writeBuffer, 1, gotBytes - writtenBytes, outputFilePtr);

                        if (newlyWrittenBytes + writtenBytes != gotBytes) {
                            printf("Written Bytes missmatch again! %i : %i\nTerminating now...\n",
                                   newlyWrittenBytes + writtenBytes, gotBytes);

                            fclose(filePtr);
                            goto exit;
                        }

                        printf("Recovered missmatch :)\n");
                    }

                    if (eof) {
                        break;
                    }
                }

                ++filecount;
                fclose(filePtr);
            }
        }

    exit:
        fclose(outputFilePtr);
        printf("Done!\n");
        return 1;
    }
}
