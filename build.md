
# Building From Source

In the source directory, you may execute any of the following:

`make deps-apt` - installs the build dependencies required to compile the program.

`make` - creates an executable for x86_64 Linux.

`make clean` - deletes only the generated executable file created by only executing `make`.

`make clean-build` - deletes the generated build directory in it's entirety.

`make all` - **generate all of the following:**

### For Windows 95 OSR 2.5 and above, Pentium CPU minimum (32 bit)

*   Windows i686 static executable file
*   Portable Windows i386 release .zip file

### For Windows x86_64 (64 bit)

*   Windows x86_64 static executable file
*   Portable Windows x86_64 release .zip file

### For Linux 3.2.0 and above, 386 CPU minimum (32 bit)

*   Linux i386 static executable file
*   Portable Linux i386 release .zip file
*   Linux i386 release .deb file

### For Linux 3.2.0 and above, x86_64 (64 bit)

*   Linux x86_64 static executable file
*   Portable Linux x86_64 release .zip file
*   Linux x86_64 release .deb file

All output is found in the build directory created in the source directory.