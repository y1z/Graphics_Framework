#version 430 core
/*! VERTEX SHADER
\author Yhaliff Said Barraza
 */

 /** Defines */


/** STRUCTS */

struct Vertex{
  vec4 position;
  vec2 texcoords;
};

struct LightData{
  vec3 m_lambertLightDir;
};

/** LAYOUT */
layout(location = 0) in vec4 la_position;
layout(location = 1) in vec2 la_texcoords;
layout(location = 2) in vec3 la_normal;
layout(location = 3) in vec3 la_tangent;
layout(location = 4) in vec3 la_color;

/** UNIFORM  VARIABLES */

uniform mat4 u_projection;
uniform mat4 u_view;

uniform mat4x4 u_world;

uniform vec4 u_meshColor;

uniform vec3 u_lambertLightDir;
uniform vec3 m_lightPosition;

/** OUT VARIABLES */
out vec2 outTexcoords;
out vec3 outNormal;
out vec4 outColor;
out vec3 outLambertLightDir;

/** MAIN */
void main()
{
    gl_Position = la_position * (u_world * u_view * u_projection);
    vec4 wsNormal= vec4(la_normal.xyz,1.0f) * u_world;
    outNormal = wsNormal.xyz;
    outTexcoords = la_texcoords;
    outColor = u_meshColor;
    outLambertLightDir = -normalize(u_lambertLightDir);
}
