//Mackenzie Broughton
//CPSC 3710 - Spring 2022
//Assignment 1

#version 150

in vec4 vPosition;

uniform mat4 uModelView;
uniform mat4 uProjection;
uniform vec4 uColour;

out vec4 vColour;

void main()
{
  gl_Position = uProjection * uModelView * vPosition;
  
  vColour = uColour;
}
