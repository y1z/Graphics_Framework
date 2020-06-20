#version 430 core
/*! FRAGMENT/PIXEL SHADER
\author Yhaliff Said Barraza
 */
 /** STRUCT */
 struct LightData{
   vec3 m_lambertLightDir;
 };

 /** UNIFORMS */
uniform sampler2D uTextureSampler;

/** IN VARIABLES */
in vec2 outTexcoords;
in vec3 outNormal;
in vec4 outColor;
in vec3 outLambertLightDir;
/** OUT VARIABLES */
out vec4 resultColor;


//out vec2 resultTexcoords;
/** MAIN*/
void main()
{
  vec3 norm = -normalize(outNormal);
  vec3 lightDir = -normalize( outLambertLightDir );
  float light = max(dot( outLambertLightDir  , norm ),0.0f);
  resultColor = texture(uTextureSampler, outTexcoords) * light;//  vec4(norm.xyz,1.0f);// * light; //* vec4(outLightData.m_lambertLightDir.xyz,1.0f);
}
