
# Building From Source

This software is built with the [EZRE](https://github.com/alex-free/ezre) build system. In the source directory, you may execute any of the following:

`make deps` - installs the build dependencies required to compile the program on x86_64 Linux or Mac OS. The host Linux distribution must have either the `dnf` or `apt` package manager for this to work. Mac OS hosts must have MacPorts installed.

`make` - creates an executable for the host system.

`make LEGACY=TRUE` - creates an executable using alternative flags, only for Mac OS hosts.

`make clean` - deletes only the generated executable file created by only executing `make`.

`make windows-i686-release` - generate a portable Windows i686 release .zip file for Windows 95 OSR 2.5 and above. Pentium or newer required.

`make windows-x86_64-release` - generate a portable Windows x86_64 release .zip file for all 64 bit Windows versions.

`make mac-os-release` - generate a portable Mac OS release .zip file (your current architecture by default). In general any newer Mac OS version that is the same as yours or newer will work with the built executable, with 2 executions. Mac OS 10.15 and newer can't run support x86 32 bit executables. Mac OS X 10.7 and newer can not run PowerPC executables. 

`make mac-os-release LEGACY=TRUE` - generate a portable Mac OS release .zip file (using alternative flags, intended for allowing for a second alternative Mac build). In general any newer Mac OS version that is the same as the target of the LEGACY alt flags or newer will work with the built executable, with 2 executions. Mac OS 10.15 and newer can't run support x86 32 bit executables. Mac OS X 10.7 and newer can not run PowerPC executables. 

`make mac-os-release LEGACY=TRUE` - generate a portable Mac OS release .zip file (same as above can apply alternative values for i.e. PowerPC and or older Mac OS versions).

`make host-32-release` - generate a portable Linux i386 release .zip file for kernel version 3.2.0 and newer.

`make host-32-deb` - generate a Linux i386 release deb file for kernel version 3.2.0 and newer.

`make host-32-rpm` - generate a Linux i386 release rpm file for kernel version 3.2.0 and newer.

`make host-64-release` - generate a portable Linux x86_64 release .zip file for kernel version 3.2.0 and newer.

`make host-64-deb` - generate a Linux x86_64 release deb file for kernel version 3.2.0 and newer.

`make host-64-rpm` - generate a Linux x86_64 release rpm file for kernel version 3.2.0 and newer.

`make clean-build` - deletes the generated build directory in it's entirety, and therefore, all builds generated.

`make all` - generate all builds/release files. Mac OS hosts don't generate Linux builds and Linux hosts don't generate Mac OS builds.

All output is found in the `build` directory created in the source directory.
