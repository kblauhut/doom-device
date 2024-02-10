
#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320

#ifdef __cplusplus
extern "C" {
#endif
void draw_render_buffer_to_display(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
void uninstall_display(void);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
void init_display(void);
#ifdef __cplusplus
}
#endif

extern uint16_t RENDER_BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT];

#endif 