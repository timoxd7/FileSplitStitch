# File Stitcher / Splitter

This simple C++ program can split and stitch files together.

It is developed to stitch .ISO files together produced by a PS3 when copying on a FAT32 drive.

## Usage

Run the tool with the --stitch or --split parameter. Then add the Filepath.

For Example:

    FileSplitStitch.exe --split "my-fancy-game.ISO"

Will produce some Files with the name *my-fancy-game.ISO.0* with the number of the file at the end.

    FileSplitStitch.exe --stitch "my-fancy-game.ISO"

Will search for *my-fancy-game.ISO.0* and so on and stitch together.
