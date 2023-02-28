#ifndef POST_PROC_H
#define POST_PROC_H

#include <stdbool.h>

typedef struct PostProcState
{
    bool shake;
    bool chaos;
    bool confuse;
} PostProcState;

void post_proc_set_shake  ( bool enabled );
void post_proc_set_chaos  ( bool enabled );
void post_proc_set_confuse( bool enabled );

// Update scene shader uniforms to enable post-proc effects
void post_proc_send_uniforms();

#endif