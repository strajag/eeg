#include "sensor.h"

int16_t sensor_sample_array[SENSOR_SAMPLE_ARRAY_DEFAULT_SIZE];
uint16_t sensor_sample_array_size = SENSOR_SAMPLE_ARRAY_DEFAULT_SIZE;
uint32_t sensor_sample_array_fill_duration;
uint8_t sensor_sample_array_fill_latency = 122;
uint32_t sensor_sample_array_fill_delay_in_milli_seconds;
uint32_t sensor_sample_array_fill_delay_in_micro_seconds;
f32_t sensor_sample_array_fill_rate;

static uint16_t counter;

void sensor_init(void)
{
    pinMode(INPUT, PIN_INPUT);

    sensor_sample_array_fill_delay_time_set(10, SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_HZ_ALL_SAMPLE, 1);
}

void sensor_update(void)
{

}

void sensor_sample_array_fill_delay_time_set(float time, uint8_t type, bool_t include_latency)
{
    if(time == 0)
    {
        sensor_sample_array_fill_delay_in_milli_seconds = 0;
        sensor_sample_array_fill_delay_in_micro_seconds = 0;
        return;
    }

    if(type == SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_HZ_PER_SAMPLE)
        time = 1000000.0f / time;
    else if(type == SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_HZ_ALL_SAMPLE)
        time = 1000000.0f / time / (float)sensor_sample_array_size;
    else if(type == SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_US_ALL_SAMPLE)
        time /= (float)sensor_sample_array_size;

    if(time < SENSOR_SAMPLE_ARRAY_FILL_DELAY_MINIMUM || time > SENSOR_SAMPLE_ARRAY_FILL_DELAY_MAXIMUM)
    {
        Serial.println(F("time < SENSOR_SAMPLE_ARRAY_FILL_DELAY_MINIMUM || time > SENSOR_SAMPLE_ARRAY_FILL_DELAY_MAXIMUM"));
        return;
    }

    if(include_latency)
        time -= (float)sensor_sample_array_fill_latency;

    sensor_sample_array_fill_delay_in_milli_seconds = time / 1000.0f;
    sensor_sample_array_fill_delay_in_micro_seconds = time - (sensor_sample_array_fill_delay_in_milli_seconds * 1000.0f);
}

void sensor_sample_array_fill_with_analog_read(void)
{
    sensor_sample_array_fill_duration = micros();

    for(counter = 0; counter < sensor_sample_array_size; counter++)
    {
        sensor_sample_array[counter] = analogRead(PIN_INPUT);
        delay(sensor_sample_array_fill_delay_in_milli_seconds);
        delayMicroseconds(sensor_sample_array_fill_delay_in_micro_seconds);
    }

    sensor_sample_array_fill_duration = micros() - sensor_sample_array_fill_duration;
}

void sensor_sample_array_fill_rate_set(bool_t include_latency)
{
    if(include_latency)
        sensor_sample_array_fill_duration += sensor_sample_array_fill_latency;

    sensor_sample_array_fill_rate = 1000000.0f / sensor_sample_array_fill_duration * (float)sensor_sample_array_size;
}
