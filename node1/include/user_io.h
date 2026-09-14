// This module is responsible for translating User IO input into usable data.

typedef struct {
  float x;
  float y;
} vec2_t;

void joy_calibrate(void);

vec2_t joy_read(void);
