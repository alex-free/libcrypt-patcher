Name: libcrypt-patcher       
Version: 1.0.9
Summary: The ultimate all in one Libcrypt patcher for PSX disc images.
Release: 1
License: 3-BSD
URL: https://github.com/alex-free/libcrypt-patcher       
Packager: Alex Free

%description
The ultimate all in one Libcrypt patcher for PSX disc images.

%install
mkdir -p %{buildroot}/usr/bin
cp %{_sourcedir}/lcp %{buildroot}/usr/bin/

%files
/usr/bin/lcp
