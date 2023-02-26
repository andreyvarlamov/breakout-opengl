#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D image;
uniform vec4 particle_color;

void main()
{
    color = particle_color * texture ( image, tec_coords );
}