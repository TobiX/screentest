# Screentest

[![Compile](https://github.com/TobiX/screentest/actions/workflows/compile.yml/badge.svg)](https://github.com/TobiX/screentest/actions/workflows/compile.yml)

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

You will need the following to compile this program:

- Meson >= 0.61 (tested with 1.4)
- GTK3 >= 3.4 (tested on 3.24)
- glib >= 2.0 (tested on 2.80)

You will need both the libraries (either in static or dynamic form),
and their header files (which may not be installed on your system
by default.

The source of screentest is available at the GitHub release page:
    https://github.com/TobiX/screentest/releases

You will also need the ISO C9x-compliant C compiler. This program uses
ISO C9x-style structure initializers, so this is a NECESSARY CONDITION.
You can use for example gcc or clang, which is available on most Unices.

The build process has been tested on Debian unstable (www.debian.org),
but should work on any recent version of UNIX-compatible system.

### 1. PREPARING THE SOURCES

Unpack the distribution using the following command:
    tar xvaf screentest-<version>.tar.xz
Change the directory to the source directory:
    cd screentest-<version>
Set it up for your system:
    meson setup builddir

### 2. COMPILING THE PROGRAM

Run "meson compile". This will compile the "screentest" program. Since the
program is mostly standalone, you can now verify their function by running them
in the current directory (calling "./screentest").

### 3. INSTALLING

Run "meson install".
