#include <stdbool.h>

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
int min(int a, int b) { return (a < b ? a : b); }
int max(int a, int b) { return (a > b ? a : b); }

int calcIndex(int i, int j, int n) {
    return ((i)*(n)+(j));
}

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
    sum->red += ((int) p.red) * weight;
    sum->green += ((int) p.green) * weight;
    sum->blue += ((int) p.blue) * weight;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

    int red, green, blue;
    // divide by kernel's weight
    if (kernelScale == 1) {
        red = sum.red;
        green = sum.green;
        blue = sum.blue;
    } else {
        red = sum.red / kernelScale;
        green = sum.green / kernelScale;
        blue = sum.blue / kernelScale;
    }

    if (red < 0)
        red = 0;
    else if (red > 255)
        red = 255;
    if (green < 0)
        green = 0;
    else if (green > 255)
        green = 255;
    if (blue < 0)
        blue = 0;
    else if (blue > 255)
        blue = 255;

    // truncate each pixel's color values to match the range [0,255]
    current_pixel->red = (unsigned char) red;
    current_pixel->green = (unsigned char) green;
    current_pixel->blue = (unsigned char) blue;

}

static pixel doFilter(pixel_sum *sum, int dim, int i, int j, pixel *src) {
    int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    int min_row, min_col, max_row, max_col, sumLoopPixel;
    pixel *loop_pixel = &src[(i - 1) * dim + j - 1];

    // find min and max coordinates
    // relate the rows and columns in the square as -1, 0, 1 because we dont need to calculate i-1 or j-1 every time
    for(int ii = -1; ii <= 1; ii++) {
        for(int jj = -1; jj <= 1; jj++) {
            // check if smaller than min or higher than max and update
            sumLoopPixel = (int) loop_pixel->red + (int) loop_pixel->green + (int) loop_pixel->blue;
            if (sumLoopPixel <= min_intensity) {
                min_intensity = sumLoopPixel;
                min_row = ii;
                min_col = jj;
            }
            if (sumLoopPixel > max_intensity) {
                max_intensity = sumLoopPixel;
                max_row = ii;
                max_col = jj;
            }
            loop_pixel++;
        }
        loop_pixel += dim - 3;
    }

    // filter out min and max
    pixel minp = src[(min_row + i) * dim + min_col + j];
    pixel maxp = src[(max_row + i) * dim + max_col + j];

    sum->red = sum->red - ((int) minp.red + (int) maxp.red);
    sum->green = sum->green - ((int) minp.green + (int) maxp.green);
    sum->blue = sum->blue - ((int) minp.blue + (int) maxp.blue);
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel SharpKernel(int dim, int i, int j, pixel *src, int kernelScale, bool filter) {

	pixel_sum sum;
	pixel current_pixel;
    pixel *save = src;
	initialize_pixel_sum(&sum);

    src += (i - 1) * dim + j;
    sum.red -= (int) (src - 1)->red + (int) src->red + (int) (src + 1)->red;
    sum.green -= (int) (src - 1)->green + (int) src->green + (int) (src + 1)->green;
    sum.blue -= (int) (src - 1)->blue + src->blue + (int) (src + 1)->blue;

    // calculate the row address by add one row size
    src += dim;
    sum.red -= (int) (src - 1)->red - (9) * (int) src->red + (int) (src + 1)->red;
    sum.green -= (int) (src - 1)->green - (9) * (int) src->green + (int) (src + 1)->green;
    sum.blue -= (int) (src - 1)->blue - 9 * (int) src->blue + (int) (src + 1)->blue;

    // calculate the row address by add one row size
    src += dim;
    sum.red -= (int) (src - 1)->red + (int) src->red + (int) (src + 1)->red;
    sum.green -= (int) (src - 1)->green + (int) src->green + (int) (src + 1)->green;
    sum.blue -= (int) (src - 1)->blue + (int) src->blue + (int) (src + 1)->blue;

    if (filter)
        doFilter(&sum, dim, i, j, save);

	// assign kernel's result to pixel at [i,j]
	assign_sum_to_pixel(&current_pixel, sum, kernelScale);
	return current_pixel;
}

static pixel blurKernel(int dim, int i, int j, pixel *src, int kernelScale, bool filter) {

    pixel_sum sum;
    pixel current_pixel;
    pixel *save = src;

    initialize_pixel_sum(&sum);

    src += (i - 1) * dim + j;
    sum.red += (int) (src - 1)->red + (int) src->red + (int) (src + 1)->red;
    sum.green += (int) (src - 1)->green + (int) src->green + (int) (src + 1)->green;
    sum.blue += (int) (src - 1)->blue + src->blue + (int) (src + 1)->blue;

    // calculate the row address by add one row size
    src += dim;
    sum.red += (int) (src - 1)->red + (int) src->red + (int) (src + 1)->red;
    sum.green += (int) (src - 1)->green + (int) src->green + (int) (src + 1)->green;
    sum.blue += (int) (src - 1)->blue + src->blue + (int) (src + 1)->blue;

    // calculate the row address by add one row size
    src += dim;
    sum.red += (int) (src - 1)->red + (int) src->red + (int) (src + 1)->red;
    sum.green += (int) (src - 1)->green + (int) src->green + (int) (src + 1)->green;
    sum.blue += (int) (src - 1)->blue + src->blue + (int) (src + 1)->blue;

    if (filter)
        doFilter(&sum, dim, i, j, save);

    // assign kernel's result to pixel at [i,j]
    assign_sum_to_pixel(&current_pixel, sum, kernelScale);
    return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smoothSharp(int dim, pixel *src, pixel *dst, int kernelSize, int kernelScale, bool filter) {

    int i, j;
    int halfKernelSize = 1;
    int end = dim - halfKernelSize;
    dst += dim + halfKernelSize;

    for (i = halfKernelSize; i < end; i++) {
        for (j = halfKernelSize; j < end; j++)
            *(dst++) = SharpKernel(dim, i, j, src, kernelScale, filter);
        dst += halfKernelSize + halfKernelSize;
    }
}

void smoothBlur(int dim, pixel *src, pixel *dst, int kernelSize, int kernelScale, bool filter) {
    int i, j;
    int halfKernelSize = 1;
    int end = dim - halfKernelSize;
    dst += dim + halfKernelSize;

    for (i = halfKernelSize; i < end; i++) {
        for (j = halfKernelSize; j < end; j++)
            *(dst++) = blurKernel(dim, i, j, src, kernelScale, filter);
        dst += halfKernelSize + halfKernelSize;
    }
}

void charsToPixels(Image *charsImg, pixel* pixels) {
    char* data = image->data;
	for (int row = 0 ; row < m ; row++) {
		for (int col = 0 ; col < n ; col++) {
			pixels->red = *(data++);
			pixels->green = *(data++);
			pixels->blue = *(data++);
            pixels++;
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {
    char* data = image->data;
	for (int row = 0 ; row < m ; row++) {
		for (int col = 0 ; col < n ; col++) {
            *(data++) = pixels->red;
            *(data++) = pixels->green;
            *(data++) = pixels->blue;
            pixels++;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {

	for (int row = 0 ; row < m ; row++) {
		for (int col = 0 ; col < n ; col++) {
			dst->red = src->red;
			dst->green = src->green;
			dst->blue = src->blue;
            dst++;
            src++;
		}
	}
}

void doConvolution(Image *image, int kernelSize, int kernelScale, bool filter, bool blurOrSharp) {

    // calculate the multiply one time instead of two
    int multiply = m * n * sizeof (pixel);
	pixel* pixelsImg = malloc(multiply);
	pixel* backupOrg = malloc(multiply);

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);

    if(blurOrSharp)
	    smoothSharp(m, backupOrg, pixelsImg, kernelSize, kernelScale, filter);
    else
        smoothBlur(m, backupOrg, pixelsImg, kernelSize, kernelScale, filter);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	if (flag == '1') {
		// blur image
		doConvolution(image, 3, 9, false, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, 3, 1, false, true);

		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, 3, 7, true, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, 3, 1, false, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);
	}
}

