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


void smoothBlur(pixel *src, char *c) {
    int end = m - 1, i, j, k;
    pixel *pixel1, *pixel2, *pixel3, *pixel4, *pixel5, *pixel6, *pixel7, *pixel8, *pixel9;
    int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    pixel1 = src - 1;
    pixel2 = src;
    pixel3 = src + 1;
    pixel4 = pixel1 + m;
    pixel5 = pixel4 + 1;
    pixel6 = pixel5 + 1;
    pixel7 = pixel4 + m;
    pixel8 = pixel7 + 1;
    pixel9 = pixel8 + 1;

    for (i = 1; i < end; ++i) {


        sumRed1 = pixel1->red + pixel4->red + pixel7->red;
        sumRed2 = pixel2->red + pixel5->red + pixel8->red;
        sumRed3 = pixel3->red + pixel6->red + pixel9->red;
        sumGreen1 = pixel1->green + pixel4->green + pixel7->green;
        sumGreen2 = pixel2->green + pixel5->green + pixel8->green;
        sumGreen3 = pixel3->green + pixel6->green + pixel9->green;
        sumBlue1 = pixel1->blue + pixel4->blue + pixel7->blue;
        sumBlue2 = pixel2->blue + pixel5->blue + pixel8->blue;
        sumBlue3 = pixel3->blue + pixel6->blue + pixel9->blue;

        for (j = 1; j < end - 2; j+=3) {

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

        for (k = j; k < end; ++k) {
            *(c++) = (char)((pixel1->red + pixel2->red + pixel3->red + pixel4->red + pixel5->red + pixel6->red + pixel7->red + pixel8->red + pixel9->red) / 9);
            *(c++) = (char)((pixel1->green + pixel2->green + pixel3->green + pixel4->green + pixel5->green + pixel6->green + pixel7->green + pixel8->green + pixel9->green) / 9);
            *(c++)= (char)((pixel1->blue + pixel2->blue + pixel3->blue + pixel4->blue + pixel5->blue + pixel6->blue + pixel7->blue + pixel8->blue + pixel9->blue) / 9);

            ++pixel1;
            ++pixel2;
            ++pixel3;
            ++pixel4;
            ++pixel5;
            ++pixel6;
            ++pixel7;
            ++pixel8;
            ++pixel9;
        }

        pixel1 += 2;
        pixel2 += 2;
        pixel3 += 2;
        pixel4 += 2;
        pixel5 += 2;
        pixel6 += 2;
        pixel7 += 2;
        pixel8 += 2;
        pixel9 += 2;
        c += 6;
    }
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smoothSharp(unsigned char *src, unsigned char *c) {
    int end = m - 1, i, j, red, green, blue;
    register unsigned char *pixel2 = src + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3;

    for (i = 1; i < end; i++) {
        for (j = 1; j < end; j++) {

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

void smoothBlurFilter(unsigned char *pixel1, unsigned char *c) {
    int end = m - 1, i, j, min_intensity, max_intensity, red, green, blue;

    unsigned char *pixel2, *pixel3, *minp, *maxp;
    int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3, sum;

    pixel2 = pixel1 + DIM3;
    pixel3 = pixel2 + DIM3;

    for (i = 1; i < end; ++i) {
        sumRed1 = *(pixel1 - 3) + *(pixel2 - 3) + *(pixel3 - 3);
        sumGreen1 = *(pixel1 - 2) + *(pixel2 - 2) + *(pixel3 - 2);
        sumBlue1 = *(pixel1 - 1) + *(pixel2 - 1) + *(pixel3 - 1);

        sumRed2 = *pixel1 + *pixel2+ *pixel3;
        sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
        sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);

        sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
        sumGreen3 = *(pixel1 + 4) + *(pixel2 + 4) + *(pixel3 + 4);
        sumBlue3 = *(pixel1 + 5) + *(pixel2 + 5) + *(pixel3 + 5);

        for (j = 1; j < end; ++j) {

            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            sum = *(pixel1 - 3) + *(pixel1 - 2) + *(pixel1 - 1);
            min_intensity = sum;
            max_intensity = sum;
            minp = pixel1 - 3;
            maxp = pixel1 - 3;

            sum = *pixel1 + *(pixel1 + 1) + *(pixel1 + 2);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel1;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel1;
            }

            sum = *(pixel1 + 3) + *(pixel1 + 4) + *(pixel1 + 5);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel1 + 3;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel1 + 3;
            }

            sum = *(pixel2 - 3) + *(pixel2 - 2) + *(pixel2 - 1);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel2 - 3;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel2 - 3;
            }

            sum = *pixel2 + *(pixel2 + 1) + *(pixel2 + 2);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel2;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel2;
            }

            sum = *(pixel2 + 3) + *(pixel2 + 4) + *(pixel2 + 5);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel2 + 3;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel2 + 3;
            }

            sum = *(pixel3 - 3) + *(pixel3 - 2) + *(pixel3 - 1);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel3 - 3;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel3 - 3;
            }

            sum = *pixel3 + *(pixel3 + 1) + *(pixel3 + 2);
            if (sum <= min_intensity) {
                min_intensity = sum;
                minp = pixel3;
            }
            if (sum > max_intensity) {
                max_intensity = sum;
                maxp = pixel3;
            }

            sum = *(pixel3 + 3) + *(pixel3 + 4) + *(pixel3 + 5);
            if (sum <= min_intensity)
                minp = pixel3 + 3;
            if (sum > max_intensity)
                maxp = pixel3 + 3;


            // divide by kernel's weight
            red = (red - *minp - *maxp) / 7;
            green = (green - *(minp + 1) - *(maxp + 1)) / 7;
            blue = (blue - *(minp + 2) - *(maxp + 2)) / 7;

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

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;

            sumRed1 = sumRed2;
            sumGreen1 = sumGreen2;
            sumBlue1 = sumBlue2;

            sumRed2 = sumRed3;
            sumGreen2 = sumGreen3;
            sumBlue2 = sumBlue3;

            sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen3 = *(pixel1 + 4) + *(pixel2 + 4) + *(pixel3 + 4);
            sumBlue3 = *(pixel1 + 5) + *(pixel2 + 5) + *(pixel3 + 5);

        }

        pixel1 += 6;
        pixel2 += 6;
        pixel3 += 6;
        c += 6;
    }
}

void charsToPixels(Image *charsImg, pixel* pixels) {
    char *data = image->data;
    int row, i;
    for (row = 0; row < NM - 3; row+=4) {
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
    for (i = row; i < NM; ++i) {
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

void imageToChars(unsigned char *data, unsigned char* c) {
    int i, j;
    for (i = 0; i < NM3 - 5; i+=6) {
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
    for (j = i; i < NM3; ++j) {
        *c = *data;
        ++data;
        ++c;
    }
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    if (flag =='1') {
        char *chars = malloc(NM3);
        pixel *backupOrg = malloc(NM_PIXEL);
        charsToPixels(image, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlur(backupOrg + 1, c);
        imageToChars(image->data, chars);
        writeBMP(image, srcImgpName, blurRsltImgName);
        smoothSharp(chars + 3, c);
        writeBMP(image, srcImgpName, sharpRsltImgName);
        free(chars);
        free(backupOrg);
    } else {
        char *backupOrg = malloc(NM3);
        imageToChars(image->data, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlurFilter(backupOrg + 3, c);
        imageToChars(image->data, backupOrg);
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);
        smoothSharp(backupOrg + 3, c);
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
        free(backupOrg);
    }
}
