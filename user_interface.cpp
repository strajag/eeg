#include "user_interface.h"
#include "sensor.h"
#include "fft.h"

uint16_t ui_print_delay_in_milli_seconds = 0;
uint8_t ui_print_type = UI_PRINT_TYPE_FFT_PLOTTER;

static f32_t temp_time;
static uint8_t temp_type;
static bool_t temp_include_latency;

static f32_t a, b, c;
static f32_t fft_frequency;
static int16_t fft_amplitude_max = 0;
static f32_t fft_frequency_with_max_amplitude = 0;

static uint16_t counter;

void ui_init(void)
{

}

void ui_update(void)
{
    /*ui_serial_debug();*/
    ui_serial_normal();
    ui_print();
}

void ui_serial_normal(void)
{
    if(!Serial.available())
        return;

    delay(100);

    switch(Serial.read())
    {
        case 'p':
        {
            ui_print_type = UI_PRINT_TYPE_FFT_PLOTTER;
            ui_print_delay_in_milli_seconds = 0;
            break;
        }
        case 't':
        {
            ui_print_type = UI_PRINT_TYPE_FFT_TEXT;
            ui_print_delay_in_milli_seconds = 1000;
            break;
        }
    }

    while(Serial.available()) { Serial.read(); }
}

void ui_serial_debug(void)
{
    if(!Serial.available())
        return;

    delay(100);

    switch(Serial.read())
    {
        case 'p':
        {
            switch(Serial.read())
            {
                case 'n':
                {
                    ui_print_type = UI_PRINT_TYPE_NOTHING;
                    break;
                }
                case 'r':
                {
                    ui_print_type = UI_PRINT_TYPE_RAW;
                    break;
                }
                case 'f':
                {
                    ui_print_type = UI_PRINT_TYPE_SAMPLE_FILL;
                    sensor_sample_array_fill_delay_time_set(0, 0, 0);
                    break;
                }
                case 's':
                {
                    ui_print_type = UI_PRINT_TYPE_FFT_SERIAL;
                    break;
                }
                case 'p':
                {
                    ui_print_type = UI_PRINT_TYPE_FFT_PLOTTER;
                    ui_print_delay_in_milli_seconds = 0;
                    break;
                }
                case 't':
                {
                    ui_print_type = UI_PRINT_TYPE_FFT_TEXT;
                    ui_print_delay_in_milli_seconds = 1000;
                    break;
                }
                case 'd':
                {
                    ui_print_delay_in_milli_seconds = Serial.parseInt();
                    break;
                }
            }
            break;
        }
        case 's':
        {
            switch(Serial.read())
            {
                case 'd':
                {
                    temp_time = Serial.parseFloat();
                    temp_type = Serial.parseInt();
                    temp_include_latency = Serial.parseInt();
                    sensor_sample_array_fill_delay_time_set(temp_time, temp_type, temp_include_latency);
                    break;
                }
                case 'l':
                {
                    sensor_sample_array_fill_latency = Serial.parseInt();
                    break;
                }
                case 'p':
                {
                    Serial.print(F("duration: ")); Serial.println(sensor_sample_array_fill_duration);
                    Serial.print(F("rate: ")); Serial.println(sensor_sample_array_fill_rate);
                    Serial.print(F("size: ")); Serial.println(sensor_sample_array_size);
                    Serial.print(F("latency: ")); Serial.println(sensor_sample_array_fill_latency);
                    Serial.print(F("delay_ms: ")); Serial.println(sensor_sample_array_fill_delay_in_milli_seconds);
                    Serial.print(F("delay_us: ")); Serial.println(sensor_sample_array_fill_delay_in_micro_seconds);
                    break;
                }
            }
            break;
        }
    }

    while(Serial.available()) { Serial.read(); }
}

void ui_print(void)
{
    if(ui_print_type == UI_PRINT_TYPE_NOTHING)
        return;

    if(ui_print_type == UI_PRINT_TYPE_RAW)
        ui_print_raw();
    else if(ui_print_type == UI_PRINT_TYPE_SAMPLE_FILL)
        ui_print_sample_fill();
    else
    {
        sensor_sample_array_fill_with_analog_read();
        sensor_sample_array_fill_rate_set(TRUE);

        if(ui_print_type == UI_PRINT_TYPE_FFT_SERIAL)
            ui_print_fft_serial();
        else if(ui_print_type == UI_PRINT_TYPE_FFT_PLOTTER)
            ui_print_fft_plotter();
        else
            ui_print_fft_text();
    }

    delay(ui_print_delay_in_milli_seconds);
}

void ui_print_raw(void)
{
    Serial.print(1023); Serial.print(F(",")); Serial.print(0); Serial.print(F(",")); Serial.println(analogRead(PIN_INPUT));
}

void ui_print_sample_fill(void)
{
    sensor_sample_array_fill_with_analog_read();
    sensor_sample_array_fill_rate_set(FALSE);

    Serial.print(F("duration: ")); Serial.println(sensor_sample_array_fill_duration);
    Serial.print(F("rate: ")); Serial.println(sensor_sample_array_fill_rate);
}

void ui_print_fft_serial(void)
{
    fft_calculate_frequency_with_highest_amplitude(sensor_sample_array, sensor_sample_array_size, sensor_sample_array_fill_rate, TRUE);
    Serial.print(F("duration: ")); Serial.println(sensor_sample_array_fill_duration);
    Serial.print(F("rate: ")); Serial.println(sensor_sample_array_fill_rate);

    for(counter = 0; counter < sensor_sample_array_size; counter++)
    {
        Serial.print(sensor_sample_array[counter]); Serial.print(F(","));
    }

    Serial.println();

    Serial.print(F("freq: ")); Serial.println(frequency_with_highest_amplitude);
}

void ui_print_fft_plotter(void)
{
    fft_amplitude_max = 0;
    fft_frequency_with_max_amplitude = 0;
    fft_calculate_frequency_with_highest_amplitude(sensor_sample_array, sensor_sample_array_size, sensor_sample_array_fill_rate, FALSE);

    for(counter = 4; counter < 11; counter++)
    {
        a = real[counter - 1];
        b = real[counter];
        c = real[counter + 1];
        fft_frequency = (a * (counter - 1) + b * counter + c * (counter + 1)) / (a + b + c) * sensor_sample_array_fill_rate / sensor_sample_array_size;
     
        if(real[counter] > fft_amplitude_max)
        {
            fft_amplitude_max = real[counter];
            fft_frequency_with_max_amplitude = fft_frequency;
        }

        Serial.print(fft_frequency);
        Serial.print(",");
    }

    Serial.println(fft_frequency_with_max_amplitude);
}

void ui_print_fft_text(void)
{
    fft_calculate_frequency_with_highest_amplitude(sensor_sample_array, sensor_sample_array_size, sensor_sample_array_fill_rate, FALSE);

    for(counter = 1; counter < 11; counter++)
    {
        a = real[counter - 1];
        b = real[counter];
        c = real[counter + 1];
        fft_frequency = (a * (counter - 1) + b * counter + c * (counter + 1)) / (a + b + c) * sensor_sample_array_fill_rate / sensor_sample_array_size;
     
        Serial.print(fft_frequency);
        Serial.print(" - ");
        Serial.print(real[counter]);
        Serial.println();
    }

    Serial.println("-------");
}
