# ADDING A NEW TEST MODE

The process of adding new modes to screentest is not very complicated.
Follow these steps (let's assume you want to add a test called "circle").

Create the new test's source file, `test_circle.c`. You should copy the top of
the file with copyright notice and #include statements from another source file
(say `test_text.c`), so it looks like this:

```c
/*
[...]
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>

#include "callbacks.h"
```

Add the source file name (`test_circle.c`) to the `screentest_SOURCES` variable
in `Makefile.am`.

In `test_circle.c` you have to define the (non-static!) structure

```c
G_MODULE_EXPORT struct test_ops circle_ops = {
```

Make sure the name of the structure ends in "_ops". This structure, which have
four member functions, defines the basic operations of your test mode. The
structure members are:

```c
void circle_init(GtkWidget *widget);
```

This function is called when screentest is switched to your "circle" test.
The argument is GtkWidget of type DrawingArea, which covers the whole screen.
This function is optional, you can set it to NULL.

```c
void circle_draw(GtkWidget *widget, gboolean clear);
```

This function is called when screentest decides to redraw the screen
(either when mode/fgcolor/bgcolor changes, or the Expose event from
the X server is received). The first argument is the same
DrawingArea as above, the second one is True, when the screentest
wants you to clear the window before you start drawing. This function
is not optional. You must define it (after all, your test wants
to draw something, doesn't it?).

```c
void circle_cycle(GtkWidget *widget);
```

Screentest calls this function when user presses the left mouse button. You can
use it to cycle through values of some parameter (let's say, the circle
diameter). You don't need to redraw the screen. The `circle_draw()` function
will be called afterwards. This function is optional, you can set the "cycle"
member of `circle_ops` to `NULL`. The widget argument is the same as above.

```c
void circle_close(GtkWidget *widget);
```

This function is called when screentest wants to switch to another mode (and is
called before the new mode's `init()` function). You can use it to deallocate
fonts, colors, timers or other resources you have allocated (for example in
`circle_init()` function). This function is optional, you can set the "cycle"
member of `circle_ops` to `NULL`. The widget argument is the same as above.

You can use the following global variables in the above functions:

```c
GdkGC *gc, *backgc;
```

These are two graphic contexts. The first one has the foreground color set to
the current foreground color (default white) and background color to the
current background color (default black). The second one has its foreground
color set to the current background color (default black) and the background
color to the current foreground color (default white). So you can use the
first to draw with the foreground color and the second to draw with the
background color. You can (ab)use these contexts for your own colors,
but remember to set the color back to its original value before your return
from the `circle_*` function.

```c
GdkColor fgcolors[COLOR_MAX];
```

The GdkColor array with preinitialized color values (see the enum `test_color`
in callbacks.h).

```c
GdkColor grays[GRAYS_MAX];
```

The GdkColor array with pixels preinitialized to the shades of gray.
`grays[0]` is black, `grays[GRAYS_MAX-1]` is white. Currently the
`GRAYS_MAX` is #defined to `COLOR_MAX`, but do not depend on it.

Use glade to add the new mode to the menu (alternatively, edit `screentest.ui`
by hand). Make sure the name of the menu item is the name of
your `<test>_ops` struct without the "_ops" ending.

Run ./autogen.sh and make.

## Contributing your changes

If you want to distribute the resulting program: Beware this program is under
the GNU General Public License, so if you distribute the modified version, you
have to distribute the source code of all your modifications. See the file
COPYING for details.

You must make the `test_circle.c` available, but you don't have to send it
to the author of this program. However, the author of this program kindly asks
you to contribute any modifications. Please send your modifications
to the author of screentest, Tobias Gruetzmacher <tobias-screentest@23.gs>.
The preferred form of the contribution is either a GitHub pull request or an
unified diff. To make a unified diff, follow these steps:

- rename your directory with modified sources (for example
  `mv screentest-2.0 screentest-2.0-circle`).
- run `make distclean` in this directory.
- unpack the clean sources again:
  `zcat screentest-2.0.tar.gz | tar xvf -`
- create the patch with the following command:
  `diff -uNr screentest-2.0 screentest-2.0-circle > screentest-circle.patch`
- mail the screentest-circle.patch to the author.

Thanks,

- Tobias Gruetzmacher <tobias-screentest@23.gs> (current maintainer)
- Jan "Yenya" Kasprzak <kas@fi.muni.cz> (previous maintainer)

