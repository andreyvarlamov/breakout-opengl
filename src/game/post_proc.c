#include "post_proc.h"

#include "../gfx/renderer.h"

static PostProcState _postProcState;

void post_proc_set_shake( bool enabled )
{
    _postProcState.shake = enabled;
}

void post_proc_set_chaos( bool enabled )
{
    _postProcState.chaos = enabled;
}

void post_proc_set_confuse( bool enabled )
{
    _postProcState.confuse = enabled;
}

// Update scene shader uniforms to enable post-proc effects
void post_proc_set_uniforms()
{
    renderer_set_post_proc_uniforms(
        _postProcState.shake,
        _postProcState.chaos,
        _postProcState.confuse
    );
}