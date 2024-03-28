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

#ifndef SCREENTEST_COLORS_H
#define SCREENTEST_COLORS_H

#include <gdk/gdk.h>

/**
 * An array representing a predefined set of colors for screentest
 */
enum ScreentestColors {
  SCREENTEST_COLORS_WHITE,
  SCREENTEST_COLORS_RED,
  SCREENTEST_COLORS_GREEN,
  SCREENTEST_COLORS_BLUE,
  SCREENTEST_COLORS_CYAN,
  SCREENTEST_COLORS_MAGENTA,
  SCREENTEST_COLORS_YELLOW,
  SCREENTEST_COLORS_BLACK,
  SCREENTEST_COLORS_MAX
};

/**
 * One above the highest index of the `grays` array
 */
#define SCREENTEST_GRAYS_MAX SCREENTEST_COLORS_MAX

/**
 * @brief A pointer to the storage location of the background color (on stack)
 */
extern GdkRGBA *const bg_color;
/**
 * @brief An array holding all predefined `ScreentestColors`
 */
extern const GdkRGBA fgcolors[SCREENTEST_COLORS_MAX];
/**
 * @brief A pointer to the storage location of the foreground color (on stack)
 */
extern GdkRGBA *const fg_color;
/**
 * @brief An array holding all predefined gray value "colors"
 */
extern const GdkRGBA grays[SCREENTEST_GRAYS_MAX];

/**
 * @brief Set the source pattern of the given Cairo context to an opaque color
 * @param[in,out] cr The Cairo context which is used to draw the background
 */
void screentest_set_color_bg(cairo_t *cr);
/**
 * @brief Set the source pattern of the given Cairo context to an opaque color
 * @param[in,out] cr The Cairo context which is used to draw the foreground
 */
void screentest_set_color_fg(cairo_t *cr);

#endif // SCREENTEST_COLORS_H
