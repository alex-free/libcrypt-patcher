# lcp GNUMakefile by Alex Free
PROGRAM=lcp
RELEASE_NAME=libcrypt-patcher
VERSION=v1.0.3
CC=gcc
C_FLAGS=-Wall -Werror -Os -static -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -DVERSION=\"$(VERSION)\"
RELEASE_FILES=images readme.md changelog.md license.txt

$(PROGRAM): clean
	$(CC) $(C_FLAGS) $(PROGRAM).c -o $(PROGRAM)

clean:
	rm -rf $(PROGRAM).exe $(PROGRAM)

linux-x86: clean
	make $(PROGRAM) C_FLAGS='-m32 $(C_FLAGS)'

linux-x86_64: clean
	make $(PROGRAM)

windows-x86: clean
	make $(PROGRAM) CC="i686-w64-mingw32-gcc"

windows-x86_64: clean
	make $(PROGRAM) CC="x86_64-w64-mingw32-gcc"

linux-release:
	rm -rf $(RELEASE_NAME)-$(VERSION)-$(PLATFORM) $(PROGRAM)-$(VERSION)-$(PLATFORM).zip
	mkdir $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	cp -rv $(PROGRAM) $(RELEASE_FILES) $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	chmod -R 777 $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	zip -r $(RELEASE_NAME)-$(VERSION)-$(PLATFORM).zip $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	rm -rf $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)

windows-release:
	rm -rf $(RELEASE_NAME)-$(VERSION)-$(PLATFORM) $(PROGRAM)-$(VERSION)-$(PLATFORM).zip
	mkdir $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	cp -rv $(PROGRAM).exe $(RELEASE_FILES) $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	chmod -R 777 $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	zip -r $(RELEASE_NAME)-$(VERSION)-$(PLATFORM).zip $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)
	rm -rf $(RELEASE_NAME)-$(VERSION)-$(PLATFORM)

linux-x86-release: linux-x86
	make linux-release PLATFORM=linux-x86-static

linux-x86_64-release: linux-x86_64
	make linux-release PLATFORM=linux-x86_64-static

linux-x86-deb: linux-x86
	rm -rf $(RELEASE_NAME)-$(VERSION)-linux-x86-static
	mkdir -p $(RELEASE_NAME)-$(VERSION)-linux-x86-static/usr/bin
	mkdir -p $(RELEASE_NAME)-$(VERSION)-linux-x86-static/DEBIAN
	cp lcp $(RELEASE_NAME)-$(VERSION)-linux-x86-static/usr/bin
	cp control-x86 $(RELEASE_NAME)-$(VERSION)-linux-x86-static/DEBIAN/control
	dpkg-deb --build $(RELEASE_NAME)-$(VERSION)-linux-x86-static
	rm -rf $(RELEASE_NAME)-$(VERSION)-linux-x86-static

linux-x86_64-deb: linux-x86_64
	rm -rf $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static
	mkdir -p $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static/usr/bin
	mkdir -p $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static/DEBIAN
	cp lcp $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static/usr/bin
	cp control-x86_64 $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static/DEBIAN/control
	dpkg-deb --build $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static
	rm -rf $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static

windows-x86-release: windows-x86
	make windows-release PLATFORM=windows-x86

windows-x86_64-release: windows-x86_64
	make windows-release PLATFORM=windows-x86_64

clean-build: clean
	rm -rf *.zip $(RELEASE_NAME)-$(VERSION)-linux-x86-static.deb $(RELEASE_NAME)-$(VERSION)-linux-x86-static $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static.deb $(RELEASE_NAME)-$(VERSION)-linux-x86_64-static

all: clean-build linux-x86-release linux-x86-deb linux-x86_64-release linux-x86_64-deb windows-x86-release windows-x86_64-release 
