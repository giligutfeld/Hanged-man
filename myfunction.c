#include <stdbool.h>

#define NM n * m
#define NM_PIXEL n * m * sizeof (pixel)
#define NM3 n * m * 3
#define M3PLUS3 3 * m + 3
#define DIM3 m * 3

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    int sum;
} pixel;


void smoothBlur(register unsigned char *pixel1, register char *c) {
    register unsigned char *pixel2 = pixel1 + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3;
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3, end = m - 1, j, i, k;

    for (i = end - 1; i > 0; --i) {

        sumRed1 = *(pixel1 - 3) + *(pixel2 - 3) + *(pixel3 - 3);
        sumRed2 = *pixel1 + *pixel2 + *pixel3;
        sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
        sumGreen1 = *(pixel1 - 2) + *((pixel2 - 2)) + *((pixel3 - 2));
        sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
        sumGreen3 = *((pixel1 + 4)) + *((pixel2 + 4)) + *((pixel3 + 4));
        sumBlue1 = *(pixel1 - 1) + *((pixel2 - 1)) + *((pixel3 - 1));
        sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);
        sumBlue3 = *((pixel1 + 5)) + *((pixel2 + 5)) + *((pixel3 + 5));

        for (j = 1; j < end - 2; j+=3) {

            *(c++) = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *(c++) = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *(c++) = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            sumRed1 = *(pixel1 + 6) + *(pixel2 + 6) + *(pixel3 + 6);
            sumGreen1 = *(pixel1 + 7) + *((pixel2 + 7)) + *(pixel3 + 7);
            sumBlue1 = *(pixel1 + 8) + *((pixel2 + 8)) + *(pixel3 + 8);

            *(c++) = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *(c++) = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *(c++) = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            pixel1 += 9;
            pixel2 += 9;
            pixel3 += 9;

            sumRed2 = *pixel1 + *pixel2 + *pixel3;
            sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
            sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);

            *(c++) = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *(c++) = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *(c++) = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen3 = *((pixel1 + 4)) + *((pixel2 + 4)) + *((pixel3 + 4));
            sumBlue3 = *((pixel1 + 5)) + *((pixel2 + 5)) + *((pixel3 + 5));
        }

        for (k = j; k < end; ++k) {
            *(c++) = (char)((*(pixel1 - 3) + *pixel1 + *(pixel1 + 3) + *(pixel2 - 3) + *pixel2 + *(pixel2 + 3) + *(pixel3 - 3) + *pixel3 + *(pixel3 + 3)) / 9);
            *(c++) = (char)((*(pixel1 - 2) + *(pixel1 + 1) + *((pixel1 + 4)) + *((pixel2 - 2)) + *(pixel2 + 1) + *((pixel2 + 4)) + *((pixel3 - 2)) + *(pixel3 + 1) + *((pixel3 + 4))) / 9);
            *(c++) = (char)((*(pixel1 - 1) + *(pixel1 + 2) + *((pixel1 + 5)) + *((pixel2 - 1)) + *(pixel2 + 2) + *((pixel2 + 5)) + *((pixel3 - 1)) + *(pixel3 + 2) + *((pixel3 + 5))) / 9);

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
        }

        pixel1 += 6;
        pixel2 += 6;
        pixel3 += 6;
        c += 6;
    }
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smoothSharp(register unsigned char *src, unsigned char *c) {
    register int end = m - 1, i, j, red, green, blue;
    register unsigned char *pixel2 = src + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3;

    for (i = end - 1; i > 0; i--) {
        for (j = end - 1; j > 0; j--) {

            red = - (*(src - 3) + *src + *(src + 3) + *(pixel2 - 3) -9 * *pixel2 +
                     *(pixel2 + 3) + *(pixel3 - 3) + *pixel3 + *(pixel3 + 3));
            if (red < 0)
                *c = 0;
            else if (red <= 255)
                *c = red;
            else
                *c = 255;
            ++c;

            green = - (*(src - 2) + *(src + 1) + *(src + 4) + *(pixel2 - 2) -9 * *(pixel2 + 1) +
                       *(pixel2 + 4) + *(pixel3 - 2) + *(pixel3 + 1) + *(pixel3 + 4));
            if (green < 0)
                *c = 0;
            else if (green <= 255)
                *c = green;
            else
                *c = 255;

            ++c;

            blue = - (*(src - 1) + *(src + 2) + *(src + 5) + *(pixel2 - 1) -9 * *(pixel2 + 2) +
                      *(pixel2 + 5) + *(pixel3 - 1) + *(pixel3 + 2) + *(pixel3 + 5));
            if (blue < 0)
                *c = 0;
            else if (blue <= 255)
                *c = blue;
            else
                *c = 255;
            ++c;
            src += 3;
            pixel2 += 3;
            pixel3 += 3;
        }
        src += 6;
        pixel2 += 6;
        pixel3 += 6;
        c += 6;
    }
}

void smoothBlurFilter(register pixel *pixel2, register char *c) {
    register int end = m - 1, i, j, min_intensity, max_intensity, red, green, blue;

    register pixel *pixel5, *pixel8, *minp, *maxp;
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    pixel5 = pixel2 + m;
    pixel8 = pixel5 + m;

    for (i = end - 1; i > 0; --i) {
        sumRed1 = (pixel2 - 1)->red + (pixel5 - 1)->red + (pixel8 - 1)->red;
        sumGreen1 = (pixel2 - 1)->green + (pixel5 - 1)->green + (pixel8 - 1)->green;
        sumBlue1 = (pixel2 - 1)->blue + (pixel5 - 1)->blue + (pixel8 - 1)->blue;

        sumRed2 = pixel2->red + pixel5->red + pixel8->red;
        sumGreen2 = pixel2->green + pixel5->green + pixel8->green;
        sumBlue2 = pixel2->blue + pixel5->blue + pixel8->blue;

        sumRed3 = (pixel2 + 1)->red + (pixel5 + 1)->red + (pixel8 + 1)->red;
        sumGreen3 = (pixel2 + 1)->green + (pixel5 + 1)->green + (pixel8 + 1)->green;
        sumBlue3 = (pixel2 + 1)->blue + (pixel5 + 1)->blue + (pixel8 + 1)->blue;

        for (j = end - 1; j > 0; --j) {

            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            min_intensity = (pixel2 - 1)->sum;
            max_intensity = (pixel2 - 1)->sum;
            minp = (pixel2 - 1);
            maxp = (pixel2 - 1);

            if (pixel2->sum <= min_intensity) {
                min_intensity = pixel2->sum;
                minp = pixel2;
            }
            if (pixel2->sum > max_intensity) {
                max_intensity = pixel2->sum;
                maxp = pixel2;
            }

            if ((pixel2 + 1)->sum <= min_intensity) {
                min_intensity = (pixel2 + 1)->sum;
                minp = (pixel2 + 1);
            }
            if ((pixel2 + 1)->sum > max_intensity) {
                max_intensity = (pixel2 + 1)->sum;
                maxp = (pixel2 + 1);
            }

            if ((pixel5 - 1)->sum <= min_intensity) {
                min_intensity = (pixel5 - 1)->sum;
                minp = (pixel5 - 1);
            }
            if ((pixel5 - 1)->sum > max_intensity) {
                max_intensity = (pixel5 - 1)->sum;
                maxp = (pixel5 - 1);
            }

            if (pixel5->sum <= min_intensity) {
                min_intensity = pixel5->sum;
                minp = pixel5;
            }
            if (pixel5->sum > max_intensity) {
                max_intensity = pixel5->sum;
                maxp = pixel5;
            }

            if ((pixel5 + 1)->sum <= min_intensity) {
                min_intensity = (pixel5 + 1)->sum;
                minp = (pixel5 + 1);
            }
            if ((pixel5 + 1)->sum > max_intensity) {
                max_intensity = (pixel5 + 1)->sum;
                maxp = (pixel5 + 1);
            }

            if ((pixel8 - 1)->sum <= min_intensity) {
                min_intensity = (pixel8 - 1)->sum;
                minp = (pixel8 - 1);
            }
            if ((pixel8 - 1)->sum > max_intensity) {
                max_intensity = (pixel8 - 1)->sum;
                maxp = (pixel8 - 1);
            }

            if (pixel8->sum <= min_intensity) {
                min_intensity = pixel8->sum;
                minp = pixel8;
            }
            if (pixel8->sum > max_intensity) {
                max_intensity = pixel8->sum;
                maxp = pixel8;
            }

            if ((pixel8 + 1)->sum <= min_intensity)
                minp = (pixel8 + 1);
            if ((pixel8 + 1)->sum > max_intensity)
                maxp = (pixel8 + 1);


            // divide by kernel's weight
            red = (red - minp->red - maxp->red) / 7;
            green = (green - minp->green - maxp->green) / 7;
            blue = (blue - minp->blue - maxp->blue) / 7;

            // truncate each pixel's color values to match the range [0,255]
            if (red <= 255)
                *c = red;
            else
                *c = 255;
            ++c;

            if (green <= 255)
                *c = green;
            else
                *c = 255;
            ++c;

            if (blue <= 255)
                *c = blue;
            else
                *c = 255;
            ++c;

            ++pixel2;
            ++pixel5;
            ++pixel8;

            sumRed1 = sumRed2;
            sumGreen1 = sumGreen2;
            sumBlue1 = sumBlue2;

            sumRed2 = sumRed3;
            sumGreen2 = sumGreen3;
            sumBlue2 = sumBlue3;

            sumRed3 = (pixel2 + 1)->red + (pixel5 + 1)->red + (pixel8 + 1)->red;
            sumGreen3 = (pixel2 + 1)->green + (pixel5 + 1)->green + (pixel8 + 1)->green;
            sumBlue3 = (pixel2 + 1)->blue + (pixel5 + 1)->blue + (pixel8 + 1)->blue;

        }

        pixel2 += 2;
        pixel5 += 2;
        pixel8 += 2;
        c += 6;
    }
}


void charsToPixelsAndSum(Image *charsImg, register pixel* pixels) {
    pixel *helpSum = pixels + m + 1;
    register char *data = image->data;
    int row, i, j;

    // first row
    for (i = 0; i < m - 3; i += 4) {
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;
    }

    for (j = i; i < m; ++i) {
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;
    }

    for (i = 1; i < m - 1; i++) {

        // first pixel in the row
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        for (row = 1; row < m - 3; row += 3) {
            pixels->red = *data;
            ++data;
            pixels->green = *data;
            ++data;
            pixels->blue = *data;
            ++data;
            pixels->sum = pixels->red + pixels->green + pixels->blue;
            ++pixels;

            pixels->red = *data;
            ++data;
            pixels->green = *data;
            ++data;
            pixels->blue = *data;
            ++data;
            pixels->sum = pixels->red + pixels->green + pixels->blue;
            ++pixels;

            pixels->red = *data;
            pixels->sum = pixels->red;
            ++data;
            pixels->green = *data;
            pixels->sum += pixels->green;
            ++data;
            pixels->blue = *data;
            pixels->sum += pixels->blue;
            ++data;
            ++pixels;
        }
        for (j = row; j < m - 1; ++j) {
            pixels->red = *data;
            ++data;
            pixels->green = *data;
            ++data;
            pixels->blue = *data;
            ++data;
            pixels->sum = pixels->red + pixels->green + pixels->blue;
            ++pixels;
        }

        // last pixel in the row
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;
    }

    // last row
    for (i = 0; i < m - 3; i += 4) {
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;

        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;
    }

    for (j = i; i < m; ++i) {
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;
    }
}

void imageToChars(register unsigned char *data, register unsigned char* c) {
    int i, j;
    for (i = 0; i < NM3 - 6; i+=6) {
        *c = *data;
        ++data;
        ++c;

        *c = *data;
        ++data;
        ++c;

        *c = *data;
        ++data;
        ++c;

        *c = *data;
        ++data;
        ++c;

        *c = *data;
        ++data;
        ++c;

        *c = *data;
        ++data;
        ++c;
    }
    for (j = i; j < NM3; ++j) {
        *c = *data;
        ++data;
        ++c;
    }
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    if (flag == '1') {
        char *backupOrg = malloc(NM3);
        imageToChars(image->data, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlur(backupOrg + 3, c);
        imageToChars(image->data, backupOrg);
        writeBMP(image, srcImgpName, blurRsltImgName);
        smoothSharp(backupOrg + 3, c);
        writeBMP(image, srcImgpName, sharpRsltImgName);
        free(backupOrg);
    } else {
        char *chars = (unsigned char *) malloc(NM3);
        pixel *backupOrg = malloc(NM_PIXEL);
        charsToPixelsAndSum(image, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlurFilter(backupOrg + 1, c);
        imageToChars(image->data, chars);
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);
        smoothSharp(chars + 3, c);
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
        free(chars);
        free(backupOrg);
    }
}