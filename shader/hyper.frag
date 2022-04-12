#version

out vec4 out_color;

#include_glsl "src/shared_glm_glsl.h"

#insert MAX_NUM_LIGHTS

in VsOut
{
    vec2 texture_coordinate;
    vec4 coord;
    mat4 tangent_to_world_space;
} vs_out;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_map;

uniform vec4 fog_color;

uniform vec4 light_position[MAX_NUM_LIGHTS];
uniform vec3 light_color[MAX_NUM_LIGHTS];
uniform int num_lights;

void main()
{
    vec4 diffuse = texture(diffuse_texture, vs_out.texture_coordinate);
    vec4 normal =
    vs_out.tangent_to_world_space*vec4(normalize(texture(normal_map, vs_out.texture_coordinate).rbg * 2.0 - vec3(1.0, 1.0, 1.0)), 0.0);

    // TODO: Class 3: implement square law for light intensity
    vec3 diffuse_light = vec3(0.0);
    for (int i = 0; i<num_lights; ++i)
    {
        vec4 to_light = normalize(getLocalDirectionalVector(vs_out.coord, light_position[i]));
        diffuse_light = diffuse_light + clamp(dot(to_light, normal), 0, 1)*light_color[i];
        to_light = -to_light;
        diffuse_light = diffuse_light + clamp(dot(to_light, normal), 0, 1)*light_color[i];
    }

    out_color = vec4(diffuse.rgb, 1.0)*vec4(diffuse_light, 1.0);

    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fog_factor = 1.0 - z*z*fog_color.a;
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    out_color = mix(vec4(fog_color.rgb, 1.0), out_color, fog_factor );
}


