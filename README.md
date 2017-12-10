# Screentest

This is a program for tesing the quality of CRT/LCD screens. It displays
various patterns and allows you to estimate the quality of your CRT/LCD
monitor.

See the file [NEW_TESTS.md](NEW_TESTS.md) for details on adding a new test screens.

The homepage of this program is

	https://tobix.github.io/screentest/

### IF YOU LIKE IT

This program is postcardware. Read the top of the file "COPYING" and send
me a postcard, if you want. Let me know on what system you have tested it,
so I can estimate the userbase and portability of the program.

### IF YOU DON'T LIKE IT

Please let me know what can I do for improving screentest. Use the
e-mail <tobias-screentest@23.gs> or report an issue on
[GitHub](https://github.com/TobiX/screentest/issues/new).

## Authors

- Tobias Gruetzmacher <tobias-screentest@23.gs> (current maintainer)
- Jan "Yenya" Kasprzak (previous maintainer)

## Installation

The installation of this program is pretty straightforward if you are familiar
with using the command line:

### 0. PREREQUISITIES

You will need the following libraries to compile this program:

- GTK+ >= 2.0 (tested on 2.10.13)
- glib >= 2.0 (tested on 2.12.13)

You will need both the libraries (either in static or dynamic form),
and their header files (which may not be installed on your system
by default; for example, check for gtk+-devel, glib-devel and XFree86-devel
packages on Red Hat Linux or Mandrake Linux). This also assumes that the
"pkg-config" command is in your PATH.

The source of screentest is available at the GitHub release page:
    https://github.com/TobiX/screentest/releases

You will also need the ISO C9x-compliant C compiler. This program uses
ISO C9x-style structure initializers, so this is a NECESSARY CONDITION.
You can use for example gcc-3 or later, which is available on most
Unices.

The build process has been tested on Debian unstable (www.debian.org),
but should work on any recent version of UNIX-compatible system.

### 1. PREPARING THE SOURCES

Unpack the distribution using the following command:
    tar xvzf screentest-<version>.tar.gz
Change the directory to the source directory:
    cd screentest-<version>
Set it up for your system:
    ./configure
You can run "./configure --help" for details of the configuration process.

### 2. COMPILING THE PROGRAM

Run "make" (On BSD systems, you might want to use "gmake"). This will compile
the "screentest" program. Since the program is mostly standalone, you can now
verify their function by running them in the current directory (calling
"./screentest").

### 3. INSTALLING

Run "make install".


- Tobias Gruetzmacher <tobias-screentest@23.gs> (current maintainer)
- Jan "Yenya" Kasprzak (previous maintainer)
