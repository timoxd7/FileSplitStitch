#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "split.hpp"
#include "stitch.hpp"

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf(
            "Wrong command. Use\n\t--split to split a file\n\t--stitch to stitch multiple split "
            "flies together\n");
        return 0;
    }

    if (!strcmp(argv[1], "--split")) {
        return split(argc, argv);
    } else if (!strcmp(argv[1], "--stitch")) {
        return stitch(argc, argv);
    }

    return 0;
}
