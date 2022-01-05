#include <stdbool.h>

#define NM n * m
#define NM3 n * m * 3
#define M3PLUS3 3 * m + 3
#define DIM3 m * 3


void smoothBlur(unsigned char *pixel1, char *c) {
    int end = m - 1, i, j, k;
    unsigned char *pixel2, *pixel3;
    int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    pixel2 = pixel1 + DIM3;
    pixel3 = pixel2 + DIM3;

    for (i = 1; i < end; ++i) {

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
        char *backupOrg = malloc(NM3);
        imageToChars(image->data, backupOrg);
        char *c = image->data + M3PLUS3;
        smoothBlur(backupOrg + 3, c);
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
