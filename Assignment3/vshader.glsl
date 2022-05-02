/*
 * Mackenzie Broughton
 * CPSC 3710
 * April 14, 2022
 */

#version 150

in vec4 vPosition;

// uniform variables
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;
uniform vec3 uNormalVec;
uniform vec4 uLightPos;

// position of the vertex in camera space
out vec3 vPosition2;

// normal vector in camera space
out vec3 vNormal2;

// light position in camera space
out vec3 vLightPos2;

void main()
{
  // compute position in camera space
  vec4 vPositionMV = uView * uModel *  vPosition;

  // compute object position
  gl_Position = uProjection * vPositionMV;

  // Compute normal vector in camera space
  vNormal2 = uNormalMatrix * uNormalVec;

  //compute light position in camera space
  vLightPos2 = (uView * uLightPos).xyz;

  // perspective division
  vPosition2 = vPositionMV.xyz / vPositionMV.w;
}
