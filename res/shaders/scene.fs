#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D scene;

uniform vec2  offsets[9];
uniform int   edge_kernel[9];
uniform float blur_kernel[9];

uniform bool shake;
uniform bool chaos;
uniform bool confuse;

void main()
{
    color = vec4( 0.0f );
    vec3 sample[9];

    // Sample from texture offsets if using convolution matrix
    // -------------------------------------------------------
    if ( chaos || shake )
    {
        for ( int i = 0; i < 9; i++ )
        {
            sample[i] = vec3( texture( scene, tex_coords.st + offsets[i] ) );
        }
    }

    // Process effects
    // ---------------
    if ( chaos )
    {
        for ( int i = 0; i < 9; i++ )
        {
            color += vec4( sample[i] * edge_kernel[i], 0.0f );
        }
        color.a = 1.0f;
    }
    else if ( confuse )
    {
        color = vec4( 1.0f - texture( scene, tex_coords ).rgb, 1.0 );
    }
    else if ( shake )
    {
        for ( int i = 0; i < 9; i++ )
        {
            color += vec4( sample[i] * blur_kernel[i], 0.0f );
        }
        color.a = 1.0f;
    }
    else
    {
        color = texture( scene, tex_coords );
    }
}