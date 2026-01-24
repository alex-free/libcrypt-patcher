# This GNUMakefile is part of the EzRe build system v1.1.6.
# https://github.com/alex-free/ezre

include variables.mk

# Define VERSION in source files.
VERSION:=v$(VERSION)

# This is changed by *release rules.
EXECUTABLE_NAME := $(PROGRAM)

# COMPILER is only set by *release rules. we set it to $(COMPILER_HOST) for checking against other COMPILER_* values but don't actually use it otherwise for anything else.
ifeq ($(strip $(COMPILER)),)
	COMPILER := $(COMPILER_HOST)
endif

# Append DEFINE VERSION flag. We do the others later after we build them up a bit more.
COMPILER_FLAGS_HOST := $(COMPILER_FLAGS_HOST) -DVERSION=\"$(VERSION)\"
COMPILER_FLAGS_HOST_32_BIT := $(COMPILER_FLAGS_HOST_32_BIT) -DVERSION=\"$(VERSION)\"
COMPILER_FLAGS_MAC := $(COMPILER_FLAGS_MAC) -DVERSION=\"$(VERSION)\"

# Optionals that need to be set to something (all others may be empty).

# If $(COMPILER_32_BIT) is not set, set it to $(COMPILER_HOST).
ifeq ($(strip $(COMPILER_HOST_32_BIT)),)
	COMPILER_HOST_32_BIT := $(COMPILER_HOST)
endif

# If $(COMPILER_MAC) is not set, set it to $(COMPILER_HOST).
ifeq ($(strip $(COMPILER_MAC)),)
	COMPILER_MAC := $(COMPILER_HOST)
endif

# If $(COMPILER_MAC_LEGACY) is not set, set it to $(COMPILER_MAC).
ifeq ($(strip $(COMPILER_MAC_LEGACY)),)
	COMPILER_MAC_LEGACY := $(COMPILER_MAC)
endif

# If $(COMPILER_FLAGS_MAC_LEGACY) is not set, set it to $(COMPILER_FLAGS_MAC). If we do have custom COMPILER_FLAGS_MAC_LEGACY we need to appened the DEFINE VERSION flag here now.
ifeq ($(strip $(COMPILER_FLAGS_MAC_LEGACY)),)
	COMPILER_FLAGS_MAC_LEGACY := $(COMPILER_FLAGS_MAC)
else
	COMPILER_FLAGS_MAC_LEGACY := $(COMPILER_FLAGS_MAC_LEGACY) -DVERSION=\"$(VERSION)\"
endif

# If $(COMPILER_FLAGS_WINDOWS_I686) is not set, set it to $(COMPILER_FLAGS_HOST). If we do have custom COMPILER_FLAGS_WINDOWS_I686 we need to appened the DEFINE VERSION flag here now.
ifeq ($(strip $(COMPILER_FLAGS_WINDOWS_I686)),)
	COMPILER_FLAGS_WINDOWS_I686 := $(COMPILER_FLAGS_HOST)
else
	COMPILER_FLAGS_WINDOWS_I686 := $(COMPILER_FLAGS_WINDOWS_I686) -DVERSION=\"$(VERSION)\"
endif

# If $(COMPILER_FLAGS_WINDOWS_X86_64) is not set, set it to $(COMPILER_FLAGS_HOST). If we do have custom COMPILER_FLAGS_WINDOWS_X86_64 we need to appened the DEFINE VERSION flag here now.
ifeq ($(strip $(COMPILER_FLAGS_WINDOWS_X86_64)),)
	COMPILER_FLAGS_WINDOWS_X86_64 := $(COMPILER_FLAGS_HOST)
else
	COMPILER_FLAGS_WINDOWS_X86_64 := $(COMPILER_FLAGS_HOST) -DVERSION=\"$(VERSION)\"
endif

# If $(STRIP_HOST_32_BIT) is not set, set it to $(STRIP_HOST).
ifeq ($(strip $(STRIP_HOST_32_BIT)),)
	STRIP_HOST_32_BIT := $(STRIP_HOST)
endif

# If $(STRIP_MAC) is not set, set it to $(STRIP_HOST).
ifeq ($(strip $(STRIP_MAC)),)
	STRIP_MAC := $(STRIP_HOST)
endif

# If $(STRIP_MAC_LEGACY) is not set, set it to $(STRIP_MAC).
ifeq ($(strip $(STRIP_MAC_LEGACY)),)
	STRIP_MAC_LEGACY := $(STRIP_MAC)
endif

# Go through all optional scripts, if any are not empty enable them for target compilation. If they are not enabled we simply set a "echo script not enabled" command to each disabled target script to prevent syntax errors with GNUMake from an unset variable. User never actually sees that!

# Host
SHELL_SCRIPT_HOST_ENABLE := FALSE

ifneq ($(strip $(SHELL_SCRIPT_HOST)),)
	SHELL_SCRIPT_HOST_ENABLE := TRUE
else
	SHELL_SCRIPT_HOST := "echo script not enabled"
endif

# Host 32 bit
SHELL_SCRIPT_HOST_32_BIT_ENABLE := FALSE

ifneq ($(strip $(SHELL_SCRIPT_HOST_32_BIT)),)
	SHELL_SCRIPT_HOST_32_BIT_ENABLE := TRUE
else
	SHELL_SCRIPT_HOST_32_BIT := "echo script not enabled"
endif

# Mac
SHELL_SCRIPT_MAC_ENABLE := FALSE

ifneq ($(strip $(SHELL_SCRIPT_MAC)),)
	SHELL_SCRIPT_MAC_ENABLE := TRUE
else
	SHELL_SCRIPT_MAC := "echo script not enabled"
endif

# Mac legacy
SHELL_SCRIPT_MAC_LEGACY_ENABLE := FALSE

ifneq ($(strip $(SHELL_SCRIPT_MAC_LEGACY)),)
	SHELL_SCRIPT_MAC_LEGACY_ENABLE := TRUE
else
	SHELL_SCRIPT_MAC_LEGACY := "echo script not enabled"
endif

# Windows i686
SHELL_SCRIPT_WINDOWS_I686_ENABLE := FALSE

ifneq ($(strip $(SHELL_SCRIPT_WINDOWS_I686)),)
	SHELL_SCRIPT_WINDOWS_I686_ENABLE := TRUE
else
	SHELL_SCRIPT_WINDOWS_I686 := "echo script not enabled"
endif

# Windows x86_64
SHELL_SCRIPT_WINDOWS_X86_64_ENABLE := FALSE

ifneq ($(strip $(SHELL_SCRIPT_WINDOWS_X86_64)),)
	SHELL_SCRIPT_WINDOWS_X86_64_ENABLE := TRUE
else
	SHELL_SCRIPT_WINDOWS_X86_64 := "echo script not enabled"
endif

# if you specify like make LEGACY=TRUE or make mac-os-release LEGACY=TRUE then you can get alternative COMPILER, CFLAGS, LDFLAGS, and STRIP values (very useful for i.e. PowerPC). If nothing is set set it to false.
ifndef LEGACY
override LEGACY := FALSE
endif

$(PROGRAM): clean

# Must run these in a rule.

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

ifeq ($(strip $(RELEASE_NAME_SUFFIX_HOST_32)),)
	$(error Error: The $$RELEASE_NAME_SUFFIX_HOST_32 variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(RELEASE_NAME_SUFFIX_HOST)),)
	$(error Error: The $$RELEASE_NAME_SUFFIX_HOST variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(RELEASE_NAME_SUFFIX_WINDOWS_I686)),)
	$(error Error: The $$RELEASE_NAME_SUFFIX_WINDOWS_I686 variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(RELEASE_NAME_SUFFIX_WINDOWS_X86_64)),)
	$(error Error: The $$RELEASE_NAME_SUFFIX_WINDOWS_X86_64 variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(RELEASE_NAME_SUFFIX_MAC_OS)),)
	$(error Error: The $$RELEASE_NAME_SUFFIX_MAC_OS variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(RELEASE_NAME_SUFFIX_MAC_OS_LEGACY)),)
	$(error Error: The $$RELEASE_NAME_SUFFIX_MAC_OS_LEGACY variable is not set in variables.mk but is required)
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

# Check compiler.

ifeq ($(strip $(COMPILER_HOST)),)
	$(error Error: The $$COMPILER_HOST variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(COMPILER_WINDOWS_I686)),)
	$(error Error: The $$COMPILER_WINDOWS_I686 variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(COMPILER_WINDOWS_X86_64)),)
	$(error Error: The $$COMPILER_WINDOWS_X86_64 variable is not set in variables.mk but is required)
endif

# Check compiler flags.

ifeq ($(strip $(COMPILER_FLAGS_HOST)),)
	$(error Error: The $$COMPILER_FLAGS variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(COMPILER_FLAGS_HOST_32_BIT)),)
	$(error Error: The $$COMPILER_FLAGS_HOST_32_BIT variable is not set in variables.mk but is required)
endif

# Check strip.

ifeq ($(strip $(STRIP_HOST)),)
	$(error Error: The $$STRIP_HOST variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(STRIP_WINDOWS_I686)),)
	$(error Error: The $$STRIP_WINDOWS_I686 variable is not set in variables.mk but is required)
endif

ifeq ($(strip $(STRIP_WINDOWS_X86_64)),)
	$(error Error: The $$STRIP_WINDOWS_X86_64 variable is not set in variables.mk but is required)
endif

	mkdir -p $(BUILD_DIR)

	@if [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_I686)" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_X86_64)" ] && [ "$(LEGACY)" = "FALSE" ] && [ "$(SHELL_SCRIPT_MAC_ENABLE)" = "TRUE" ]; then \
		$(SHELL_SCRIPT_MAC); \
	elif [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_I686)" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_X86_64)" ] && [ "$(LEGACY)" = "TRUE" ] && [ "$(SHELL_SCRIPT_MAC_LEGACY_ENABLE)" = "TRUE" ]; then \
		$(SHELL_SCRIPT_MAC_LEGACY); \
	elif [ "$(COMPILER)" = "$(COMPILER_WINDOWS_I686)" ] && [ "$(SHELL_SCRIPT_WINDOWS_I686_ENABLE)" = "TRUE" ]; then \
		$(SHELL_SCRIPT_WINDOWS_I686); \
	elif [ "$(COMPILER)" = "$(COMPILER_WINDOWS_X86_64)" ] && [ "$(SHELL_SCRIPT_WINDOWS_X86_64_ENABLE)" = "TRUE" ]; then \
		$(SHELL_SCRIPT_WINDOWS_X86_64); \
		elif [ "$(32_BIT_BUILD)" = "TRUE" ] && [ "$(SHELL_SCRIPT_HOST_32_BIT_ENABLE)" = "TRUE" ]; then \
		$(SHELL_SCRIPT_HOST_32_BIT); \
	elif [ "$(SHELL_SCRIPT_HOST_ENABLE)" = "TRUE" ]; then \
		$(SHELL_SCRIPT_HOST); \
	fi
	
	@if [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_I686)" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_X86_64)" ] && [ "$(LEGACY)" = "FALSE" ]; then \
		echo "$(COMPILER_MAC) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) $(LDFLAGS_MAC) -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER_MAC) $(COMPILER_FLAGS_MAC) $(SOURCE_FILES) $(LDFLAGS_MAC) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
		echo "$(STRIP_MAC) $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(STRIP_MAC) $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	elif [ "$(shell uname)" = "Darwin" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_I686)" ] && [ "$(COMPILER)" != "$(COMPILER_WINDOWS_X86_64)" ] && [ "$(LEGACY)" = "TRUE" ]; then \
		echo "$(COMPILER_MAC_LEGACY) $(COMPILER_FLAGS_MAC_LEGACY) $(SOURCE_FILES) $(LDFLAGS_MAC_LEGACY) -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER_MAC_LEGACY) $(COMPILER_FLAGS_MAC_LEGACY) $(SOURCE_FILES) $(LDFLAGS_MAC_LEGACY) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
		echo "$(STRIP_MAC_LEGACY) $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(STRIP_MAC_LEGACY) $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	elif [ "$(COMPILER)" = "$(COMPILER_WINDOWS_I686)" ]; then \
		echo "$(COMPILER_WINDOWS_I686) $(COMPILER_FLAGS_WINDOWS_I686) $(SOURCE_FILES) $(LDFLAGS_WINDOWS_I686) -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER_WINDOWS_I686) $(COMPILER_FLAGS_WINDOWS_I686) $(SOURCE_FILES) $(LDFLAGS_WINDOWS_I686) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
		echo "$(STRIP_WINDOWS_I686) $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(STRIP_WINDOWS_I686) $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	elif [ "$(COMPILER)" = "$(COMPILER_WINDOWS_X86_64)" ]; then \
		echo "$(COMPILER_WINDOWS_X86_64) $(COMPILER_FLAGS_WINDOWS_X86_64) $(SOURCE_FILES) $(LDFLAGS_WINDOWS_X86_64) -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER_WINDOWS_X86_64) $(COMPILER_FLAGS_WINDOWS_X86_64) $(SOURCE_FILES) $(LDFLAGS_WINDOWS_X86_64) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
		echo "$(STRIP_WINDOWS_X86_64) $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(STRIP_WINDOWS_X86_64) $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	elif [ "$(32_BIT_BUILD)" = "TRUE" ]; then \
		echo "$(COMPILER_HOST_32_BIT) "$(COMPILER_FLAGS_HOST_32_BIT)" $(SOURCE_FILES) "$(LDFLAGS_HOST_32_BIT)" -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER_HOST_32_BIT) $(COMPILER_FLAGS_HOST_32_BIT) $(SOURCE_FILES) $(LDFLAGS_HOST_32_BIT) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
		echo "$(STRIP_HOST_32_BIT) $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(STRIP_HOST_32_BIT) $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	else \
		echo "$(COMPILER_HOST) "$(COMPILER_FLAGS_HOST)" $(SOURCE_FILES) "$(LDFLAGS_HOST)" -o $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(COMPILER_HOST) $(COMPILER_FLAGS_HOST) $(SOURCE_FILES) $(LDFLAGS_HOST) -o $(BUILD_DIR)/$(EXECUTABLE_NAME); \
		echo "$(STRIP_HOST) $(BUILD_DIR)/$(EXECUTABLE_NAME)"; \
		$(STRIP_HOST) $(BUILD_DIR)/$(EXECUTABLE_NAME); \
	fi

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

.PHONY: host-32
host-32: clean
	make $(PROGRAM) 32_BIT_BUILD=TRUE EXECUTABLE_NAME='$(PROGRAM).i386'

.PHONY: host-64
host-64: clean
	make $(PROGRAM) EXECUTABLE_NAME='$(PROGRAM).x86_64'

.PHONY: windows-i686
windows-i686: clean
	make $(PROGRAM) COMPILER=$(COMPILER_WINDOWS_I686) EXECUTABLE_NAME='$(PROGRAM).i686.exe'

.PHONY: windows-x86_64
windows-x86_64: clean
	make $(PROGRAM) COMPILER=$(COMPILER_WINDOWS_X86_64) EXECUTABLE_NAME='$(PROGRAM).x86_64.exe'

.PHONY: mac-os
mac-os: clean
ifeq ($(strip $(LEGACY)),TRUE)
	make $(PROGRAM) COMPILER=$(COMPILER_MAC) EXECUTABLE_NAME='$(PROGRAM).$(shell uname -p)'
else
	make $(PROGRAM) COMPILER=$(COMPILER_MAC) EXECUTABLE_NAME='$(PROGRAM).$(shell uname -m)'
endif

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

.PHONY: host-32-release
host-32-release: host-32
	make release PLATFORM='$(RELEASE_NAME_SUFFIX_HOST_32)' EXECUTABLE_NAME='$(PROGRAM).i386'

.PHONY: host-64-release
host-64-release: host-64
	make release PLATFORM='$(RELEASE_NAME_SUFFIX_HOST)' EXECUTABLE_NAME='$(PROGRAM).x86_64'

.PHONY: windows-i686-release
windows-i686-release: windows-i686
	make release PLATFORM='$(RELEASE_NAME_SUFFIX_WINDOWS_I686)' EXECUTABLE_NAME='$(PROGRAM).i686.exe' WINDOWS_RELEASE=true

.PHONY: windows-x86_64-release
windows-x86_64-release: windows-x86_64
	make release PLATFORM='$(RELEASE_NAME_SUFFIX_WINDOWS_X86_64)' EXECUTABLE_NAME='$(PROGRAM).x86_64.exe' WINDOWS_RELEASE=true

.PHONY: mac-os-release
mac-os-release: mac-os
ifeq ($(strip $(LEGACY)),TRUE)
	make release PLATFORM='$(RELEASE_NAME_SUFFIX_MAC_OS_LEGACY)' EXECUTABLE_NAME='$(PROGRAM).$(shell uname -p)'
else
	make release PLATFORM='$(RELEASE_NAME_SUFFIX_MAC_OS)' EXECUTABLE_NAME='$(PROGRAM).$(shell uname -m)'
endif

.PHONY: host-32-deb
host-32-deb: host-32
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)/usr/bin
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)/DEBIAN
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) $(BUILD_DIR)/$(PROGRAM)
	cp $(BUILD_DIR)/$(PROGRAM) $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)/usr/bin
	cp control-i386 $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST_32)

.PHONY: host-64-deb
host-64-deb: host-64
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)/usr/bin
	mkdir -p $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)/DEBIAN
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) $(BUILD_DIR)/$(PROGRAM)
	cp $(BUILD_DIR)/$(PROGRAM) $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)/usr/bin
	cp control-x86_64 $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)
	rm -rf $(BUILD_DIR)/$(RELEASE_BASE_NAME)-$(VERSION)-$(RELEASE_NAME_SUFFIX_HOST)

.PHONY: host-32-rpm
host-32-rpm: host-32
	rm -rf rpm-tmp
	mkdir -p rpm-tmp/RPMS rpm-tmp/SPECS rpm-tmp/SOURCES rpm-tmp/BUILD
	cp ezre.spec rpm-tmp/SPECS/ezre.spec
	cp $(BUILD_DIR)/$(EXECUTABLE_NAME) rpm-tmp/SOURCES/$(PROGRAM)
	ls rpm-tmp/SOURCES/
	rpmbuild -bb --target i386 rpm-tmp/SPECS/ezre.spec --define "_topdir $(shell pwd)/rpm-tmp"
	ls rpm-tmp/RPMS
	mv rpm-tmp/RPMS/i386/*.rpm build/
	rm -rf rpm-tmp

.PHONY: host-64-rpm
host-64-rpm: host-64
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
		make host-32-release; \
		make host-32-deb EXECUTABLE_NAME='$(PROGRAM).i386'; \
		make host-32-rpm EXECUTABLE_NAME='$(PROGRAM).i386'; \
		make host-64-release; \
		make host-64-deb EXECUTABLE_NAME='$(PROGRAM).x86_64'; \
		make host-64-rpm EXECUTABLE_NAME='$(PROGRAM).x86_64'; \
	elif [ "$(shell uname)" = "Darwin" ]; then \
		make mac-os-release; \
	fi
	
	make windows-i686-release
	make windows-x86_64-release
	
	make clean
