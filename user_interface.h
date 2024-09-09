#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "my_library.h"

#define UI_PRINT_TYPE_NOTHING (0)
#define UI_PRINT_TYPE_RAW (1)
#define UI_PRINT_TYPE_SAMPLE_FILL (2)
#define UI_PRINT_TYPE_FFT_SERIAL (3)
#define UI_PRINT_TYPE_FFT_PLOTTER (4)
#define UI_PRINT_TYPE_FFT_TEXT (5)

extern uint16_t ui_print_delay_in_milli_seconds; /* ms */
extern uint8_t ui_print_type;

extern void ui_init(void);
extern void ui_update(void);

extern void ui_serial_normal(void);
extern void ui_serial_debug(void);

extern void ui_print(void);
extern void ui_print_raw(void);
extern void ui_print_sample_fill(void);
extern void ui_print_fft_serial(void);
extern void ui_print_fft_plotter(void);
extern void ui_print_fft_text(void);

#endif /* USER_INTERFACE_H */
