Name: libcrypt-patcher       
Version: v1.1.0
Summary: The ultimate all in one LibCrypt PSX/PS1 game patcher.
Release: 1
License: 3-BSD
URL: https://github.com/alex-free       
Packager: Alex Free
Group: Unspecified

%description
The ultimate all in one LibCrypt PSX/PS1 game patcher.

%install
mkdir -p %{buildroot}/usr/bin
cp %{_sourcedir}/lcp %{buildroot}/usr/bin/

%files
/usr/bin/lcp
