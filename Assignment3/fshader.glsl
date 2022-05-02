/*
 * Mackenzie Broughton
 * CPSC 3710
 * April 14, 2022
 */

#version 150

in vec3 vPosition2;
in vec3 vNormal2;
in vec3 vLightPos2;

// uniform variables
uniform vec3 uLightAmbient;
uniform vec3 uLightDiffuse;
uniform vec3 uLightSpecular;
uniform vec3 uFaceColour;
uniform float uShininess;

out vec4  fColour;

void main()
{
    //Computing necessary vectors for reflection computations
    vec3 L = normalize(vLightPos2 - vPosition2);
    vec3 V = normalize(-1 * vPosition2);
    vec3 H = normalize(V + L);

    //Computing ambient reflection
    //Ia = ka * La
    vec3 ambient = uFaceColour * uLightAmbient;

    //Computing diffuse reflection
    //Id = kd(l (dot) n) * Ld
    //If l (dot) n is negative, use 0 (take max)
    vec3 diffuse = uFaceColour * (max(0, dot(L, vNormal2))) * uLightDiffuse;

    //Computing specular reflection
    //Is = ks * Ls(n (dot) H)^4a
    //If n (dot) H is negative, use 0 (take max)
    vec3 specular = uFaceColour * uLightSpecular * pow(max(0, dot(vNormal2, H)), (uShininess * 4));

    //combining the different reflections
    fColour.rgb = ambient + diffuse + specular;
    fColour.a = 1.0;
}
