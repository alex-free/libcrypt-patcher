# EzRe GNUMakefile Variables

# REQUIRED: executable name in release (.exe file extension is appended for Windows builds). I.e. hello.
PROGRAM=lcp
# REQUIRED: source files to be compiled into $(PROGRAM) target. Can use wildcard (i.e. *.c, *.cpp, etc) or specify files specifically. These files are looked for in the same directory that the EZRE `Makefile` and `variables.mk` files are in (relative).
SOURCE_FILES=lcp.c
# REQUIRED: Basename of all release files (.zip, .deb). I.e. hello-world.
RELEASE_BASE_NAME=libcrypt-patcher
# REQUIRED: Version number, passed as 'VERSION' string to $(SOURCE_FILES). I.e. 1.0. Prepended with a v.
VERSION=1.0.9

# OPTIONAL: additional files included in all portable zip releases. I.e. readme.md.
RELEASE_FILES=*.md images
# OPTIONAL: files included only in the Linux portable releases (.zip).
LINUX_SPECIFIC_RELEASE_FILES=
# OPTIONAL: files included only in the Windows portable releases (.zip).
WINDOWS_SPECIFIC_RELEASE_FILES=

# All dependencies required to build the software, EzRe style.
# For APT:
BUILD_DEPENDS_APT=build-essential g++-multilib gcc-multilib mingw-w64-tools g++-mingw-w64 zip dpkg-dev rpm
# For DNF:
BUILD_DEPENDS_DNF=gcc g++ libstdc++-static.i686 glibc-static.i686 libstdc++-static.x86_64 mingw64-gcc mingw32-gcc mingw32-gcc-c++ mingw64-gcc-c++ zip
# For MacPorts (Note we do also have these: rpm dpkg but no cross compiler for linux via MacPorts...):
BUILD_DEPENDS_MACPORTS=mingw-w64

# REQUIRED: Appended to end of release file name. Release file format is $(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX).
LINUX_I386_RELEASE_NAME_SUFFIX=linux-i386-static
LINUX_X86_64_RELEASE_NAME_SUFFIX=linux-x86_64-static
WINDOWS_I686_RELEASE_NAME_SUFFIX=windows-i686-static
WINDOWS_X86_64_RELEASE_NAME_SUFFIX=windows-x86_64-static
MAC_OS_RELEASE_NAME_SUFFIX=mac-os-$(shell uname -m)

# REQUIRED: Host system compiler. gcc for c. g++ for C++.
COMPILER=gcc
# REQUIRED: Windows Cross Compiler For i686.  i686-w64-mingw32-gcc for C. i686-w64-mingw32-g++ for C++
WINDOWS_I686_COMPILER=i686-w64-mingw32-$(COMPILER)
# REQUIRED: Windows Cross Compiler For x86_64. x86_64-w64-mingw32-gcc for C. x86_64-w64-mingw32-g++ for C++.
WINDOWS_X86_64_COMPILER=x86_64-w64-mingw32-$(COMPILER)

# REQUIRED: Host system ar
AR=ar
# REQUIRED: Windows i686 AR command (for building libraries with EZRE used by the target program).
WINDOWS_I686_AR=i686-w64-mingw32-ar
# REQUIRED: Windows x86_64 AR command (for building libraries with EZRE used by the target program).
WINDOWS_X86_64_AR=x86_64-w64-mingw32-ar

# REQUIRED: Host system strip.
STRIP=strip
# REQUIRED: Windows i686 strip command (for building libraries with EZRE used by the target program).
WINDOWS_I686_STRIP=i686-w64-mingw32-strip
# REQUIRED: Windows x86_64 strip command (for building libraries with EZRE used by the target program).
WINDOWS_X86_64_STRIP=x86_64-w64-mingw32-strip

# REQUIRED: compiler flags used to compile $(SOURCE_FILES). To make a C/C++ program portable, you probably at least want `-static` as shown below. I like using `-Wall -Wextra -Werror -pedantic -static` or some variation. We can't use `-static` on Mac OS though.
COMPILER_FLAGS_MAC=-Wall -Wextra -Werror -pedantic -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
COMPILER_FLAGS=-Wall -Wextra -Werror -pedantic -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -static
# REQUIRED: compiler flag appended to $(COMPILER_FLAGS) to compile $(SOURCE_FILES) for Linux x86 builds. This tells GCC to build i386 code on an x86_64 system.
COMPILER_FLAGS_LINUX_I386=-m32

# REQUIRED: set to `YES` to build additional libraries (must edit Makefile with relevant info). By default this is set to `NO`.
BUILD_LIB=NO
# OPTIONAL: You may compile a library with different CFLAGS set here. (i.e. `-Wall -Wextra -Werror -pedantic -Wno-unused-function`)
COMPILER_FLAGS_LIB=-Wall -Wextra -Werror -pedantic -Wno-unused-function

# REQUIRED: create builds in this directory relative to $(SOURCE_FILES). THIS DIRECTORY WILL BE DELETED WHEN EXECUTING `make clean-build` SO BE EXTREMELY CAREFUL WITH WHAT YOU SET THIS TOO.
BUILD_DIR=build