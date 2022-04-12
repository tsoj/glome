#version

out vec4 out_color;

in VsOut
{
    vec2 texture_coordinate;
} vs_out;

uniform sampler2D image;

void main()
{
    out_color = texture(image, vs_out.texture_coordinate);
}
