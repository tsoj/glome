#version

#include_glsl "src/shared_glm_glsl.h"

layout(location = 0) in vec3 vert_position_model_space;
layout(location = 1) in vec3 vert_normal_model_space;
layout(location = 2) in vec3 vert_tangent_model_space;
layout(location = 3) in vec2 texture_coordinate;

out VsOut
{
    vec2 texture_coordinate;
    vec4 coord;
    mat4 tangent_to_world_space;
} vs_out;

uniform mat4 model_hypersphere_orientation;
uniform mat3 model_orientation;
uniform mat4 camera_hypersphere_orientation;
uniform mat3 camera_orientation;
uniform float field_of_view;
uniform float aspect_ratio;
uniform float far_plane;
uniform float radius;

void main()
{
    enablePrintf();

    vec4 vert_coord = getCoord(model_hypersphere_orientation, model_orientation, vert_position_model_space, radius);

    vs_out.coord = vert_coord;

    vec3 view_angles = getViewAngles(camera_orientation, camera_hypersphere_orientation, vert_coord, radius);
    gl_Position = vec4(getViewSpaceCoords(
        view_angles,
        field_of_view, aspect_ratio, far_plane
    ), 1.0);

    // Don't draw triangles with vertices at almost 180deg angle to the camera view direction
    // because they might be on different camera sides because of the curved space
    float vertex_model_angle_length = angleDistance(radius, length(vert_position_model_space));
    if(abs(radians(85.0f)-min(abs(view_angles.x),radians(85.0f))) < vertex_model_angle_length)
    {
        gl_Position.x = gl_Position.x/0.0;
    }
    if(abs(radians(85.0f)-min(abs(view_angles.y),radians(85.0f))) < vertex_model_angle_length)
    {
        gl_Position.x = gl_Position.x/0.0;
    }

    vec4 vert_normal_world_space =
        model_hypersphere_orientation*vec4(model_orientation*vert_normal_model_space, 0);
    vec4 vert_tangent_world_space =
        model_hypersphere_orientation*vec4(model_orientation*vert_tangent_model_space, 0.0);
    vec4 vert_bi_tangent_world_space =
        model_hypersphere_orientation*vec4(model_orientation*cross(vert_tangent_model_space, vert_normal_model_space), 0.0);

    vs_out.tangent_to_world_space = mat4(
        normalize(vert_tangent_world_space),
        normalize(vert_normal_world_space),
        normalize(vert_bi_tangent_world_space),
        normalize(model_hypersphere_orientation[3])
    );

    vs_out.texture_coordinate = texture_coordinate;
}
