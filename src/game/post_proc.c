#include "post_proc.h"

#include "../gfx/renderer.h"

static PostProcState _postProcState;

void post_proc_set_shake( bool enabled )
{
    _postProcState.shake = enabled;
}

void post_proc_set_chaos( bool enabled )
{
    // Only one can be active at a time
    if ( !_postProcState.confuse )
    {
        _postProcState.chaos = enabled;
    }
}

void post_proc_set_confuse( bool enabled )
{
    // Only one can be active at a time
    if ( !_postProcState.chaos )
    {
        _postProcState.confuse = enabled;
    }
}

void post_proc_reset_all  ()
{
    _postProcState.shake = false;
    _postProcState.chaos = false;
    _postProcState.confuse = false;
}

void post_proc_send_uniforms()
{
    renderer_set_post_proc_uniforms(
        _postProcState.shake,
        _postProcState.chaos,
        _postProcState.confuse
    );
}