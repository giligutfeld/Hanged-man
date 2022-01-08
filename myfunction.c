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


// Apply the sharp kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothSharp(register unsigned char *pixel1, unsigned char *c) {

    // save the pointers to 3 pixels in registers
    register unsigned char *pixel2 = pixel1 + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3 + 3;
    pixel1+=3;

    // save sums of 3 pixels in column for every color for 3 columns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3, k, sum, end = m - 3, i , j;

    // sharp every pixel in each row
    for (i = end + 1; i > 0; --i) {

        // the first pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

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
        for (j = end; j > 1; j -= 3) {

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
        for (k = j + 1; k > 0; --k) {
            sum = (-(*(pixel1 - 3) + *pixel1 + *(pixel1 + 3) + *(pixel2 - 3) + *(pixel2 + 3) +
                     *(pixel3 - 3) + *pixel3 + *(pixel3 + 3)) + 9 * *pixel2);
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = (-(*(pixel1 - 2) + *(pixel1 + 1) + *((pixel1 + 4)) + *((pixel2 - 2)) +
                     *((pixel2 + 4)) + *((pixel3 - 2)) + *(pixel3 + 1) + *((pixel3 + 4))) + 9 * *(pixel2 + 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = (-(*(pixel1 - 1) + *(pixel1 + 2) + *((pixel1 + 5)) + *((pixel2 - 1)) +
                     *((pixel2 + 5)) + *((pixel3 - 1)) + *(pixel3 + 2) + *((pixel3 + 5))) + 9 * *(pixel2 + 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
        }

        // the last pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

        pixel1 += 6;
        pixel3 += 6;
    }
}

// Apply the blur kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothBlur(register unsigned char *pixel2, register unsigned char *c) {

    register int i;
    for (i = 0; i < DIM3; ++i)
        *++c = *(pixel2++);

    // save the pointers to 3 pixels in registers
    register unsigned char *pixel1 = pixel2 - DIM3 + 3;
    register unsigned char *pixel3 = pixel2 + DIM3 + 3;

    // save sums of 3 pixels in column for every color for 3 columns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3,
            j, k, end = m - 3;

    // blur every pixel in each row
    for (i = end + 1; i > 0; --i) {

        // the first pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

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
        for (j = end; j > 1; j -= 3) {

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
        for (k = j + 1; k > 0; --k) {
            *++c = (char)((*(pixel1 - 3) + *pixel1 + *(pixel1 + 3) + *(pixel2 - 3) + *pixel2 + *(pixel2 + 3) + *(pixel3 - 3) + *pixel3 + *(pixel3 + 3)) / 9);
            *++c = (char)((*(pixel1 - 2) + *(pixel1 + 1) + *((pixel1 + 4)) + *((pixel2 - 2)) + *(pixel2 + 1) + *((pixel2 + 4)) + *((pixel3 - 2)) + *(pixel3 + 1) + *((pixel3 + 4))) / 9);
            *++c = (char)((*(pixel1 - 1) + *(pixel1 + 2) + *((pixel1 + 5)) + *((pixel2 - 1)) + *(pixel2 + 2) + *((pixel2 + 5)) + *((pixel3 - 1)) + *(pixel3 + 2) + *((pixel3 + 5))) / 9);

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
        }

        // the last pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

        pixel1 += 6;
        pixel3 += 6;
    }

    for (i = 0; i < DIM3; ++i)
        *++c = *(pixel2++);
}


// Apply the blur filter kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothBlurFilter(register pixel *pixel1, register char *c) {
    register int i, j, min_intensity, max_intensity, red, green, blue, end = m - 3;

    // save the pointers to 3 pixels in registers
    register pixel *pixel2 = pixel1 + m;
    register pixel *pixel3 = pixel2 + m;
    register pixel *minp, *maxp;


    // save sums of 3 pixels in column for every color for 3 colomns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3;

    // blur every pixel in each column
    for (i = end + 1; i > 0; --i) {

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = (pixel1 - 1)->red + (pixel2 - 1)->red + (pixel3 - 1)->red;
        sumGreen1 = (pixel1 - 1)->green + (pixel2 - 1)->green + (pixel3 - 1)->green;
        sumBlue1 = (pixel1 - 1)->blue + (pixel2 - 1)->blue + (pixel3 - 1)->blue;

        sumRed2 = pixel1->red + pixel2->red + pixel3->red;
        sumGreen2 = pixel1->green + pixel2->green + pixel3->green;
        sumBlue2 = pixel1->blue + pixel2->blue + pixel3->blue;

        sumRed3 = (pixel1 + 1)->red + (pixel2 + 1)->red + (pixel3 + 1)->red;
        sumGreen3 = (pixel1 + 1)->green + (pixel2 + 1)->green + (pixel3 + 1)->green;
        sumBlue3 = (pixel1 + 1)->blue + (pixel2 + 1)->blue + (pixel3 + 1)->blue;

        // blur every pixel in each column
        for (j = end; j > 1; j -= 3) {

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // save the minimum and maximum pixel as the first
            min_intensity = (pixel1 - 1)->sum;
            max_intensity = (pixel1 - 1)->sum;
            minp = (pixel1 - 1);
            maxp = (pixel1 - 1);

            // then check every pixel and update the min and max in accordance
            if (pixel1->sum <= min_intensity) {
                min_intensity = pixel1->sum;
                minp = pixel1;
            }
            if (pixel1->sum > max_intensity) {
                max_intensity = pixel1->sum;
                maxp = pixel1;
            }

            if ((pixel1 + 1)->sum <= min_intensity) {
                min_intensity = (pixel1 + 1)->sum;
                minp = (pixel1 + 1);
            }
            if ((pixel1 + 1)->sum > max_intensity) {
                max_intensity = (pixel1 + 1)->sum;
                maxp = (pixel1 + 1);
            }

            if ((pixel2 - 1)->sum <= min_intensity) {
                min_intensity = (pixel2 - 1)->sum;
                minp = (pixel2 - 1);
            }
            if ((pixel2 - 1)->sum > max_intensity) {
                max_intensity = (pixel2 - 1)->sum;
                maxp = (pixel2 - 1);
            }

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

            if ((pixel3 - 1)->sum <= min_intensity) {
                min_intensity = (pixel3 - 1)->sum;
                minp = (pixel3 - 1);
            }
            if ((pixel3 - 1)->sum > max_intensity) {
                max_intensity = (pixel3 - 1)->sum;
                maxp = (pixel3 - 1);
            }

            if (pixel3->sum <= min_intensity) {
                min_intensity = pixel3->sum;
                minp = pixel3;
            }
            if (pixel3->sum > max_intensity) {
                max_intensity = pixel3->sum;
                maxp = pixel3;
            }

            if ((pixel3 + 1)->sum <= min_intensity)
                minp = (pixel3 + 1);
            if ((pixel3 + 1)->sum > max_intensity)
                maxp = (pixel3 + 1);


            // divide by kernel's weight
            red = (red - minp->red - maxp->red) / 7;
            green = (green - minp->green - maxp->green) / 7;
            blue = (blue - minp->blue - maxp->blue) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            ++pixel1;
            ++pixel2;
            ++pixel3;

            sumRed1 = (pixel1 + 1)->red + (pixel2 + 1)->red + (pixel3 + 1)->red;
            sumGreen1 = (pixel1 + 1)->green + (pixel2 + 1)->green + (pixel3 + 1)->green;
            sumBlue1 = (pixel1 + 1)->blue + (pixel2 + 1)->blue + (pixel3 + 1)->blue;

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // save the minimum and maximum pixel as the first
            min_intensity = (pixel1 - 1)->sum;
            max_intensity = (pixel1 - 1)->sum;
            minp = (pixel1 - 1);
            maxp = (pixel1 - 1);

            // then check every pixel and update the min and max in accordance
            if (pixel1->sum <= min_intensity) {
                min_intensity = pixel1->sum;
                minp = pixel1;
            }
            if (pixel1->sum > max_intensity) {
                max_intensity = pixel1->sum;
                maxp = pixel1;
            }

            if ((pixel1 + 1)->sum <= min_intensity) {
                min_intensity = (pixel1 + 1)->sum;
                minp = (pixel1 + 1);
            }
            if ((pixel1 + 1)->sum > max_intensity) {
                max_intensity = (pixel1 + 1)->sum;
                maxp = (pixel1 + 1);
            }

            if ((pixel2 - 1)->sum <= min_intensity) {
                min_intensity = (pixel2 - 1)->sum;
                minp = (pixel2 - 1);
            }
            if ((pixel2 - 1)->sum > max_intensity) {
                max_intensity = (pixel2 - 1)->sum;
                maxp = (pixel2 - 1);
            }

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

            if ((pixel3 - 1)->sum <= min_intensity) {
                min_intensity = (pixel3 - 1)->sum;
                minp = (pixel3 - 1);
            }
            if ((pixel3 - 1)->sum > max_intensity) {
                max_intensity = (pixel3 - 1)->sum;
                maxp = (pixel3 - 1);
            }

            if (pixel3->sum <= min_intensity) {
                min_intensity = pixel3->sum;
                minp = pixel3;
            }
            if (pixel3->sum > max_intensity) {
                max_intensity = pixel3->sum;
                maxp = pixel3;
            }

            if ((pixel3 + 1)->sum <= min_intensity)
                minp = (pixel3 + 1);
            if ((pixel3 + 1)->sum > max_intensity)
                maxp = (pixel3 + 1);


            // divide by kernel's weight
            red = (red - minp->red - maxp->red) / 7;
            green = (green - minp->green - maxp->green) / 7;
            blue = (blue - minp->blue - maxp->blue) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            ++pixel1;
            ++pixel2;
            ++pixel3;

            sumRed2 = (pixel1 + 1)->red + (pixel2 + 1)->red + (pixel3 + 1)->red;
            sumGreen2 = (pixel1 + 1)->green + (pixel2 + 1)->green + (pixel3 + 1)->green;
            sumBlue2 = (pixel1 + 1)->blue + (pixel2 + 1)->blue + (pixel3 + 1)->blue;

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // save the minimum and maximum pixel as the first
            min_intensity = (pixel1 - 1)->sum;
            max_intensity = (pixel1 - 1)->sum;
            minp = (pixel1 - 1);
            maxp = (pixel1 - 1);

            // then check every pixel and update the min and max in accordance
            if (pixel1->sum <= min_intensity) {
                min_intensity = pixel1->sum;
                minp = pixel1;
            }
            if (pixel1->sum > max_intensity) {
                max_intensity = pixel1->sum;
                maxp = pixel1;
            }

            if ((pixel1 + 1)->sum <= min_intensity) {
                min_intensity = (pixel1 + 1)->sum;
                minp = (pixel1 + 1);
            }
            if ((pixel1 + 1)->sum > max_intensity) {
                max_intensity = (pixel1 + 1)->sum;
                maxp = (pixel1 + 1);
            }

            if ((pixel2 - 1)->sum <= min_intensity) {
                min_intensity = (pixel2 - 1)->sum;
                minp = (pixel2 - 1);
            }
            if ((pixel2 - 1)->sum > max_intensity) {
                max_intensity = (pixel2 - 1)->sum;
                maxp = (pixel2 - 1);
            }

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

            if ((pixel3 - 1)->sum <= min_intensity) {
                min_intensity = (pixel3 - 1)->sum;
                minp = (pixel3 - 1);
            }
            if ((pixel3 - 1)->sum > max_intensity) {
                max_intensity = (pixel3 - 1)->sum;
                maxp = (pixel3 - 1);
            }

            if (pixel3->sum <= min_intensity) {
                min_intensity = pixel3->sum;
                minp = pixel3;
            }
            if (pixel3->sum > max_intensity) {
                max_intensity = pixel3->sum;
                maxp = pixel3;
            }

            if ((pixel3 + 1)->sum <= min_intensity)
                minp = (pixel3 + 1);
            if ((pixel3 + 1)->sum > max_intensity)
                maxp = (pixel3 + 1);


            // divide by kernel's weight
            red = (red - minp->red - maxp->red) / 7;
            green = (green - minp->green - maxp->green) / 7;
            blue = (blue - minp->blue - maxp->blue) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            ++pixel1;
            ++pixel2;
            ++pixel3;

            sumRed3 = (pixel1 + 1)->red + (pixel2 + 1)->red + (pixel3 + 1)->red;
            sumGreen3 = (pixel1 + 1)->green + (pixel2 + 1)->green + (pixel3 + 1)->green;
            sumBlue3 = (pixel1 + 1)->blue + (pixel2 + 1)->blue + (pixel3 + 1)->blue;
        }

        int k;
        // the last pixels in the row if the number doesnt divide by 3
        for (k = j + 1; k > 0; --k) {
            // save sums of each color of the pixel
            red = pixel1->red + pixel2->red + pixel3->red + (pixel1 + 1)->red + (pixel2 + 1)->red + (pixel3 + 1)->red + (pixel1 - 1)->red + (pixel2 - 1)->red + (pixel3 - 1)->red;
            green = pixel1->green + pixel2->green + pixel3->green + (pixel1 + 1)->green + (pixel2 + 1)->green + (pixel3 + 1)->green + (pixel1 - 1)->green + (pixel2 - 1)->green + (pixel3 - 1)->green;
            blue = (pixel1 - 1)->blue + (pixel2 - 1)->blue + (pixel3 - 1)->blue + pixel1->blue + pixel2->blue + pixel3->blue + (pixel1 + 1)->blue + (pixel2 + 1)->blue + (pixel3 + 1)->blue;

            // save the minimum and maximum pixel as the first
            min_intensity = (pixel1 - 1)->sum;
            max_intensity = (pixel1 - 1)->sum;
            minp = (pixel1 - 1);
            maxp = (pixel1 - 1);

            // then check every pixel and update the min and max in accordance
            if (pixel1->sum <= min_intensity) {
                min_intensity = pixel1->sum;
                minp = pixel1;
            }
            if (pixel1->sum > max_intensity) {
                max_intensity = pixel1->sum;
                maxp = pixel1;
            }

            if ((pixel1 + 1)->sum <= min_intensity) {
                min_intensity = (pixel1 + 1)->sum;
                minp = (pixel1 + 1);
            }
            if ((pixel1 + 1)->sum > max_intensity) {
                max_intensity = (pixel1 + 1)->sum;
                maxp = (pixel1 + 1);
            }

            if ((pixel2 - 1)->sum <= min_intensity) {
                min_intensity = (pixel2 - 1)->sum;
                minp = (pixel2 - 1);
            }
            if ((pixel2 - 1)->sum > max_intensity) {
                max_intensity = (pixel2 - 1)->sum;
                maxp = (pixel2 - 1);
            }

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

            if ((pixel3 - 1)->sum <= min_intensity) {
                min_intensity = (pixel3 - 1)->sum;
                minp = (pixel3 - 1);
            }
            if ((pixel3 - 1)->sum > max_intensity) {
                max_intensity = (pixel3 - 1)->sum;
                maxp = (pixel3 - 1);
            }

            if (pixel3->sum <= min_intensity) {
                min_intensity = pixel3->sum;
                minp = pixel3;
            }
            if (pixel3->sum > max_intensity) {
                max_intensity = pixel3->sum;
                maxp = pixel3;
            }

            if ((pixel3 + 1)->sum <= min_intensity)
                minp = (pixel3 + 1);
            if ((pixel3 + 1)->sum > max_intensity)
                maxp = (pixel3 + 1);


            // divide by kernel's weight
            red = (red - minp->red - maxp->red) / 7;
            green = (green - minp->green - maxp->green) / 7;
            blue = (blue - minp->blue - maxp->blue) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            ++pixel1;
            ++pixel2;
            ++pixel3;
        }

        // go to the next row to the second pixel
        pixel1 += 2;
        pixel2 += 2;
        pixel3 += 2;
        c += 6;
    }
}




// copy the pixels of the image to the array of pixels
void charsToPixelsAndSum(register char *data, register pixel* pixels) {

    register int i;
    register int end = NM -7;

    // copy the pixels in the image to pixels
    for (i = 0; i < end; i += 8) {
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

    register int j;
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
    register int i;
    // copy every pixel in the data to the chars array in loop unrolling
    for (i = 0; i < NM3 - 7; i += 8) {
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
        *++c = *++data;
    }

    register int j;
    // copy the last pixels of the data to the chars array
    for (j = i; j < NM3; ++j) {
        *++c = *++data;
    }
}


// blur, sharp or filter the image
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    // no filter
    if (flag == '1') {

        // copy the first row of the data to array of chars
        char *newData = malloc(NM3);
        char *oldData = image->data;

        // call to the smooth blur function from the second pixel in the first row of the image
        smoothBlur(oldData, newData - 1);

        oldData = image->data;
        image->data = newData;

        // copy the data to back-up array of chars and write the blur image
        writeBMP(image, srcImgpName, blurRsltImgName);

        // call to the smooth sharp function from the second pixel in the first row of the image and write
        // the sharped image and free the memory we allocated
        smoothSharp(newData, oldData - 1 + DIM3);
        image->data = oldData;
        writeBMP(image, srcImgpName, sharpRsltImgName);

        free(newData);
    }

        // filter
    else {
        // copy the data to back-up array of pixels
        pixel *backupOrg = malloc(NM_PIXEL);
        charsToPixelsAndSum(image->data - 1, backupOrg);

        // call to the smooth blur filter function from the second pixel in the first row of the image
        char *c = image->data + M3PLUS3;
        smoothBlurFilter(backupOrg + 1, c - 1);

        // copy the data to back-up array of chars and write the filtered blur image
        char *chars = (unsigned char *) malloc(NM3);
        imageToChars(image->data - 1, chars - 1);
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // call to the smooth sharp function from the second pixel in the first row of the image and write
        // the filtered sharped image and free the memory we allocated
        smoothSharp(chars, image->data - 1 + DIM3);
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
        free(chars);
        free(backupOrg);
    }
}
