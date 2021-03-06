/*
 *  Diamond Core Filters - collected filters for the Diamond platform
 *
 *  Copyright (c) 2002-2005 Intel Corporation
 *  Copyright (c) 2006 Larry Huston <larry@thehustons.net>
 *  Copyright (c) 2008-2009 Carnegie Mellon University
 *  All Rights Reserved.
 *
 *  This software is distributed under the terms of the Eclipse Public
 *  License, Version 1.0 which can be found in the file named LICENSE.
 *  ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS SOFTWARE CONSTITUTES
 *  RECIPIENT'S ACCEPTANCE OF THIS AGREEMENT
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include "lib_results.h"
#include "lib_filimage.h"
#include "rgb.h"

#define	RED_SCALE	205
#define	RED_BASE	50
#define	GREEN_SCALE	205
#define	GREEN_BASE	50
#define	BLUE_SCALE	205
#define	BLUE_BASE	50


static void
write_pixel(RGBImage * img, int x, int y, u_char r, u_char g, u_char b)
{
	ssize_t         offset;
	RGBPixel       *pixel;


	if (x < 0 || y < 0 || x >= img->width || y >= img->height) {
		return;
	}
	offset = x + (y * img->width);
	assert(offset < img->width * img->height);
	pixel = &img->data[offset];

	pixel->r = r;
	pixel->g = g;
	pixel->b = b;
	pixel->a = 255;
}


RGBImage       *
image_gen_image_scale(RGBImage * data, int scale)
{
	RGBImage       *img;
	int             x, y;
	int             i, j;
	double          cum_r, cum_g, cum_b;
	double          avg_r, avg_g, avg_b;
	double          min_color, max_color;
	int             x_scale, y_scale;
	int		columns, rows, nbytes;

	assert(data->rows);
	assert(data->columns);

	/* compute new real size */
	columns = data->columns / scale;
	rows = data->rows / scale;
	nbytes = sizeof(RGBImage) + columns * rows * sizeof(RGBPixel);

	img = (RGBImage *) malloc(nbytes);
	if (img == NULL) {
		/*
		 * XXX log 
		 */
		printf("XXX failed to allocate image \n");
		exit(1);
	}


	/*
	 * First go through all the data to cmpute the number of rows,
	 * max number of columns as well as the min and max values
	 * for this image.
	 */

	img->columns = columns;
	img->rows = rows;
	img->nbytes = nbytes;
	img->type = data->type;
	min_color = 0;
	max_color = 255;

#ifdef VERBOSE
	fprintf(stderr, "min color %f max %f diff %f \n", min_color, max_color,
	                        max_color - min_color);
	fprintf(stderr, "orig row %d col %d \n", data->columns, data->rows);
	fprintf(stderr, "scaled row %d col %d \n", img->columns, img->rows);
#endif

	for (y = 0; y < img->rows; y++) {
		for (x = 0; x < img->columns; x++) {
			y_scale = y * scale;
			x_scale = x * scale;

			cum_r = cum_g = cum_b = 0;
			for (i = 0; i < scale; i++) {
				for (j = 0; j < scale; j++) {
					RGBPixel       *pixel =
					    &data->data[(y_scale + j) * data->columns +
					                (x_scale + i)];
					cum_r += pixel->r;
					cum_g += pixel->g;
					cum_b += pixel->b;
				}
			}

			avg_r = cum_r / ((double) (scale * scale));
			avg_g = cum_g / ((double) (scale * scale));
			avg_b = cum_b / ((double) (scale * scale));

			if (avg_r < min_color) {
				avg_r = min_color;
			}
			if (avg_g < min_color) {
				avg_g = min_color;
			}
			if (avg_b < min_color) {
				avg_b = min_color;
			}
			if (avg_r > max_color) {
				avg_r = max_color;
			}
			if (avg_g > max_color) {
				avg_g = max_color;
			}
			if (avg_b > max_color) {
				avg_b = max_color;
			}

			write_pixel(img, x, y, (u_char) avg_r, (u_char) avg_g,
			            (u_char) avg_b);
		}
	}

#ifdef VERBOSE
	fprintf(stderr, "image_gen_image_scale\n");
#endif

	return (img);
}
