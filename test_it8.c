/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006-2017 Tobias Gruetzmacher <tobias@portfolio16.de>
 *  Copyright (C) 2013 Antoine Beaupré <anarcat@debian.org>
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <assert.h>

#include "callbacks.h"
#include "gettext.h"

#define GRID_STEP 64

// how many hues/brightness steps
#define COLOR_STEP 12
#define RGB_COLOR_MAX (3*COLOR_STEP)
#define CMYK_COLOR_MAX (4*COLOR_STEP)
#define HCL_COLOR_MAX (12*COLOR_STEP)

GdkColor rgb_colors[RGB_COLOR_MAX];
GdkColor cmyk_colors[CMYK_COLOR_MAX];
GdkColor hcl_colors[HCL_COLOR_MAX];

static void it8_hcl_to_rgb(GdkColor *color, guint h, gfloat c, gfloat l)
{
  // intermediate values for HCL computations
  gfloat x, m;
  // temporary output values for rgb computations
  gfloat r, g, b;

  assert( 0 <= l);
  assert( l <= 1);
  assert( 0 <= c);
  assert( c <= 1);
  assert( 0 <= h);
  assert( h <= 360);

  // hold on to your butts: https://en.wikipedia.org/wiki/HSL_color_space#From_luma.2Fchroma.2Fhue
  x = c * ( 1 - abs(((h/60) % 2) -1) );
  printf ("x: %f, c: %f, h: %d, abs: %d\n", x, c, h, abs(((h/60) % 2) -1));
  switch (h/60)
    {
    case 0:
      r = c; g = x; b = 0;
      break;
    case 1:
      r = x; g = c; b = 0;
      break;
    case 2:
      r = 0; g = c; b = x;
      break;
    case 3:
      r = 0; g = x; b = c;
      break;
    case 4:
      r = x; g = 0; b = c;
      break;
    case 5:
      r = c; g = 0; b = x;
      break;
    default:
      r = g = b = 0;
      break;
    }
  printf("tmp: %f, %f, %f\n", r, g, b);
  // yes, this is magical
  m = l - ( 0.30 * r + 0.59 * g + 0.11 * b );
  r += m; g += m; b += m;
  // cap to our colorspace
  r = r < 0 ? 0 : r;
  g = g < 0 ? 0 : g;
  b = b < 0 ? 0 : b;
  printf("rgb: %f, %f, %f (m: %f)\n", r, g, b, m);
  printf("hcl (%d, %f, %f) -> rgb 0x%02x%02x%02x\n", h, c, l, (int)(r*0xff), (int)(g*0xff), (int)(b*0xff));
  r *= 0xffff; g *= 0xffff; b *= 0xffff; // scale up to 255, we cast to int below
  //printf("final rgb: (m: %f) %f, %f, %f\n", m, r, g, b);

  color->red = (gint)r; color->green = (gint)g; color->blue = (gint)b;

  // used to be:
  //return ((gint)r << 16) + ((gint)g << 8) + (gint)b;
  //hcl_colors[i].red = ( rgb & ( 255 << 16 ) ) >> 16;
  //hcl_colors[i].green = ( rgb & ( 255 << 8 ) ) >> 8;
  //hcl_colors[i].blue = rgb & ( 255 );

  //printf("hcl -> rgb: (%d, %f, %f) -> (0x%x, 0x%x, 0x%x)\n", h, c, l, color->red, color->green, color->blue);
  
}

// taken from src/library/grDevices/src/colors.c in R 3.0.1
static void hsv2rgb(double h, double s, double v, 
                    double *r, double *g, double *b)
{
    double f, p, q, t;
    int i;

    f = modf(h * 6.0, &t);
    i = ((int) t) % 6;

    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - (s * (1 - f)));
    switch (i) {
    case 0:	*r = v;		*g = t;		*b = p;	break;
    case 1:	*r = q;		*g = v;		*b = p;	break;
    case 2:	*r = p;		*g = v;		*b = t;	break;
    case 3:	*r = p;		*g = q;		*b = v; break;
    case 4:	*r = t;		*g = p;		*b = v; break;
    case 5:	*r = v;		*g = p;		*b = q;	break;
    default:
	error("bad hsv to rgb color conversion");
    }
}

/**
 * following functions are taken from src/library/grDevices/src/colors.c in R 3.0.1
 */

/* D65 White Point */

#define WHITE_X 95.047
#define WHITE_Y 100.000
#define WHITE_Z 108.883
#define WHITE_u 0.1978398
#define WHITE_v 0.4683363

#ifndef DEG2RAD
#define DEG2RAD 0.01745329251994329576
#endif

/* Standard CRT Gamma */

#define GAMMA 2.4

static double gtrans(double u)
{
    if (u > 0.00304)
	return 1.055 * pow(u, (1 / GAMMA)) - 0.055;
    else
	return 12.92 * u;
}

static void
hcl2rgb(double h, double c, double l, double *R, double *G, double *B)
{
    double L, U, V;
    double u, v;
    double X, Y, Z;

    /* Step 1 : Convert to CIE-LUV */

    h = DEG2RAD * h;
    L = l;
    U = c * cos(h);
    V = c * sin(h);

    /* Step 2 : Convert to CIE-XYZ */

    if (L <= 0 && U == 0 && V == 0) {
        X = 0; Y = 0; Z = 0;
    }
    else {
        Y = WHITE_Y * ((L > 7.999592) ? pow((L + 16)/116, 3) : L / 903.3);
        u = U / (13 * L) + WHITE_u;
        v = V / (13 * L) + WHITE_v;
        X =  9.0 * Y * u / (4 * v);
        Z =  - X / 3 - 5 * Y + 3 * Y / v;
    }

    /* Step 4 : CIE-XYZ to sRGB */

    *R = gtrans(( 3.240479 * X - 1.537150 * Y - 0.498535 * Z) / WHITE_Y);
    *G = gtrans((-0.969256 * X + 1.875992 * Y + 0.041556 * Z) / WHITE_Y);
    *B = gtrans(( 0.055648 * X - 0.204043 * Y + 1.057311 * Z) / WHITE_Y);
}

/**
 * end of imported functions
 */

static void it8_hcl_to_rgb2(GdkColor *color, double h, double s, double v)
{
  double r, g, b;
  hcl2rgb(h, s, v, &r, &g, &b);
  if (r < 0) { r = 0; } else if (r > 1) { r = 1; }
  if (g < 0) { g = 0; } else if (g > 1) { g = 1; }
  if (b < 0) { b = 0; } else if (b > 1) { b = 1; }
  printf("hcl res: %f %f %f\n", r, g, b);
  color->red = (gint) (0xffff * r);
  color->green = (gint) (0xffff * g);
  color->blue = (gint) (0xffff * b);
}

static void it8_hsv_to_rgb2(GdkColor *color, double h, double s, double v)
{
  double r, g, b;
  hsv2rgb(h/360, s, v, &r, &g, &b);
  color->red = (gint)(0xffff*r);color->green=(gint)(0xffff*g);color->blue=(gint)(0xffff*b);
}

#define RETURN_RGB(r,g,b) color->red = (gint)(0xffff*r);color->green=(gint)(0xffff*g);color->blue=(gint)(0xffff*b);break;

// code stolen from colorspace.c in r-cran-colorspace-1.2.2
static void it8_hsv_to_rgb(GdkColor *color, gfloat h, gfloat s, gfloat v)
{
  double m, n, f;
  int i;

  assert( 0 <= s);
  assert( s <= 1);
  assert( 0 <= v);
  assert( v <= 1);
  assert( 0 <= h);
  assert( h <= 360);

  h = h /60;		/* convert to [0, 6] */
  i = floor(h);
  f = h - i;
  if(!(i & 1))	/* if i is even */
    f = 1 - f;
  m = v * (1 - s);
  n = v * (1 - s * f);
  switch (i) {
  case 6:
  case 0: RETURN_RGB(v, n, m);
  case 1: RETURN_RGB(n, v, m);
  case 2: RETURN_RGB(m, v, n);
  case 3: RETURN_RGB(m, n, v);
  case 4: RETURN_RGB(n, m, v);
  case 5: RETURN_RGB(v, m, n);
  }
}

static void it8_init(G_GNUC_UNUSED GtkWidget * widget)
{
  gint i, j, n;
  gint rgb;
  gint h;
  gfloat s, v;

  memset(hcl_colors, 0, HCL_COLOR_MAX * sizeof(GdkColor)); n = 0;
  for (s = 1.0; s >= 0.20; s-=0.30) {
    for (v = 0.5; v <= 1.0; v+=0.15) {
      for (i = 0; i < COLOR_STEP; i++) {
        h = (i * (360/COLOR_STEP)) % 360;
        printf("%d hsv -> rgb: (%d, %f, %f) -> ", n, h, s, v);
        it8_hsv_to_rgb2(&hcl_colors[n], h, s, v) ;
        printf("(0x%04x%04x%04x)\n", hcl_colors[n].red, hcl_colors[n].green, hcl_colors[n].blue);
        n++;
      }
    }
  }

  memset(rgb_colors, 0, RGB_COLOR_MAX * sizeof(GdkColor));
  for (i = 0; i < COLOR_STEP; i++) {
    rgb_colors[i].red = ((1 << 16) - 1);
    rgb_colors[i].green = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    rgb_colors[i].blue = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
  }
  for (i = 0; i < COLOR_STEP; i++) {
    rgb_colors[i+COLOR_STEP].red = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    rgb_colors[i+COLOR_STEP].green = ((1 << 16) - 1);
    rgb_colors[i+COLOR_STEP].blue = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
  }
  for (i = 0; i < COLOR_STEP; i++) {
    rgb_colors[i+2*COLOR_STEP].red = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    rgb_colors[i+2*COLOR_STEP].green = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    rgb_colors[i+2*COLOR_STEP].blue = ((1 << 16) - 1);
  }
  memset(cmyk_colors, 0, CMYK_COLOR_MAX * sizeof(GdkColor));
  for (i = 0; i < COLOR_STEP; i++) {
    cmyk_colors[i].red = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    cmyk_colors[i].green = ((1 << 16) - 1);
    cmyk_colors[i].blue = ((1 << 16) - 1);
  }
  for (i = 0; i < COLOR_STEP; i++) {
    cmyk_colors[i+COLOR_STEP].red = ((1 << 16) - 1);
    cmyk_colors[i+COLOR_STEP].green = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    cmyk_colors[i+COLOR_STEP].blue = ((1 << 16) - 1);
  }
  for (i = 0; i < COLOR_STEP; i++) {
    cmyk_colors[i+2*COLOR_STEP].red = ((1 << 16) - 1);
    cmyk_colors[i+2*COLOR_STEP].green = ((1 << 16) - 1);
    cmyk_colors[i+2*COLOR_STEP].blue = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
  }
  for (i = 0; i < COLOR_STEP; i++) {
    cmyk_colors[i+3*COLOR_STEP].red = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    cmyk_colors[i+3*COLOR_STEP].green = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
    cmyk_colors[i+3*COLOR_STEP].blue = i * ((1 << 16) - 1) / (COLOR_STEP - 1);
  }
}

static void it8_draw(GtkWidget * widget)
{
	GdkWindow *win = widget->window;
	gint w, h; // window size
	gint i; // counter
	gint d; // boxes size
        gint x_offset; // where we are vertically
        gint border = 20;

	gdk_window_get_size(win, &w, &h);

	d = w / 4;
	if (d > h / 4)
		d = h / 4;

	for (i = ((h - 1) % GRID_STEP) / 2; i < h;
	     i += GRID_STEP)
		gdk_draw_line(win, gc, 0, i, w - 1, i);

	d = 20;

        d = ( h - 2*border ) / ( COLOR_STEP );
        printf("h: %d, w: %d, d: %d\n", h, w, d);

        x_offset = border;

        for (i = 0; i < HCL_COLOR_MAX; i++) {
          gdk_gc_set_rgb_fg_color(gc, &hcl_colors[i]);
          gdk_draw_rectangle(win, gc, TRUE, x_offset + (i/COLOR_STEP) * d, border + (i % COLOR_STEP) * d, d, d);
        }

        x_offset += ((i+1)/COLOR_STEP) * d;
        for (i = 0; i < CMYK_COLOR_MAX; i++) {
          gdk_gc_set_rgb_fg_color(gc, &cmyk_colors[i]);
          gdk_draw_rectangle(win, gc, TRUE, x_offset + (i/COLOR_STEP) * d, border + (i % COLOR_STEP) * d, d, d);
        }

        x_offset += ((i+1)/COLOR_STEP) * d;
        for (i = 0; i < RGB_COLOR_MAX; i++) {
          gdk_gc_set_rgb_fg_color(gc, &rgb_colors[i]);
          gdk_draw_rectangle(win, gc, TRUE, x_offset + (i/COLOR_STEP) * d, border + (i % COLOR_STEP) * d, d, d);
        }
}

G_MODULE_EXPORT struct test_ops it8_ops = {
	.init = it8_init,
	.draw = it8_draw,
	.cycle = NULL,
	.close = NULL
};
