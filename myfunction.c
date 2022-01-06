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


// Apply the blur kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothBlur(register unsigned char *pixel1, register char *c) {
    // save the pointers to 3 pixels in registers
    register unsigned char *pixel2 = pixel1 + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3;

    // save sums of 3 pixels in column for every color for 3 columns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3, end = m - 1, j, i, k;

    // blur every pixel in each row
    for (i = end - 1; i > 0; --i) {

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = *(pixel1 - 3) + *(pixel2 - 3) + *(pixel3 - 3);
        sumRed2 = *pixel1 + *pixel2 + *pixel3;
        sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
        sumGreen1 = *(pixel1 - 2) + *((pixel2 - 2)) + *((pixel3 - 2));
        sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
        sumGreen3 = *((pixel1 + 4)) + *((pixel2 + 4)) + *((pixel3 + 4));
        sumBlue1 = *(pixel1 - 1) + *((pixel2 - 1)) + *((pixel3 - 1));
        sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);
        sumBlue3 = *((pixel1 + 5)) + *((pixel2 + 5)) + *((pixel3 + 5));

        // blur every pixel in each column
        for (j = 1; j < end - 2; j+=3) {

            // update the colors of the first pixel
            *++c = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed1 = *(pixel1 + 6) + *(pixel2 + 6) + *(pixel3 + 6);
            sumGreen1 = *(pixel1 + 7) + *((pixel2 + 7)) + *(pixel3 + 7);
            sumBlue1 = *(pixel1 + 8) + *((pixel2 + 8)) + *(pixel3 + 8);

            // update the colors of the second pixel
            *++c = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            // go to the next 3 pixels
            pixel1 += 9;
            pixel2 += 9;
            pixel3 += 9;

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed2 = *pixel1 + *pixel2 + *pixel3;
            sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
            sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);

            // update the colors of the third pixel
            *++c = (char)((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = (char)((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = (char)((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen3 = *((pixel1 + 4)) + *((pixel2 + 4)) + *((pixel3 + 4));
            sumBlue3 = *((pixel1 + 5)) + *((pixel2 + 5)) + *((pixel3 + 5));
        }

        // the last pixels in the row if the number doesnt divide by 3
        for (k = j; k < end; ++k) {
            *++c = (char)((*(pixel1 - 3) + *pixel1 + *(pixel1 + 3) + *(pixel2 - 3) + *pixel2 + *(pixel2 + 3) + *(pixel3 - 3) + *pixel3 + *(pixel3 + 3)) / 9);
            *++c = (char)((*(pixel1 - 2) + *(pixel1 + 1) + *((pixel1 + 4)) + *((pixel2 - 2)) + *(pixel2 + 1) + *((pixel2 + 4)) + *((pixel3 - 2)) + *(pixel3 + 1) + *((pixel3 + 4))) / 9);
            *++c = (char)((*(pixel1 - 1) + *(pixel1 + 2) + *((pixel1 + 5)) + *((pixel2 - 1)) + *(pixel2 + 2) + *((pixel2 + 5)) + *((pixel3 - 1)) + *(pixel3 + 2) + *((pixel3 + 5))) / 9);

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
        }

        // go to the next start of row
        pixel1 += 6;
        pixel2 += 6;
        pixel3 += 6;
        c += 6;
    }
}


// Apply the sharp kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothSharp(register unsigned char *pixel1, unsigned char *c) {

    // save the pointers to 3 pixels in registers
    register unsigned char *pixel2 = pixel1 + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3;

    // save sums of 3 pixels in column for every color for 3 columns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3, end = m - 1, j, i, k, sum;

    // sharp every pixel in each row
    for (i = end - 1; i > 0; --i) {

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = -*(pixel1 - 3) - *(pixel2 - 3) - *(pixel3 - 3);
        sumRed2 = -*pixel1 - *pixel2 - *pixel3;
        sumRed3 = -*(pixel1 + 3) - *(pixel2 + 3) - *(pixel3 + 3);
        sumGreen1 = -*(pixel1 - 2) - *((pixel2 - 2)) - *((pixel3 - 2));
        sumGreen2 = -*(pixel1 + 1) - *(pixel2 + 1) - *(pixel3 + 1);
        sumGreen3 = -*((pixel1 + 4)) - *((pixel2 + 4)) - *((pixel3 + 4));
        sumBlue1 = -*(pixel1 - 1) - *((pixel2 - 1)) - *((pixel3 - 1));
        sumBlue2 = -*(pixel1 + 2) - *(pixel2 + 2) - *(pixel3 + 2);
        sumBlue3 = -*((pixel1 + 5)) - *((pixel2 + 5)) - *((pixel3 + 5));

        // sharp every pixel in each column
        for (j = 1; j < end - 2; j += 3) {

            // update the colors of the first pixel
            sum = ((sumRed1 + sumRed2 + sumRed3) + 10 * *pixel2);
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 + 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 + 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed1 = -*(pixel1 + 6) - *(pixel2 + 6) - *(pixel3 + 6);
            sumGreen1 = -*(pixel1 + 7) - *((pixel2 + 7)) - *(pixel3 + 7);
            sumBlue1 = -*(pixel1 + 8) - *((pixel2 + 8)) - *(pixel3 + 8);

            // update the colors of the second pixel
            sum =  ((sumRed1 + sumRed2 + sumRed3) + 10 * *(pixel2 + 3));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 + 4));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 + 5));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            // go to the next 3 pixels
            pixel1 += 9;
            pixel2 += 9;
            pixel3 += 9;

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed2 = -*pixel1 - *pixel2 - *pixel3;
            sumGreen2 = -*(pixel1 + 1) - *(pixel2 + 1) - *(pixel3 + 1);
            sumBlue2 = -*(pixel1 + 2) - *(pixel2 + 2) - *(pixel3 + 2);

            // update the colors of the third pixel
            sum =  ((sumRed1 + sumRed2 + sumRed3) + 10 * *(pixel2 - 3));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 - 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 - 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed3 = -*(pixel1 + 3) - *(pixel2 + 3) - *(pixel3 + 3);
            sumGreen3 = -*((pixel1 + 4)) - *((pixel2 + 4)) - *((pixel3 + 4));
            sumBlue3 = -*((pixel1 + 5)) - *((pixel2 + 5)) - *((pixel3 + 5));
        }

        // the last pixels in the row if the number doesnt divide by 3
        for (k = j; k < end; ++k) {
            sum = (char) (-(*(pixel1 - 3) + *pixel1 + *(pixel1 + 3) + *(pixel2 - 3) + *pixel2 + *(pixel2 + 3) +
                            *(pixel3 - 3) + *pixel3 + *(pixel3 + 3)) + 10 * *pixel2);
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = (char) (-(*(pixel1 - 2) + *(pixel1 + 1) + *((pixel1 + 4)) + *((pixel2 - 2)) + *(pixel2 + 1) +
                            *((pixel2 + 4)) + *((pixel3 - 2)) + *(pixel3 + 1) + *((pixel3 + 4))) + 10 * *(pixel2 + 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = (char) (-(*(pixel1 - 1) + *(pixel1 + 2) + *((pixel1 + 5)) + *((pixel2 - 1)) + *(pixel2 + 2) +
                            *((pixel2 + 5)) + *((pixel3 - 1)) + *(pixel3 + 2) + *((pixel3 + 5))) + 10 * *(pixel2 + 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
        }

        // go to the next start of row
        pixel1 += 6;
        pixel2 += 6;
        pixel3 += 6;
        c += 6;
    }
}

// Apply the blur filter kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothBlurFilter(register pixel *pixel2, register char *c) {
    register int end = m - 1, i, j, min_intensity, max_intensity, red, green, blue;

    // save the pointers to 3 pixels in registers
    register pixel *pixel5 = pixel2 + m;
    register pixel *pixel8 = pixel5 + m;
    register pixel *minp, *maxp;

    // save sums of 3 pixels in column for every color for 3 colomns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    // blur every pixel in each column
    for (i = end - 1; i > 0; --i) {

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = (pixel2 - 1)->red + (pixel5 - 1)->red + (pixel8 - 1)->red;
        sumGreen1 = (pixel2 - 1)->green + (pixel5 - 1)->green + (pixel8 - 1)->green;
        sumBlue1 = (pixel2 - 1)->blue + (pixel5 - 1)->blue + (pixel8 - 1)->blue;

        sumRed2 = pixel2->red + pixel5->red + pixel8->red;
        sumGreen2 = pixel2->green + pixel5->green + pixel8->green;
        sumBlue2 = pixel2->blue + pixel5->blue + pixel8->blue;

        sumRed3 = (pixel2 + 1)->red + (pixel5 + 1)->red + (pixel8 + 1)->red;
        sumGreen3 = (pixel2 + 1)->green + (pixel5 + 1)->green + (pixel8 + 1)->green;
        sumBlue3 = (pixel2 + 1)->blue + (pixel5 + 1)->blue + (pixel8 + 1)->blue;

        // blur every pixel in each column
        for (j = end - 1; j > 0; --j) {

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // save the minimum and maximum pixel as the first
            min_intensity = (pixel2 - 1)->sum;
            max_intensity = (pixel2 - 1)->sum;
            minp = (pixel2 - 1);
            maxp = (pixel2 - 1);

            // then check every pixel and update the min and max in accordance
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
                *++c = red;
            else
                *++c = 255;

            if (green <= 255)
                *++c = green;
            else
                *++c = 255;

            if (blue <= 255)
                *++c = blue;
            else
                *++c = 255;

            // go to the next pixel
            ++pixel2;
            ++pixel5;
            ++pixel8;

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
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

        // go to the next row to the second pixel
        pixel2 += 2;
        pixel5 += 2;
        pixel8 += 2;
        c += 6;
    }
}


// copy the pixels of the image to the array of pixels
void charsToPixelsAndSum(Image *charsImg, register pixel* pixels) {

    // save the data in a register
    register char *data = image->data - 1;
    register int i;
\
    // copy the pixels in the image to pixels
    for (i = 0; i < NM - 3; i += 4) {
        pixels->red = *++data;
        pixels->green = *++data;
        pixels->blue = *++data;
        pixels->sum = pixels->red + pixels->green + pixels->blue;
        ++pixels;

        pixels->red = *++data;
        pixels->green = *++data;
        pixels->blue = *++data;
        pixels->sum = pixels->red + pixels->green + pixels->blue;
        ++pixels;

        pixels->red = *++data;
        pixels->green = *++data;
        pixels->blue = *++data;
        pixels->sum = pixels->red + pixels->green + pixels->blue;
        ++pixels;

        pixels->red = *++data;
        pixels->green = *++data;
        pixels->blue = *++data;
        pixels->sum = pixels->red + pixels->green + pixels->blue;
        ++pixels;
    }

    int j;
    // copy the last pixels of the image
    for (j = i; i < NM; ++i) {
        pixels->red = *++data;
        pixels->green = *++data;
        pixels->blue = *++data;
        pixels->sum = pixels->red + pixels->green + pixels->blue;
        ++pixels;
    }
}

// copy the pixels of the image to the array of chars
void imageToChars(register unsigned char *data, register unsigned char* c) {
    int i, j;
    --c;
    --data;
    // copy every pixel in the data to the chars array in loop unrolling
    for (i = 0; i < NM3 - 5; i+=6) {
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
    }

    // copy the last pixels of the data to the chars array
    for (j = i; j < NM3; ++j)
        *++c = *++data;
}


// blur, sharp or filter the image
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    // no filter
    if (flag == '1') {

        // copy the data to back-up array of chars
        char *backupOrg = malloc(NM3);
        imageToChars(image->data, backupOrg);

        // call to the smooth blur function from the second pixel in the first row of the image
        char *c = image->data + M3PLUS3;
        smoothBlur(backupOrg + 3, c - 1);

        // copy the data to back-up array of chars and write the blur image
        imageToChars(image->data, backupOrg);
        writeBMP(image, srcImgpName, blurRsltImgName);

        // call to the smooth sharp function from the second pixel in the first row of the image and write
        // the sharped image and free the memory we allocated
        smoothSharp(backupOrg + 3, c - 1);
        writeBMP(image, srcImgpName, sharpRsltImgName);
        free(backupOrg);
    }

        // filter
    else {

        // copy the data to back-up array of pixels
        pixel *backupOrg = malloc(NM_PIXEL);
        charsToPixelsAndSum(image, backupOrg);

        // call to the smooth blur filter function from the second pixel in the first row of the image
        char *c = image->data + M3PLUS3;
        smoothBlurFilter(backupOrg + 1, c - 1);

        // copy the data to back-up array of chars and write the filtered blur image
        char *chars = (unsigned char *) malloc(NM3);
        imageToChars(image->data, chars);
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // call to the smooth sharp function from the second pixel in the first row of the image and write
        // the filtered sharped image and free the memory we allocated
        smoothSharp(chars + 3, c - 1);
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
        free(chars);
        free(backupOrg);
    }
}