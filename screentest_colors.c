/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006-2017 Tobias Gruetzmacher <tobias-screentest@23.gs>
 *  Copyright (C) 2021 Apr Thorsten Kattanek <thorsten.kattanek@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "screentest_colors.h"

#define GRAY_VALUE(idx)                                                        \
  ((GdkRGBA){idx / ((float)SCREENTEST_GRAYS_MAX - 1),                          \
             idx / ((float)SCREENTEST_GRAYS_MAX - 1),                          \
             idx / ((float)SCREENTEST_GRAYS_MAX - 1), 1.0})

const GdkRGBA fgcolors[SCREENTEST_COLORS_MAX] = {
    // WHITE
    {1.0, 1.0, 1.0, 1.0},
    // RED
    {1.0, 0.0, 0.0, 1.0},
    // GREEN
    {0.0, 1.0, 0.0, 1.0},
    // BLUE
    {0.0, 0.0, 1.0, 1.0},
    // CYAN
    {0.0, 1.0, 1.0, 1.0},
    // MAGENTA
    {1.0, 0.0, 1.0, 1.0},
    // YELLOW
    {1.0, 1.0, 0.0, 1.0},
    // BLACK
    {0.0, 0.0, 0.0, 1.0}};
GdkRGBA bg_col = {0.0, 0.0, 0.0, 1.0}; // fgcolors[SCREENTEST_COLORS_BLACK];
GdkRGBA fg_col = {1.0, 1.0, 1.0, 1.0}; // fgcolors[SCREENTEST_COLORS_WHITE];
GdkRGBA *const bg_color = &bg_col;
GdkRGBA *const fg_color = &fg_col;
const GdkRGBA grays[SCREENTEST_GRAYS_MAX] = {
    GRAY_VALUE(0), GRAY_VALUE(1), GRAY_VALUE(2), GRAY_VALUE(3),
    GRAY_VALUE(4), GRAY_VALUE(5), GRAY_VALUE(6), GRAY_VALUE(7),
};

void screentest_set_color_bg(cairo_t *cr) {
  cairo_set_source_rgb(cr, bg_color->red, bg_color->green, bg_color->blue);
}

void screentest_set_color_fg(cairo_t *cr) {
  cairo_set_source_rgb(cr, fg_color->red, fg_color->green, fg_color->blue);
}
