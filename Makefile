# lcp GNUMakefile by Alex Free
PROGRAM=lcp
VERSION=1.0.2
CC=gcc
C_FLAGS=-Wall -Werror -Os -static -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -DVERSION=\"$(VERSION)\"
RELEASE_FILES=images readme.md changelog.md license.txt

$(PROGRAM): clean
	$(CC) $(C_FLAGS) $(PROGRAM).c -o $(PROGRAM)

clean:
	rm -rf $(PROGRAM).exe $(PROGRAM)

linux-x86:
	make $(PROGRAM) C_FLAGS='-m32 $(C_FLAGS)'

linux-x86_64:
	make $(PROGRAM)

windows-x86:
	make $(PROGRAM) CC="i686-w64-mingw32-gcc"

windows-x86_64:
	make $(PROGRAM) CC="x86_64-w64-mingw32-gcc"

linux-release:
	rm -rf $(PROGRAM)-v$(VERSION)-$(PLATFORM) $(PROGRAM)-v$(VERSION)-$(PLATFORM).zip
	mkdir $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	cp -rv $(PROGRAM) $(RELEASE_FILES) $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	chmod -R 777 $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	zip -r $(PROGRAM)-v$(VERSION)-$(PLATFORM).zip $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	rm -rf $(PROGRAM)-v$(VERSION)-$(PLATFORM)

windows-release:
	rm -rf $(PROGRAM)-v$(VERSION)-$(PLATFORM) $(PROGRAM)-v$(VERSION)-$(PLATFORM).zip
	mkdir $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	cp -rv $(PROGRAM).exe $(RELEASE_FILES) $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	chmod -R 777 $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	zip -r $(PROGRAM)-v$(VERSION)-$(PLATFORM).zip $(PROGRAM)-v$(VERSION)-$(PLATFORM)
	rm -rf $(PROGRAM)-v$(VERSION)-$(PLATFORM)

linux-x86-release: linux-x86
	make linux-release PLATFORM=linux-x86_static

linux-x86_64-release: linux-x86_64
	make linux-release PLATFORM=linux-x86_64_static

linux-x86_64-deb: linux-x86_64
	rm -rf libcrypt-patcher-$(VERSION)-amd64
	mkdir -p libcrypt-patcher-$(VERSION)-amd64/usr/bin
	mkdir -p libcrypt-patcher-$(VERSION)-amd64/DEBIAN
	cp lcp libcrypt-patcher-$(VERSION)-amd64/usr/bin
	cp control libcrypt-patcher-$(VERSION)-amd64/DEBIAN
	dpkg-deb --build libcrypt-patcher-$(VERSION)-amd64

windows-x86-release: windows-x86
	make windows-release PLATFORM=windows-x86

windows-x86_64-release: windows-x86_64
	make windows-release PLATFORM=windows-x86_64

clean-build: clean
	rm -rf *.zip libcrypt-patcher-1.0.2-amd64 libcrypt-patcher-1.0.2-amd64.deb

all: linux-x86-release linux-x86_64-release linux-x86_64-deb windows-x86-release windows-x86_64-release libcrypt-patcher-$(VERSION)-amd64 libcrypt-patcher-$(VERSION)-amd64.deb
