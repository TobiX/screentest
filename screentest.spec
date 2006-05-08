Summary: The CRT screen quality testing utility
Name: screentest
Version: 1.0
Release: 1
Copyright: GPL (+postcardware)
Group: X11/Utilities
Source: ftp://ftp.fi.muni.cz/pub/linux/people/jan_kasprzak/screentest/screentest-%{version}.tar.gz
URL: http://www.fi.muni.cz/~kas/screentest/
BuildPrereq: gtk+-devel >= 1.2, XFree86-devel
BuildRoot: /var/tmp/%{name}-root

%description
Screentest is a simple program which displays various patterns
(colors, circles, grids, text) on your screen in order to allow you
to evaluate the quality of your CRT monitor (sharpness, linearity, etc).

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr
make

%install
if test -d ${RPM_BUILD_ROOT}
then rm -rf ${RPM_BUILD_ROOT}
fi
mkdir ${RPM_BUILD_ROOT}
mkdir -p ${RPM_BUILD_ROOT}/usr/bin
make prefix=$RPM_BUILD_ROOT/usr install

%clean
if test -d ${RPM_BUILD_ROOT}
then rm -rf ${RPM_BUILD_ROOT}
fi

%files
%defattr(-, root,root)
%doc README NEW_TESTS COPYING ChangeLog INSTALL NEWS AUTHORS
/usr/bin/screentest

%changelog
* Mon Aug 20 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
- the initial release.
