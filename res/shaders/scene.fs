#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D scene;

void main()
{
    color = texture( scene, tex_coords );
}