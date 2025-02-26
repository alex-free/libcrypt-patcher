# This GNUMakefile is part of EzRe v1.0.7, see https://github.com/alex-free/ezre

include variables.mk

# Define VERSION in source files.
VERSION:=v$(VERSION)
COMPILER_FLAGS+=-DVERSION=\""$(VERSION)"\"
COMPILER_FLAGS_MAC+=-DVERSION=\""$(VERSION)"\"

$(PROGRAM): clean

# Check release name info.

ifeq ($(strip $(PROGRAM)),)
	$(error Error: The $$PROGRAM variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(SOURCE_FILES)),)
	$(error Error: The $$SOURCE_FILES variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(RELEASE_BASE_NAME)),)
	$(error Error: The $$RELEASE_BASE_NAME variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(VERSION)),)
	$(error Error: The $$VERSION variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(LINUX_I386_RELEASE_NAME_SUFFIX)),)
	$(error Error: The $$LINUX_I386_RELEASE_NAME_SUFFIX variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(LINUX_X86_64_RELEASE_NAME_SUFFIX)),)
	$(error Error: The $$LINUX_X86_64_RELEASE_NAME_SUFFIX variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_I686_RELEASE_NAME_SUFFIX)),)
	$(error Error: The $$WINDOWS_I686_RELEASE_NAME_SUFFIX variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_X86_64_RELEASE_NAME_SUFFIX)),)
	$(error Error: The $$WINDOWS_X86_64_RELEASE_NAME_SUFFIX variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(MAC_OS_RELEASE_NAME_SUFFIX)),)
	$(error Error: The $$MAC_OS_RELEASE_NAME_SUFFIX variable is not set in variables.mk but is required)
endif

# Check dependencies.

ifeq ($(strip $(BUILD_DEPENDS_MACPORTS)),)
	$(error Error: The $$BUILD_DEPENDS_MACPORTS variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(BUILD_DEPENDS_APT)),)
	$(error Error: The $$BUILD_DEPENDS_APT variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(BUILD_DEPENDS_DNF)),)
	$(error Error: The $$BUILD_DEPENDS_DNF variable is not set in variables.mk but is required)
endif

# Check build output.

ifeq ($(strip $(BUILD_DIR)),)
	$(error Error: The $$BUILD_DIR variable is not set in variables.mk but is required)
endif

# Check for library compilation.

ifeq ($(strip $(BUILD_LIB)),)
	$(error Error: The $$BUILD_LIB variable is not set in variables.mk but is required)
endif

# Check compiler.

ifeq ($(strip $(COMPILER)),)
	$(error Error: The $$COMPILER variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_I686_COMPILER)),)
	$(error Error: The $$WINDOWS_I686_COMPILER variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_X86_64_COMPILER)),)
	$(error Error: The $$WINDOWS_X86_64_COMPILER variable is not set in variables.mk but is required)
endif

# Check compiler flags.

ifeq ($(strip $(COMPILER_FLAGS)),)
	$(error Error: The $$COMPILER_FLAGS variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(COMPILER_FLAGS_LINUX_I386)),)
	$(error Error: The $$COMPILER_FLAGS_LINUX_I386 variable is not set in variables.mk but is required)
endif

# Check ar.

ifeq ($(strip $(AR)),)
	$(error Error: The $$AR variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_I686_AR)),)
	$(error Error: The $$WINDOWS_I686_AR variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_X86_64_AR)),)
	$(error Error: The $$WINDOWS_X86_64_AR variable is not set in variables.mk but is required)
endif

# Check strip.

ifeq ($(strip $(STRIP)),)
	$(error Error: The $$STRIP variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_I686_STRIP)),)
	$(error Error: The $$WINDOWS_I686_STRIP variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(WINDOWS_X86_64_STRIP)),)
	$(error Error: The $$WINDOWS_X86_64_STRIP variable is not set in variables.mk but is required)
endif

# Begin actual default recipe.

	mkdir -p $(BUILD_DIR)

# OPTIONAL, not default: Build C libraries to be used by the target executable. To enable this, you must replace `library-file*` with the actual name of the library, and you must set `BUILD_LIB=YES` in `variables.mk`.

# Mac OS does not support static linking, so we remove the -static flag from $(COMPILER_FLAGS). We echo the command we are going to use since the if statement doesn't display it by itself cleanly. We only account for this when EXECUTABLE_NAME is not set to avoid doing that for mingw cross compiled builds.

# If BUILD_LIB=YES. 	
# If EXECUTABLE_NAME is provided
# If Mac OS
# else (BUILD_LIB=NO)
# If EXECUTABLE_NAME is provided
# If Mac OS
# End

ifeq ("$(BUILD_LIB)", "YES")
	# Compile library file into object and then create the static library
	$(COMPILER) $(COMPILER_FLAGS_LIB) -c library-files-dir/library-file.c -o $(BUILD_DIR)/library-file-object.o
	$(AR) rcs $(BUILD_DIR)/library-file-archive.a $(BUILD_DIR)/library-file-object.o

ifeq ($(strip $(EXECUTABLE_NAME)),)

	@if [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(WINDOWS_I686_COMPILER)" ] && [ "$(COMPILER)" != "$(WINDOWS_X86_64_COMPILER)" ]; then \
		echo "$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(PROGRAM)"; \
		$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(PROGRAM); \
	else \
		echo "$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(PROGRAM)"; \
		$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(PROGRAM); \
	fi

	$(STRIP) $(BUILD_DIR)/$(PROGRAM)
else

	@if [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(WINDOWS_I686_COMPILER)" ] && [ "$(COMPILER)" != "$(WINDOWS_X86_64_COMPILER)" ]; then \
		echo "$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	else \
		echo "$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -L./$(BUILD_DIR) -llibrary-file -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	fi

	$(STRIP) $(BUILD_DIR)/$(EXECUTABLE_NAME)
endif

else

ifeq ($(strip $(EXECUTABLE_NAME)),)

	@if [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(WINDOWS_I686_COMPILER)" ] && [ "$(COMPILER)" != "$(WINDOWS_X86_64_COMPILER)" ]; then \
		echo "$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -o $(BUILD_DIR)/$(PROGRAM)"; \
		$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -o $(BUILD_DIR)/$(PROGRAM); \
	else \
		echo "$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -o $(BUILD_DIR)/$(PROGRAM)"; \
		$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -o $(BUILD_DIR)/$(PROGRAM); \
	fi
	$(STRIP) $(BUILD_DIR)/$(PROGRAM)

else

	@if [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(WINDOWS_I686_COMPILER)" ] && [ "$(COMPILER)" != "$(WINDOWS_X86_64_COMPILER)" ]; then \
		echo "$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	else \
		echo "$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	fi

	$(STRIP) $(BUILD_DIR)/$(EXECUTABLE_NAME)
endif

endif

.PHONY: deps
deps:
	echo "Info: root privileges are required to install build dependencies.";
	@if [ "$(shell uname)" = "Darwin" ]; then \
		echo "Mac OS detected."; \
		if command -v port > /dev/null; then \
			echo "Using MacPorts"; \
			sudo port -N install $(BUILD_DEPENDS_MACPORTS); \
		else \
			echo "MacPorts is not installed"; \
		fi; \
	elif [ "$(shell uname)" = "Linux" ]; then \
		echo "Linux detected."; \
		if command -v dnf > /dev/null; then \
			echo "Using dnf"; \
			sudo dnf -y install $(BUILD_DEPENDS_DNF); \
		elif command -v apt > /dev/null; then \
			echo "Using apt"; \
			sudo apt install --yes $(BUILD_DEPENDS_APT); \
		else \
			echo "MacPorts, DNF, and APT package managers were not found. The make deps rule requires one of these package managers to automatically install all required build dependencies."; \
		fi; \
	fi

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/$(PROGRAM).exe $(BUILD_DIR)/$(PROGRAM) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.a

.PHONY: clean-build
clean-build:
	rm -rf $(BUILD_DIR)

.PHONY: linux-i386
linux-i386: clean
	make $(PROGRAM) COMPILER_FLAGS='$(COMPILER_FLAGS_LINUX_I386) $(COMPILER_FLAGS)' COMPILER_FLAGS_LIB='$(COMPILER_FLAGS_LINUX_I386) $(COMPILER_FLAGS_LIB)' EXECUTABLE_NAME='$(PROGRAM).i386'

.PHONY: linux-x86_64
linux-x86_64: clean
	make $(PROGRAM) EXECUTABLE_NAME='$(PROGRAM).x86_64'

.PHONY: windows-i686
windows-i686: clean
	make $(PROGRAM) COMPILER=$(WINDOWS_I686_COMPILER) EXECUTABLE_NAME='$(PROGRAM).i686.exe' AR=$(WINDOWS_I686_AR) STRIP=$(WINDOWS_I686_STRIP)

.PHONY: windows-x86_64
windows-x86_64: clean
	make $(PROGRAM) COMPILER=$(WINDOWS_X86_64_COMPILER) EXECUTABLE_NAME='$(PROGRAM).x86_64.exe' AR=$(WINDOWS_X86_64_AR) STRIP=$(WINDOWS_X86_64_STRIP)

.PHONY: mac-os
mac-os: clean
	make $(PROGRAM) EXECUTABLE_NAME='$(PROGRAM).$(shell uname -m)'

.PHONY: release
release:
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM) $(BUILD_DIR)/$(PROGRAM)-$(VERSION)-$(PLATFORM).zip
	mkdir $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM)
ifeq ($(strip $(WINDOWS_RELEASE)),)
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) $(BUILD_DIR)/$(PROGRAM)
	cp -r $(BUILD_DIR)/$(PROGRAM) $(RELEASE_FILES) $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM)
else
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) $(BUILD_DIR)/$(PROGRAM).exe
	cp -r $(BUILD_DIR)/$(PROGRAM).exe $(RELEASE_FILES) $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM)
endif
	chmod -R 777 $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM)
	cd $(BUILD_DIR) && zip -rq $(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM).zip $(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM)
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(PLATFORM)

.PHONY: linux-i386-release
linux-i386-release: linux-i386
ifeq ($(strip $(LINUX_SPECIFIC_RELEASE_FILES)),)
	make release PLATFORM='$(LINUX_I386_RELEASE_NAME_SUFFIX)' EXECUTABLE_NAME='$(PROGRAM).i386'
else
	make release PLATFORM='$(LINUX_I386_RELEASE_NAME_SUFFIX)' RELEASE_FILES='$(LINUX_SPECIFIC_RELEASE_FILES) $(RELEASE_FILES)' EXECUTABLE_NAME='$(PROGRAM).i386'
endif

.PHONY: linux-x86_64-release
linux-x86_64-release: linux-x86_64
ifeq ($(strip $(LINUX_SPECIFIC_RELEASE_FILES)),)
	make release PLATFORM='$(LINUX_X86_64_RELEASE_NAME_SUFFIX)' EXECUTABLE_NAME='$(PROGRAM).x86_64'
else
	make release PLATFORM='$(LINUX_X86_64_RELEASE_NAME_SUFFIX)' RELEASE_FILES='$(LINUX_SPECIFIC_RELEASE_FILES) $(RELEASE_FILES)' EXECUTABLE_NAME='$(PROGRAM).x86_64'
endif

.PHONY: windows-i686-release
windows-i686-release: windows-i686
ifeq ($(strip $(WINDOWS_SPECIFIC_RELEASE_FILES)),)
	make release PLATFORM='$(WINDOWS_I686_RELEASE_NAME_SUFFIX)' EXECUTABLE_NAME='$(PROGRAM).i686.exe' WINDOWS_RELEASE=true
else
	make release PLATFORM='$(WINDOWS_I686_RELEASE_NAME_SUFFIX)' RELEASE_FILES='$(WINDOWS_SPECIFIC_RELEASE_FILES) $(RELEASE_FILES)' EXECUTABLE_NAME='$(PROGRAM).i686.exe' WINDOWS_RELEASE=true
endif

.PHONY: windows-x86_64-release
windows-x86_64-release: windows-x86_64
ifeq ($(strip $(WINDOWS_SPECIFIC_RELEASE_FILES)),)
	make release PLATFORM='$(WINDOWS_X86_64_RELEASE_NAME_SUFFIX)' EXECUTABLE_NAME='$(PROGRAM).x86_64.exe' WINDOWS_RELEASE=true
else
	make release PLATFORM='$(WINDOWS_X86_64_RELEASE_NAME_SUFFIX)' RELEASE_FILES='$(WINDOWS_SPECIFIC_RELEASE_FILES) $(RELEASE_FILES)' EXECUTABLE_NAME='$(PROGRAM).x86_64.exe' WINDOWS_RELEASE=true
endif

.PHONY: mac-os-release
mac-os-release: mac-os
ifeq ($(strip $(MAC_OS_SPECIFIC_RELEASE_FILES)),)
	make release PLATFORM='$(MAC_OS_RELEASE_NAME_SUFFIX)' EXECUTABLE_NAME='$(PROGRAM).$(shell uname -m)'
else
	make release PLATFORM='$(MAC_OS_RELEASE_NAME_SUFFIX)' RELEASE_FILES='$(MAC_OS_SPECIFIC_RELEASE_FILES) $(RELEASE_FILES)' EXECUTABLE_NAME='$(PROGRAM).mac-os'
endif

.PHONY: linux-i386-deb
linux-i386-deb: linux-i386
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)/usr/bin
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)/DEBIAN
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) $(BUILD_DIR)/$(PROGRAM)
	cp $(BUILD_DIR)/$(PROGRAM) $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)/usr/bin
	cp control-i386 $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_I386_RELEASE_NAME_SUFFIX)

.PHONY: linux-x86_64-deb
linux-x86_64-deb: linux-x86_64
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)/usr/bin
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)/DEBIAN
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) $(BUILD_DIR)/$(PROGRAM)
	cp $(BUILD_DIR)/$(PROGRAM) $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)/usr/bin
	cp control-x86_64 $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(LINUX_X86_64_RELEASE_NAME_SUFFIX)

.PHONY: linux-i386-rpm
linux-i386-rpm: linux-i386
	rm -rf rpm-tmp
	mkdir -p rpm-tmp/RPMS rpm-tmp/SPECS rpm-tmp/SOURCES rpm-tmp/BUILD
	cp ezre.spec rpm-tmp/SPECS/ezre.spec
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) rpm-tmp/SOURCES/$(PROGRAM)
	ls rpm-tmp/SOURCES/
	rpmbuild -bb --target i386 rpm-tmp/SPECS/ezre.spec --define "_topdir $(shell pwd)/rpm-tmp"
	ls rpm-tmp/RPMS
	mv rpm-tmp/RPMS/i386/*.rpm build/
	rm -rf rpm-tmp

.PHONY: linux-x86_64-rpm
linux-x86_64-rpm: linux-x86_64
	rm -rf rpm-tmp
	mkdir -p rpm-tmp/RPMS rpm-tmp/SPECS rpm-tmp/SOURCES rpm-tmp/BUILD
	cp ezre.spec rpm-tmp/SPECS/ezre.spec
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) rpm-tmp/SOURCES/$(PROGRAM)
	rpmbuild -bb --target x86_64 rpm-tmp/SPECS/ezre.spec --define "_topdir $(shell pwd)/rpm-tmp"
	mv rpm-tmp/RPMS/x86_64/*.rpm build/
	rm -rf rpm-tmp

.PHONY: all
all:
	make clean-build
	
	@if [ "$(shell uname)" = "Linux" ]; then \
		make linux-i386-release; \
		make linux-i386-deb EXECUTABLE_NAME='$(PROGRAM).i386'; \
		make linux-i386-rpm EXECUTABLE_NAME='$(PROGRAM).i386'; \
		make linux-x86_64-release; \
		make linux-x86_64-deb EXECUTABLE_NAME='$(PROGRAM).x86_64'; \
		make linux-x86_64-rpm EXECUTABLE_NAME='$(PROGRAM).x86_64'; \
	elif [ "$(shell uname)" = "Darwin" ]; then \
		make mac-os-release; \
	fi
	
	make windows-i686-release
	make windows-x86_64-release
	
	make clean
