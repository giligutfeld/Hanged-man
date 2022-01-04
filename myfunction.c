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


void smoothBlur(int dim, pixel *src, char *c) {
    int end = dim - 1, i, j, k;
    ++src;
    pixel *pixel1, *pixel2, *pixel3, *pixel4, *pixel5, *pixel6, *pixel7, *pixel8, *pixel9;
    int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    for (i = end - 1; i > 0; --i) {

        pixel1 = src - 1;
        pixel2 = src;
        pixel3 = src + 1;
        pixel4 = pixel1 + dim;
        pixel5 = pixel4 + 1;
        pixel6 = pixel5 + 1;
        pixel7 = pixel4 + dim;
        pixel8 = pixel7 + 1;
        pixel9 = pixel8 + 1;

        sumRed1 = pixel1->red + pixel4->red + pixel7->red;
        sumRed2 = pixel2->red + pixel5->red + pixel8->red;
        sumRed3 = pixel3->red + pixel6->red + pixel9->red;
        sumGreen1 = pixel1->green + pixel4->green + pixel7->green;
        sumGreen2 = pixel2->green + pixel5->green + pixel8->green;
        sumGreen3 = pixel3->green + pixel6->green + pixel9->green;
        sumBlue1 = pixel1->blue + pixel4->blue + pixel7->blue;
        sumBlue2 = pixel2->blue + pixel5->blue + pixel8->blue;
        sumBlue3 = pixel3->blue + pixel6->blue + pixel9->blue;

        for (j = end - 3; j > 0; j-=3) {

            *(c++) = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *(c++) = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *(c++) = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            pixel1 = pixel3 + 1;
            pixel4 = pixel6 + 1;
            pixel7 = pixel9 + 1;

            sumRed1 = pixel1->red + pixel4->red + pixel7->red;
            sumGreen1 = pixel1->green + pixel4->green + pixel7->green;
            sumBlue1 = pixel1->blue + pixel4->blue + pixel7->blue;

            *(c++) = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *(c++) = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *(c++) = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            pixel2 = pixel1 + 1;
            pixel5 = pixel4 + 1;
            pixel8 = pixel7 + 1;

            sumRed2 = pixel2->red + pixel5->red + pixel8->red;
            sumGreen2 = pixel2->green + pixel5->green + pixel8->green;
            sumBlue2 = pixel2->blue + pixel5->blue + pixel8->blue;

            *(c++) = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *(c++) = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *(c++) = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            pixel3 = pixel2 + 1;
            pixel6 = pixel5 + 1;
            pixel9 = pixel8 + 1;

            sumRed3 = pixel3->red + pixel6->red + pixel9->red;
            sumGreen3 = pixel3->green + pixel6->green + pixel9->green;
            sumBlue3 = pixel3->blue + pixel6->blue + pixel9->blue;
        }

        for (k = j - 1; k > 0; --k) {
            pixel2 = src + dim;
            pixel3 = pixel2 + dim;

            *(c++) = (char)(((src - 1)->red + src->red + (src + 1)->red + (pixel2 - 1)->red + pixel2->red + (pixel2 + 1)->red + (pixel3 - 1)->red + pixel3->red + (pixel3 + 1)->red) / 9);
            *(c++) = (char)(((src - 1)->green + src->green + (src + 1)->green + (pixel2 - 1)->green + pixel2->green + (pixel2 + 1)->green + (pixel3 - 1)->green + pixel3->green + (pixel3 + 1)->green) / 9);
            *(c++)= (char)(((src - 1)->blue + src->blue + (src + 1)->blue + (pixel2 - 1)->blue + pixel2->blue + (pixel2 + 1)->blue + (pixel3 - 1)->blue + pixel3->blue + (pixel3 + 1)->blue) / 9);
        }
        src += dim;
        c += 6;
    }
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smoothSharp(int dim, unsigned char *src, unsigned char *c) {
    int end = dim - 1, i, j, red, green, blue;
    src += 3;
    unsigned char *pixel2, *pixel3;

    for (i = end - 1; i > 0; i--) {
        pixel2 = src + DIM3;
        pixel3 = pixel2 + DIM3;
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
        c += 6;
    }
}

void smoothBlurFilter(int dim, pixel *src, char *c) {
    int end = dim - 1, i, j, min_intensity, max_intensity, red, green, blue;
    ++src;

    pixel *pixel1, *pixel2, *pixel3, *pixel4, *pixel5, *pixel6, *pixel7, *pixel8, *pixel9, *minp, *maxp;
    int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    for (i = end - 1; i > 0; --i) {

        pixel1 = src - 1;
        pixel2 = src;
        pixel3 = src + 1;
        pixel4 = pixel1 + dim;
        pixel5 = pixel4 + 1;
        pixel6 = pixel5 + 1;
        pixel7 = pixel4 + dim;
        pixel8 = pixel7 + 1;
        pixel9 = pixel8 + 1;

        sumRed1 = pixel1->red + pixel4->red + pixel7->red;
        sumGreen1 = pixel1->green + pixel4->green + pixel7->green;
        sumBlue1 = pixel1->blue + pixel4->blue + pixel7->blue;

        sumRed2 = pixel2->red + pixel5->red + pixel8->red;
        sumGreen2 = pixel2->green + pixel5->green + pixel8->green;
        sumBlue2 = pixel2->blue + pixel5->blue + pixel8->blue;

        sumRed3 = pixel3->red + pixel6->red + pixel9->red;
        sumGreen3 = pixel3->green + pixel6->green + pixel9->green;
        sumBlue3 = pixel3->blue + pixel6->blue + pixel9->blue;

        for (j = end - 1; j > 0; --j) {

            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            min_intensity = pixel1->sum;
            max_intensity = pixel1->sum;
            minp = pixel1;
            maxp = pixel1;

            if (pixel2->sum <= min_intensity) {
                min_intensity = pixel2->sum;
                minp = pixel2;
            }
            if (pixel2->sum > max_intensity) {
                max_intensity = pixel2->sum;
                maxp = pixel2;
            }

            if (pixel3->sum <= min_intensity) {
                min_intensity = pixel3->sum;
                minp = pixel3;
            }
            if (pixel3->sum > max_intensity) {
                max_intensity = pixel3->sum;
                maxp = pixel3;
            }

            if (pixel4->sum <= min_intensity) {
                min_intensity = pixel4->sum;
                minp = pixel4;
            }
            if (pixel4->sum > max_intensity) {
                max_intensity = pixel4->sum;
                maxp = pixel4;
            }

            if (pixel5->sum <= min_intensity) {
                min_intensity = pixel5->sum;
                minp = pixel5;
            }
            if (pixel5->sum > max_intensity) {
                max_intensity = pixel5->sum;
                maxp = pixel5;
            }

            if (pixel6->sum <= min_intensity) {
                min_intensity = pixel6->sum;
                minp = pixel6;
            }
            if (pixel6->sum > max_intensity) {
                max_intensity = pixel6->sum;
                maxp = pixel6;
            }

            if (pixel7->sum <= min_intensity) {
                min_intensity = pixel7->sum;
                minp = pixel7;
            }
            if (pixel7->sum > max_intensity) {
                max_intensity = pixel7->sum;
                maxp = pixel7;
            }

            if (pixel8->sum <= min_intensity) {
                min_intensity = pixel8->sum;
                minp = pixel8;
            }
            if (pixel8->sum > max_intensity) {
                max_intensity = pixel8->sum;
                maxp = pixel8;
            }

            if (pixel9->sum <= min_intensity)
                minp = pixel9;
            if (pixel9->sum > max_intensity)
                maxp = pixel9;


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

            ++pixel1;
            ++pixel2;
            ++pixel3;
            ++pixel4;
            ++pixel5;
            ++pixel6;
            ++pixel7;
            ++pixel8;
            ++pixel9;

            sumRed1 = sumRed2;
            sumGreen1 = sumGreen2;
            sumBlue1 = sumBlue2;

            sumRed2 = sumRed3;
            sumGreen2 = sumGreen3;
            sumBlue2 = sumBlue3;

            sumRed3 = pixel3->red + pixel6->red + pixel9->red;
            sumGreen3 = pixel3->green + pixel6->green + pixel9->green;
            sumBlue3 = pixel3->blue + pixel6->blue + pixel9->blue;

        }
        src += dim;
        c += 6;
    }
}

void charsToPixels(Image *charsImg, pixel* pixels) {
    char *data = image->data;
    int row, i;
    for (row = NM - 3; row > 0; row-=4) {
        pixels->red = *(data++);
        pixels->green = *(data++);
        pixels->blue = *(data++);
        ++pixels;
        pixels->red = *(data++);
        pixels->green = *(data++);
        pixels->blue = *(data++);
        ++pixels;
        pixels->red = *(data++);
        pixels->green = *(data++);
        pixels->blue = *(data++);
        ++pixels;
        pixels->red = *(data++);
        pixels->green = *(data++);
        pixels->blue = *(data++);
        ++pixels;
    }
    for (i = row - 1; i > 0; --i) {
        pixels->red = *(data++);
        pixels->green = *(data++);
        pixels->blue = *(data++);
        ++pixels;
    }
}


void charsToPixelsAndSum(Image *charsImg, pixel* pixels) {
    pixel *helpSum = pixels + m + 1;
    char *data = image->data;
    int row, i, j;

    // first row
    for (i = m - 3; i > 0; i -= 4) {
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

    for (j = i - 1; i > 0; --i) {
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
    for (i = m - 3; i > 0; i -= 4) {
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

    for (j = i - 1; i > 0; --i) {
        pixels->red = *data;
        ++data;
        pixels->green = *data;
        ++data;
        pixels->blue = *data;
        ++data;
        ++pixels;
    }
}

void imageToChars(unsigned char *data, unsigned char* c) {
    int i, j;
    for (i = NM3 - 5; i > 0; i-=6) {
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
    for (j = i - 1; i > 0; --j) {
        *c = *data;
        ++data;
        ++c;
    }
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    if (flag =='1') {
        char *chars = (unsigned char *) malloc(NM3);
        pixel *backupOrg = malloc(NM_PIXEL);
        charsToPixels(image, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlur(m, backupOrg, c);
        imageToChars(image->data, chars);
        writeBMP(image, srcImgpName, blurRsltImgName);
        smoothSharp(m, chars, c);
        writeBMP(image, srcImgpName, sharpRsltImgName);
        free(chars);
        free(backupOrg);
    } else {
        char *chars = (unsigned char *) malloc(NM3);
        pixel *backupOrg = malloc(NM_PIXEL);
        charsToPixelsAndSum(image, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlurFilter(m, backupOrg, c);
        imageToChars(image->data, chars);
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);
        smoothSharp(m, chars, c);
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
        free(chars);
        free(backupOrg);
    }
}
