#version 430 core 
/*! VERTEX SHADER 
\author Yhaliff Said Barraza
 */

/** STRUCTS */

struct Vertex{
  vec4 position; 
  vec2 texcoords;
};

/** LAYOUT */
layout(location = 0) in vec4 la_position; 
layout(location = 1) in vec2 la_texcoords;
layout(location = 2) in vec3 la_normal;

/** UNIFORM  VARIABLES */

uniform mat4 u_projection;
uniform mat4 u_view;

uniform mat4x4 u_world;

uniform vec4 uColor;

uniform vec3 u_lambertLightDir;

/** OUT VARIABLES */ 
out vec2 outTexcoords;
out vec3 outNormal;
out vec4 outColor;

/** MAIN */
void main()
{
  //mat4 mv = u_view * u_projection;
  //mat4 mvp = mv * u_world;
    gl_Position = la_position * (u_world * u_view * u_projection); 
    outNormal = normalize(la_normal) * u_world; 
    outTexcoords = la_texcoords;
    outColor = uColor; 
} 
