#include "my_library.h"
#include "user_interface.h"
#include "sensor.h"

void debug_printf_definition(char *string) { Serial.print(string); }

void setup(void)
{
    Serial.begin(9600);

    analogReference(EXTERNAL);

    ui_init();
    sensor_init();
}

void loop(void)
{
    ui_update();
    sensor_update();
}
