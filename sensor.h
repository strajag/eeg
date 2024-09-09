#ifndef SENSOR_H
#define SENSOR_H

#include "my_library.h"

#define PIN_INPUT (14)

#define SENSOR_SAMPLE_ARRAY_DEFAULT_SIZE (256)

#define SENSOR_SAMPLE_ARRAY_FILL_DELAY_MAXIMUM (1000000) /* us */
#define SENSOR_SAMPLE_ARRAY_FILL_DELAY_MINIMUM (200) /* us */

#define SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_US_PER_SAMPLE (0)
#define SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_US_ALL_SAMPLE (1)
#define SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_HZ_PER_SAMPLE (2)
#define SENSOR_SAMPLE_ARRAY_FILL_DELAY_TIME_TYPE_HZ_ALL_SAMPLE (3)

extern int16_t sensor_sample_array[SENSOR_SAMPLE_ARRAY_DEFAULT_SIZE];
extern uint16_t sensor_sample_array_size;
extern uint32_t sensor_sample_array_fill_duration; /* us */
extern uint8_t sensor_sample_array_fill_latency ; /* us */
extern uint32_t sensor_sample_array_fill_delay_in_milli_seconds; /* ms */
extern uint32_t sensor_sample_array_fill_delay_in_micro_seconds; /* us */
extern f32_t sensor_sample_array_fill_rate;

extern void sensor_init(void);
extern void sensor_update(void);

extern void sensor_sample_array_fill_delay_time_set(float time, uint8_t type, bool_t include_latency);
extern void sensor_sample_array_fill_with_analog_read(void);
extern void sensor_sample_array_fill_rate_set(bool_t include_latency);

#endif /* SENSOR_H */
