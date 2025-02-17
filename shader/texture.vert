#version

layout(location = 0) in vec2 vert_position;
layout(location = 1) in vec2 texture_coordinate;

out VsOut
{
    vec2 texture_coordinate;
} vs_out;


void main()
{
    gl_Position = vec4(vert_position, 0.0, 1.0);
    vs_out.texture_coordinate = texture_coordinate;
}
