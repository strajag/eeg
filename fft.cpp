#include "fft.h"
#include "fft_private.h"

static uint8_t power_of_two_of_number_of_samples;
static int32_t temp_variable_1;
static int32_t temp_variable_2;
static int32_t temp_variable_3;
int16_t real[FFT_SAMPLE_ARRAY_MAX_SIZE];
int16_t imaginary[FFT_SAMPLE_ARRAY_MAX_SIZE];
static uint8_t scale;
static int16_t temp_variable_4;
static int16_t temp_variable_5;
static int16_t temp_variable_6;
static uint8_t temp_variable_7;

static uint8_t qu;
static uint8_t temp1;
static uint8_t temp2;
static int16_t temp_variable_8;
static uint8_t accuracy = 5; /* min 1 max 7 */
static uint8_t test;

static f32_t a, b, c;
f32_t frequency_with_highest_amplitude;

static int16_t counter_1, counter_2, counter_3;

void fft_calculate_frequency_with_highest_amplitude(int16_t *sample_array, uint16_t sample_amount, f32_t sample_rate, bool_t is_calculate_frequency_with_highest_amplitude)
{
    power_of_two_of_number_of_samples = 0;
    temp_variable_1 = sample_amount;

    while(temp_variable_1 /= 2)
        power_of_two_of_number_of_samples++;

    temp_variable_1 = 0;
    temp_variable_2 = 0;
    temp_variable_3 = 0;

    for(counter_1 = 0; counter_1 < sample_amount; counter_1++)
    {
        real[counter_1] = 0;
        imaginary[counter_1] = 0;

        temp_variable_3 += sample_array[counter_1];

        if(sample_array[counter_1] > temp_variable_1)
            temp_variable_1 = sample_array[counter_1];

        if(sample_array[counter_1] < temp_variable_2)
            temp_variable_2 = sample_array[counter_1];
    }

    temp_variable_1 -= temp_variable_2;
    temp_variable_2 = temp_variable_1;
    temp_variable_3 >>= power_of_two_of_number_of_samples;

    scale = 0;

    if(temp_variable_1 > 1024)
    {
        while(temp_variable_2 > 1024)
        {
            temp_variable_2 >>= 1;
            scale++;
        }
    }

    if(temp_variable_1 < 1024)
    {
        while(temp_variable_2 < 1024)
        {
            temp_variable_2 <<= 1;
            scale++;
        }
    }

    if(temp_variable_1 > 1024)
    {
        for(counter_1 = 0; counter_1 < sample_amount; counter_1++)
        {
            sample_array[counter_1] -= temp_variable_3;
            sample_array[counter_1] >>= scale;
        }

        scale = 128 - scale;
    }

    if(temp_variable_1 < 1024)
    {
        scale--;

        for(counter_1 = 0; counter_1 < sample_amount; counter_1++)
        {
            sample_array[counter_1] -= temp_variable_3;
            sample_array[counter_1] <<= scale;
        }

        scale += 128;
    }

    temp_variable_1 = 0;

    for(counter_1 = 0; counter_1 < power_of_two_of_number_of_samples; counter_1++)
    {
        if(counter_1 == 0)
            temp_variable_3 = 1;
        else if (counter_1 == 1)
            temp_variable_3 = 2;
        else
            temp_variable_3 *= 2;

        temp_variable_2 = sample_amount / (temp_variable_3 * 2);

        for(counter_2 = 0; counter_2 < temp_variable_3; counter_2++)
        {
            temp_variable_1++;
            imaginary[temp_variable_1] = imaginary[counter_2] + temp_variable_2;
        }
    }

    for(counter_1 = 0; counter_1 < sample_amount; counter_1++)
    {
        real[counter_1] = sample_array[imaginary[counter_1]];
        imaginary[counter_1] = 0;
    }

    /* fft */
    temp_variable_7 = 0;
    for(counter_1 = 0; counter_1 < power_of_two_of_number_of_samples; counter_1++)
    {
        if(counter_1 == 0)
            temp_variable_1 = 1;
        else
        {
            temp_variable_1 = 2;

            for(temp_variable_2 = 0; temp_variable_2 < counter_1 - 1; temp_variable_2++)
                temp_variable_1 *= 2;
        }

        temp_variable_2 = sample_amount / (temp_variable_1 * 2);

        for(counter_2 = 0; counter_2 < temp_variable_1; counter_2++)
        {
            temp_variable_3 = 1024 / (temp_variable_1 * -2) * counter_2;

            while(temp_variable_3 < 0)
                temp_variable_3 += 1024;

            while(temp_variable_3 > 1024)
                temp_variable_3 -= 1024;

            temp_variable_4 = counter_2;

            for(counter_3 = 0; counter_3 < temp_variable_2; counter_3++)
            {
                if(temp_variable_3 == 0)
                {
                    temp_variable_5 = real[temp_variable_1 + temp_variable_4];
                    temp_variable_6 = imaginary[temp_variable_1 + temp_variable_4];
                }
                else if(temp_variable_3 == 256)
                {
                    temp_variable_5 = -imaginary[temp_variable_1 + temp_variable_4];
                    temp_variable_6 = real[temp_variable_1 + temp_variable_4];
                }
                else if(temp_variable_3 == 512)
                {
                    temp_variable_5 = -real[temp_variable_1 + temp_variable_4];
                    temp_variable_6 = -imaginary[temp_variable_1 + temp_variable_4];
                }
                else if(temp_variable_3 == 768)
                {
                    temp_variable_5 = imaginary[temp_variable_1 + temp_variable_4];
                    temp_variable_6 = -real[temp_variable_1 + temp_variable_4];
                }
                else if(temp_variable_3 == 1024)
                {
                    temp_variable_5 = real[temp_variable_1 + temp_variable_4];
                    temp_variable_6 = imaginary[temp_variable_1 + temp_variable_4];
                }
                else
                {
                    temp_variable_5 = fft_private_sine(real[temp_variable_1 + temp_variable_4], 256 - temp_variable_3) - fft_private_sine(imaginary[temp_variable_1 + temp_variable_4], temp_variable_3);
                    temp_variable_6 = fft_private_sine(real[temp_variable_1 + temp_variable_4], temp_variable_3) + fft_private_sine(imaginary[temp_variable_1 + temp_variable_4], 256 - temp_variable_3);
                }

                real[temp_variable_4 + temp_variable_1] = real[temp_variable_4] - temp_variable_5;
                real[temp_variable_4] += temp_variable_5;

                if(real[temp_variable_4] > 15000 || real[temp_variable_4] < -15000)
                    temp_variable_7 = 1;

                imaginary[temp_variable_4 + temp_variable_1] = imaginary[temp_variable_4] - temp_variable_6;
                imaginary[temp_variable_4] += temp_variable_6;

                if(imaginary[temp_variable_4] > 15000 || imaginary[temp_variable_4] < -15000)
                    temp_variable_7 = 1;

                temp_variable_4 += temp_variable_1 * 2;
            }
        }

        if(temp_variable_7 == 1)
        {
            for(counter_2 = 0; counter_2 < sample_amount; counter_2++)
            {
                real[counter_2] >>= 1;
                imaginary[counter_2] >>= 1;
            }

            temp_variable_7 = 0;
            scale--;
        }
    }

    if(scale > 128)
    {
        scale -= 128;

        for(counter_1 = 0; counter_1 < sample_amount; counter_1++)
        {
            real[counter_1] >>= scale;
            imaginary[counter_1] >>= scale;
        }

        scale = 0;
    }
    else
        scale = 128 - scale;

    temp_variable_1 = sample_rate / sample_amount;
    temp_variable_2 = 0;

    if(is_calculate_frequency_with_highest_amplitude)
    {
        for(counter_1 = 1; counter_1 < sample_amount / 2; counter_1++)
        {
            real[counter_1] = fft_private_rss(real[counter_1], imaginary[counter_1]);
            imaginary[counter_1] = imaginary[counter_1 - 1] + temp_variable_1;

            if(temp_variable_2 < real[counter_1])
            {
                temp_variable_3 = counter_1;
                temp_variable_2 = real[counter_1];
            }
        }

        a = real[temp_variable_3 - 1];
        b = real[temp_variable_3];
        c = real[temp_variable_3 + 1];
        frequency_with_highest_amplitude = (a * (temp_variable_3 - 1) + b * temp_variable_3 + c * (temp_variable_3 + 1)) / (a + b + c) * sample_rate / sample_amount;
    }
    else
    {
        for(counter_1 = 1; counter_1 < sample_amount / 2; counter_1++)
        {
            real[counter_1] = fft_private_rss(real[counter_1], imaginary[counter_1]);
            imaginary[counter_1] = imaginary[counter_1 - 1] + temp_variable_1;
        }
    }
}

int16_t fft_private_sine(int16_t am, int16_t th)
{
    while(th > 1024)
        th -= 1024;

    while(th < 0)
        th += 1024;

    qu = th >> 8;

    if(qu == 1)
        th = 512 - th;
    else if(qu == 2)
        th -= 512;
    else if(qu == 3)
        th = 1024 - th;

    temp1 = 0;
    temp2 = 128;
    am >>= 1;
    temp_variable_8 = am;

    while(accuracy--)
    {
        test = (temp1 + temp2) >> 1;
        temp_variable_8 >>= 1;

        if(th > test)
        {
            temp1 = test;
            am += temp_variable_8;
        }
        else if(th < test)
        {
            temp2 = test;
            am -= temp_variable_8;
        }
    }

    if(qu == 2)
        am = -am;
    else if(qu == 3)
        am = -am;

    return(am);
}

int16_t fft_private_rss(int16_t a, int16_t b)
{
    if(a == 0 && b == 0)
        return(0);

    if(a < 0)
        a = -a;

    if(b < 0)
        b = -b;

    if(a > b)
    {
        temp_variable_5 = a;
        temp_variable_4 = b;
    }
    else
    {
        temp_variable_5 = b;
        temp_variable_4 = a;
    }

    if(temp_variable_5 > temp_variable_4 * 3)
        return(temp_variable_5);
    else
    {
        temp_variable_6 = temp_variable_4 >> 3;

        if(temp_variable_6 == 0)
            temp_variable_6 = 1;

        temp_variable_7 = 0;
        temp_variable_8 = temp_variable_4;

        while(temp_variable_8 < temp_variable_5)
        {
            temp_variable_8 += temp_variable_6;
            temp_variable_7++;
        }

        temp_variable_7++;
        counter_2 = 0;
        counter_3 = 1;

        while(counter_3 < temp_variable_7)
        {
            counter_2++;
            if(counter_2 > 3)
                counter_3 += 7;
            else
                counter_3 += counter_2 + 1;
        }

        temp_variable_7 = 7 - counter_2;
        temp_variable_6 >>= 1;

        for(counter_2 = 0; counter_2 < temp_variable_7; counter_2++)
            temp_variable_5 += temp_variable_6;

        return(temp_variable_5);
    }
}
