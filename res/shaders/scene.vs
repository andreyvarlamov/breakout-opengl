#version 330 core
layout ( location = 0 ) in vec4 vertex;

out vec2 tex_coords;

uniform bool  shake;
uniform bool  chaos;
uniform bool  confuse;
uniform float time;

void main()
{
    gl_Position = vec4( vertex.xy, 0.0f, 1.0f );
    vec2 tex = vertex.zw;

    if ( chaos )
    {
        float strength = 0.3f;
        vec2 pos = vec2(
            tex.x + sin( time ) * strength,
            tex.y + cos( time ) * strength
        );
        tex_coords = pos;
    }
    else if ( confuse )
    {
        tex_coords = vec2( 1.0 - tex.x, 1.0 - tex.y );
    }
    else
    {
        tex_coords = tex;
    }

    if ( shake )
    {
        float strength = 0.01f;
        gl_Position.x += cos( time + 10 ) * strength;
        gl_Position.y += cos( time + 15 ) * strength;
    }
}