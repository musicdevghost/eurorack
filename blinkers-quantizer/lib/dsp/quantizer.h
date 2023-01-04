#include <stdlib.h>

typedef enum {
    SEMI_TONES
} scale_t;

uint8_t quantize_to_scale_8b(uint32_t value, scale_t scale_ref);