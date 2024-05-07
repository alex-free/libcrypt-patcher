# Variables for EzRe Makefile

# Basename of all release files (.zip, .deb)
RELEASE_BASE_NAME=libcrypt-patcher
# Version number, passed as 'VERSION' string to $(SOURCE_FILES)
VERSION=v1.0.7
# Appeneded to end of release file name
LINUX_I386_RELEASE_NAME_SUFFIX=linux-i386-static
LINUX_X86_64_RELEASE_NAME_SUFFIX=linux-x86_64-static
WINDOWS_I686_RELEASE_NAME_SUFFIX=windows-i686-static
WINDOWS_X86_64_RELEASE_NAME_SUFFIX=windows-x86_64-static
# Release file format is $(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX)

# Files included in all portable releases (.zip)
RELEASE_FILES=images readme.md changelog.md license.md
# OPTIONAL: files included only in the Linux portable releases (.zip)
LINUX_SPECIFIC_RELEASE_FILES=
# OPTIONAL: files included only in the Windows portable releases (.zip)
WINDOWS_SPECIFIC_RELEASE_FILES=

# All dependencies required to build the software, to be installed when using deps-apt EzRe Makefile rule (For Debian/Ubuntu)
BUILD_DEPENDS_APT=build-essential gcc-multilib mingw-w64-tools zip dpkg-dev

# Executable name in release (.exe file extension is appended for Windows builds)
PROGRAM=lcp
# Source files to be compiled into $(PROGRAM) target
SOURCE_FILES=lcp.c
# Compiler flags used to compile $(SOURCE_FILES)
COMPILER_FLAGS=-Wall -Werror -Os -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -static
# Compiler flag appended to $(COMPILER_FLAGS) to compile $(SOURCE_FILES) for Linux x86 builds
COMPILER_FLAGS_LINUX_X86=-m32
# Create builds in this directory relative to $(SOURCE_FILES)
BUILD_DIR=build

# Linux Compiler For i386 and x86_64
LINUX_COMPILER=gcc
# Windows Cross Compiler For i686
WINDOWS_X86_COMPILER=i686-w64-mingw32-gcc
# Windows Cross Compiler For x86_64
WINDOWS_X86_64_COMPILER=x86_64-w64-mingw32-gcc
# Host system compiler
COMPILER=$(LINUX_COMPILER)
