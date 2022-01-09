#include <stdbool.h>

#define NM n * m
#define NM3 n * m * 3
#define NM4 n * m * sizeof (int)
#define DIM3 m * 3

// Apply the sharp kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothSharp(register unsigned char *pixel1, unsigned char *c) {

    // save the pointers to 3 pixels in registers
    register unsigned char *pixel2 = pixel1 + DIM3;
    register unsigned char *pixel3 = pixel2 + DIM3;

    // save sums of 3 pixels in column for every color for 3 columns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3,
    sum, i, end = m - 3;

    // sharp every pixel in each row
    for (i = end + 1; i > 0; --i) {

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = -*pixel1 - *pixel2 - *pixel3;
        sumRed2 = -*(pixel1 + 3) - *(pixel2 + 3) - *(pixel3 + 3);
        sumGreen1 = -*(pixel1 + 1) - *(pixel2 + 1) - *(pixel3 + 1);
        sumGreen2 = -*((pixel1 + 4)) - *((pixel2 + 4)) - *((pixel3 + 4));
        sumBlue1 = -*(pixel1 + 2) - *(pixel2 + 2) - *(pixel3 + 2);
        sumBlue2 = -*((pixel1 + 5)) - *((pixel2 + 5)) - *((pixel3 + 5));

        register int j;
        // sharp every pixel in each column
        for (j = end; j > 1; j -= 3) {

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed3 = -*(pixel1 + 6) - *(pixel2 + 6) - *(pixel3 + 6);
            sumGreen3 = -*((pixel1 + 7)) - *((pixel2 + 7)) - *((pixel3 + 7));
            sumBlue3 = -*((pixel1 + 8)) - *((pixel2 + 8)) - *((pixel3 + 8));

            // update the colors of the first pixel
            sum = ((sumRed1 + sumRed2 + sumRed3) + 10 * *(pixel2 + 3));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 + 4));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 + 5));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            // go to the next 3 pixels
            pixel1 += 9;
            pixel2 += 9;
            pixel3 += 9;

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed1 = -*(pixel1) - *(pixel2) - *(pixel3);
            sumGreen1 = -*(pixel1 + 1) - *((pixel2 + 1)) - *(pixel3 + 1);
            sumBlue1 = -*(pixel1 + 2) - *((pixel2 + 2)) - *(pixel3 + 2);

            // update the colors of the second pixel
            sum =  ((sumRed1 + sumRed2 + sumRed3) + 10 * *(pixel2 - 3));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 - 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 - 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed2 = -*(pixel1 + 3) - *(pixel2 + 3) - *(pixel3 + 3);
            sumGreen2 = -*(pixel1 + 4) - *(pixel2 + 4) - *(pixel3 + 4);
            sumBlue2 = -*(pixel1 + 5) - *(pixel2 + 5) - *(pixel3 + 5);

            // update the colors of the third pixel
            sum =  ((sumRed1 + sumRed2 + sumRed3) + 10 * *pixel2);
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 + 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 + 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
        }

        // if the remainder of m by dividing in 3 is 1 or 2
        if(j >= 0){
            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed3 = -*(pixel1 + 6) - *(pixel2 + 6) - *(pixel3 + 6);
            sumGreen3 = -*((pixel1 + 7)) - *((pixel2 + 7)) - *((pixel3 + 7));
            sumBlue3 = -*((pixel1 + 8)) - *((pixel2 + 8)) - *((pixel3 + 8));

            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;

            // update the colors of the pixel
            sum =  ((sumRed1 + sumRed2 + sumRed3) + 10 * *(pixel2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 + 1));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            sum = ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 + 2));
            *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));

            // if the remainder of m by dividing in 3 is 2
            if(j == 1){

                pixel1 += 3;
                pixel2 += 3;
                pixel3 += 3;

                // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
                sumRed1 = -*(pixel1 + 3) - *(pixel2 + 3) - *(pixel3 + 3);
                sumGreen1 = -*(pixel1 + 4) - *((pixel2 + 4)) - *(pixel3 + 4);
                sumBlue1 = -*(pixel1 + 5) - *((pixel2 + 5)) - *(pixel3 + 5);

                // update the colors of the pixel
                sum =  ((sumRed1 + sumRed2 + sumRed3) + 10 * *pixel2);
                *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
                sum =  ((sumGreen1 + sumGreen2 + sumGreen3) + 10 * *(pixel2 + 1));
                *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
                sum = ((sumBlue1 + sumBlue2 + sumBlue3) + 10 * *(pixel2 + 2));
                *++c = (((sum = (sum > 0 ? sum : 0)) < 255 ? sum : 255));
            }
        }

        pixel1 += 6;
        pixel2 += 6;
        pixel3 += 6;
        c += 6;
    }
}

// Apply the blur kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothBlur(register unsigned char *pixel2, register unsigned char *c) {

    register int i;
    for (i = DIM3; i > 0; --i)
        *++c = *(pixel2++);

    // save the pointers to 3 pixels in registers
    register unsigned char *pixel1 = pixel2 - DIM3 + 3;
    register unsigned char *pixel3 = pixel2 + DIM3 + 3;

    // save sums of 3 pixels in column for every color for 3 columns
    register int sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3,
            j, end = m - 3;

    // blur every pixel in each row
    for (i = end + 1; i > 0; --i) {

        // the first pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = *(pixel1 - 3) + *(pixel2 - 3) + *(pixel3 - 3);
        sumRed2 = *pixel1 + *pixel2 + *pixel3;
        sumGreen1 = *(pixel1 - 2) + *((pixel2 - 2)) + *((pixel3 - 2));
        sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
        sumBlue1 = *(pixel1 - 1) + *((pixel2 - 1)) + *((pixel3 - 1));
        sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);

        // blur every pixel in each column
        for (j = end; j > 1; j -= 3) {

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen3 = *((pixel1 + 4)) + *((pixel2 + 4)) + *((pixel3 + 4));
            sumBlue3 = *((pixel1 + 5)) + *((pixel2 + 5)) + *((pixel3 + 5));

            // update the colors of the first pixel
            *++c = ((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = ((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = ((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed1 = *(pixel1 + 6) + *(pixel2 + 6) + *(pixel3 + 6);
            sumGreen1 = *(pixel1 + 7) + *((pixel2 + 7)) + *(pixel3 + 7);
            sumBlue1 = *(pixel1 + 8) + *((pixel2 + 8)) + *(pixel3 + 8);

            // update the colors of the second pixel
            *++c = ((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = ((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = ((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            // go to the next 3 pixels
            pixel1 += 9;
            pixel2 += 9;
            pixel3 += 9;

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel
            sumRed2 = *pixel1 + *pixel2 + *pixel3;
            sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
            sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);

            // update the colors of the third pixel
            *++c = ((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = ((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = ((sumBlue1 + sumBlue2 + sumBlue3) / 9);
        }

        // if the remainder of m by dividing in 3 is 1 or 2
        if(j >= 0){
            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;

            // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
            sumRed3 = *(pixel1) + *(pixel2) + *(pixel3);
            sumGreen3 = *((pixel1 + 1)) + *((pixel2 + 1)) + *((pixel3 + 1));
            sumBlue3 = *((pixel1 + 2)) + *((pixel2 + 2)) + *((pixel3 + 2));

            // update the colors of the pixel
            *++c = ((sumRed1 + sumRed2 + sumRed3) / 9);
            *++c = ((sumGreen1 + sumGreen2 + sumGreen3) / 9);
            *++c = ((sumBlue1 + sumBlue2 + sumBlue3) / 9);

            // if the remainder of m by dividing in 3 is 2
            if(j >= 1){
                pixel1 += 3;
                pixel2 += 3;
                pixel3 += 3;

                // update the sums to the next pixel (we have two sums columns colors from the previous pixel)
                sumRed1 = *(pixel1) + *(pixel2) + *(pixel3);
                sumGreen1 = *((pixel1 + 1)) + *((pixel2 + 1)) + *((pixel3 + 1));
                sumBlue1 = *((pixel1 + 2)) + *((pixel2 + 2)) + *((pixel3 + 2));

                // update the colors of the pixel
                *++c = ((sumRed1 + sumRed2 + sumRed3) / 9);
                *++c = ((sumGreen1 + sumGreen2 + sumGreen3) / 9);
                *++c = ((sumBlue1 + sumBlue2 + sumBlue3) / 9);
            }
        }

        // the last pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

        pixel1 += 6;
        pixel3 += 6;
    }

    // copy last row
    for (i = DIM3; i > 0; --i)
        *++c = *(pixel2++);
}



// Apply the blur filter kernel over each pixel, ignore pixels where the kernel exceeds bounds
void smoothBlurFilter(register unsigned char *pixel2, register unsigned char *c, int *sums) {

    register int *sum1 = sums;
    register int i;
    for (i = DIM3; i > 0; --i)
        *++c = *(pixel2++);

    register int min_intensity, max_intensity, red, green, blue, end = m - 3;

    // save the pointers to 3 pixels in registers
    register unsigned char *pixel1 = pixel2 - DIM3 + 3;
    register unsigned char *pixel3 = pixel2 + DIM3 + 3;
    register unsigned char *minp, *maxp;


    // save sums of 3 pixels in column for every color for 3 colomns
    register int j, sumRed1, sumRed2, sumRed3, sumGreen1, sumGreen2, sumGreen3, sumBlue1, sumBlue2, sumBlue3, sum;
    register int *sum2 = sum1 + m;
    register int *sum3 = sum2 + m;

    // blur every pixel in each column
    for (i = end + 1; i > 0; --i) {

        // the first pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

        // save sums of 3 pixels in column for every color for 3 columns
        sumRed1 = *(pixel1 - 3) + *(pixel2 - 3) + *(pixel3 - 3);
        sumRed2 = *pixel1 + *pixel2 + *pixel3;
        sumGreen1 = *(pixel1 - 2) + *((pixel2 - 2)) + *((pixel3 - 2));
        sumGreen2 = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1);
        sumBlue1 = *(pixel1 - 1) + *((pixel2 - 1)) + *((pixel3 - 1));
        sumBlue2 = *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2);

        // blur every pixel in each column
        for (j = 1; j < end; j += 3) {

            sumRed3 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen3 = *(pixel1 + 4) + *(pixel2 + 4) + *(pixel3 + 4);
            sumBlue3 = *(pixel1 + 5) + *(pixel2 + 5) + *(pixel3 + 5);

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // save the minimum and maximum pixel as the first
            sum = *sum1;
            min_intensity = *(sum1 - 1);
            if (min_intensity < sum) {
                maxp = pixel1;
                max_intensity = sum;
                minp = pixel1 - 3;
            } else {
                maxp = pixel1 - 3;
                max_intensity = min_intensity;
                minp = pixel1;
                min_intensity = sum;
            }

            sum = *(sum1 + 1);
            if (max_intensity < sum) {
                maxp = pixel1 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel1 + 3;
                min_intensity = sum;
            }

            sum = *(sum2 - 1);
            if (max_intensity < sum) {
                maxp = pixel2 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 - 3;
                min_intensity = sum;
            }

            sum = *(sum2);
            if (max_intensity < sum) {
                maxp = pixel2;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2;
                min_intensity = sum;
            }

            sum = *(sum2 + 1);
            if (max_intensity < sum) {
                maxp = pixel2 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 + 3;
                min_intensity = sum;
            }

            sum = *(sum3 - 1);
            if (max_intensity < sum) {
                maxp = pixel3 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3 - 3;
                min_intensity = sum;
            }

            sum = *(sum3);
            if (max_intensity < sum) {
                maxp = pixel3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3;
                min_intensity = sum;
            }

            sum = *(sum3 + 1);
            if (max_intensity < sum)
                maxp = pixel3 + 3;
            else if (min_intensity >= sum)
                minp = pixel3 + 3;

            // divide by kernel's weight
            red = (red - *minp - *maxp) / 7;
            green = (green - *(minp + 1) - *(maxp + 1)) / 7;
            blue = (blue - *(minp + 2) - *(maxp + 2)) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
            ++sum1;
            ++sum2;
            ++sum3;

            sumRed1 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen1 = *(pixel1 + 4) + *(pixel2 + 4) + *(pixel3 + 4);
            sumBlue1 = *(pixel1 + 5) + *(pixel2 + 5) + *(pixel3 + 5);

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // then check every pixel and update the min and max in accordance
            sum = *sum1;
            min_intensity = *(sum1 - 1);
            if (min_intensity < sum) {
                maxp = pixel1;
                max_intensity = sum;
                minp = pixel1 - 3;
            } else {
                maxp = pixel1 - 3;
                max_intensity = min_intensity;
                minp = pixel1;
                min_intensity = sum;
            }

            sum = *(sum1 + 1);
            if (max_intensity < sum) {
                maxp = pixel1 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel1 + 3;
                min_intensity = sum;
            }

            sum = *(sum2 - 1);
            if (max_intensity < sum) {
                maxp = pixel2 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 - 3;
                min_intensity = sum;
            }

            sum = *(sum2);
            if (max_intensity < sum) {
                maxp = pixel2;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2;
                min_intensity = sum;
            }

            sum = *(sum2 + 1);
            if (max_intensity < sum) {
                maxp = pixel2 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 + 3;
                min_intensity = sum;
            }

            sum = *(sum3 - 1);
            if (max_intensity < sum) {
                maxp = pixel3 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3 - 3;
                min_intensity = sum;
            }

            sum = *(sum3);
            if (max_intensity < sum) {
                maxp = pixel3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3;
                min_intensity = sum;
            }

            sum = *(sum3 + 1);
            if (max_intensity < sum)
                maxp = pixel3 + 3;
            else if (min_intensity >= sum)
                minp = pixel3 + 3;


            // divide by kernel's weight
            red = (red - *minp - *maxp) / 7;
            green = (green - *(minp + 1) - *(maxp + 1)) / 7;
            blue = (blue - *(minp + 2) - *(maxp + 2)) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
            ++sum1;
            ++sum2;
            ++sum3;

            sumRed2 = *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3);
            sumGreen2 = *(pixel1 + 4) + *(pixel2 + 4) + *(pixel3 + 4);
            sumBlue2 = *(pixel1 + 5) + *(pixel2 + 5) + *(pixel3 + 5);

            // save sums of each color of the pixel
            red = sumRed1 + sumRed2 + sumRed3;
            green = sumGreen1 + sumGreen2 + sumGreen3;
            blue = sumBlue1 + sumBlue2 + sumBlue3;

            // then check every pixel and update the min and max in accordance
            sum = *sum1;
            min_intensity = *(sum1 - 1);
            if (min_intensity < sum) {
                maxp = pixel1;
                max_intensity = sum;
                minp = pixel1 - 3;
            } else {
                maxp = pixel1 - 3;
                max_intensity = min_intensity;
                minp = pixel1;
                min_intensity = sum;
            }

            sum = *(sum1 + 1);
            if (max_intensity < sum) {
                maxp = pixel1 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel1 + 3;
                min_intensity = sum;
            }

            sum = *(sum2 - 1);
            if (max_intensity < sum) {
                maxp = pixel2 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 - 3;
                min_intensity = sum;
            }

            sum = *(sum2);
            if (max_intensity < sum) {
                maxp = pixel2;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2;
                min_intensity = sum;
            }

            sum = *(sum2 + 1);
            if (max_intensity < sum) {
                maxp = pixel2 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 + 3;
                min_intensity = sum;
            }

            sum = *(sum3 - 1);
            if (max_intensity < sum) {
                maxp = pixel3 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3 - 3;
                min_intensity = sum;
            }

            sum = *(sum3);
            if (max_intensity < sum) {
                maxp = pixel3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3;
                min_intensity = sum;
            }

            sum = *(sum3 + 1);
            if (max_intensity < sum)
                maxp = pixel3 + 3;
            else if (min_intensity >= sum)
                minp = pixel3 + 3;


            // divide by kernel's weight
            red = (red - *minp - *maxp) / 7;
            green = (green - *(minp + 1) - *(maxp + 1)) / 7;
            blue = (blue - *(minp + 2) - *(maxp + 2)) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
            ++sum1;
            ++sum2;
            ++sum3;

        }

        int k;
        // the last pixels in the row if the number doesnt divide by 3
        for (k = j; k < end + 2; ++k) {
            // save sums of each color of the pixel
            red = *pixel1 + *pixel2 + *pixel3 + *(pixel1 + 3) + *(pixel2 + 3) + *(pixel3 + 3) + *(pixel1 - 3) +
                  *(pixel2 - 3) + *(pixel3 - 3);
            green = *(pixel1 + 1) + *(pixel2 + 1) + *(pixel3 + 1) + *(pixel1 + 4) + *(pixel2 + 4) + *(pixel3 + 4) +
                    *(pixel1 - 2) + *(pixel2 - 2) + *(pixel3 - 2);
            blue = *(pixel1 - 1) + *(pixel2 - 1) + *(pixel3 - 1) + *(pixel1 + 2) + *(pixel2 + 2) + *(pixel3 + 2) +
                   *(pixel1 + 5) + *(pixel2 + 5) + *(pixel3 + 5);

            // then check every pixel and update the min and max in accordance
            sum = *sum1;
            min_intensity = *(sum1 - 1);
            if (min_intensity < sum) {
                maxp = pixel1;
                max_intensity = sum;
                minp = pixel1 - 3;
            } else {
                maxp = pixel1 - 3;
                max_intensity = min_intensity;
                minp = pixel1;
                min_intensity = sum;
            }

            sum = *(sum1 + 1);
            if (max_intensity < sum) {
                maxp = pixel1 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel1 + 3;
                min_intensity = sum;
            }

            sum = *(sum2 - 1);
            if (max_intensity < sum) {
                maxp = pixel2 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 - 3;
                min_intensity = sum;
            }

            sum = *(sum2);
            if (max_intensity < sum) {
                maxp = pixel2;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2;
                min_intensity = sum;
            }

            sum = *(sum2 + 1);
            if (max_intensity < sum) {
                maxp = pixel2 + 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel2 + 3;
                min_intensity = sum;
            }

            sum = *(sum3 - 1);
            if (max_intensity < sum) {
                maxp = pixel3 - 3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3 - 3;
                min_intensity = sum;
            }

            sum = *(sum3);
            if (max_intensity < sum) {
                maxp = pixel3;
                max_intensity = sum;
            } else if (min_intensity >= sum) {
                minp = pixel3;
                min_intensity = sum;
            }

            sum = *(sum3 + 1);
            if (max_intensity < sum)
                maxp = pixel3 + 3;
            else if (min_intensity >= sum)
                minp = pixel3 + 3;

            // divide by kernel's weight
            red = (red - *minp - *maxp) / 7;
            green = (green - *(minp + 1) - *(maxp + 1)) / 7;
            blue = (blue - *(minp + 2) - *(maxp + 2)) / 7;

            // truncate each pixel's color values to match the range [0,255]
            *++c = (((red = (red < 255 ? red : 255))));
            *++c = (((green = (green < 255 ? green : 255))));
            *++c = (((blue = (blue < 255 ? blue : 255))));

            // go to the next pixel
            pixel1 += 3;
            pixel2 += 3;
            pixel3 += 3;
            ++sum1;
            ++sum2;
            ++sum3;
        }

        // the last pixel in the row
        *++c = *(pixel2++);
        *++c = *(pixel2++);
        *++c = *(pixel2++);

        // go to the next row to the second pixel
        pixel1 += 6;
        pixel3 += 6;
        sum1 += 2;
        sum2 += 2;
        sum3 += 2;
    }

    // copy last row
    for (i = DIM3; i > 0; --i)
        *++c = *(pixel2++);
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
        smoothSharp(newData, oldData + 2 + DIM3);
        image->data = oldData;
        writeBMP(image, srcImgpName, sharpRsltImgName);

        free(newData);
    }

        // filter
    else {
        register int *sums = malloc (NM4);
        register int *backupSums = sums + 1;
        --sums;
        register int i;
        register unsigned char *helpPixels = image->data;
        for (i = NM - 7; i > 0; i-=8) {
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
        }

        register int j;
        for (j = i + 7; j > 0; --j) {
            *++sums = *helpPixels + *(helpPixels + 1) + *(helpPixels + 2);
            helpPixels += 3;
        }
        register char *newData = malloc(NM3);
        register char *oldData = image->data;

        // call to the smooth blur filter function from the second pixel in the first row of the image
        smoothBlurFilter(oldData, newData - 1, backupSums);

        oldData = image->data;
        image->data = newData;

        // copy the data to back-up array of chars and write the filtered blur image
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // call to the smooth sharp function from the second pixel in the first row of the image and write
        // the filtered sharped image and free the memory we allocated
        smoothSharp(newData, oldData + 2 + DIM3);
        image->data = oldData;
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);

        free(backupSums - 1);
        free(newData);
    }
}
