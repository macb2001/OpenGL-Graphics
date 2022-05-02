//Mackenzie Broughton
//CPSC 3710 - Spring 2022
//Assignment 1

#include <Angel.h>

GLint colourLoc, modelViewLoc, projLoc;
GLfloat theta = 0.0;
GLfloat theta_x = 0.0, theta_y = 0.0, theta_z = 0.0;
GLfloat scale = 1.0;
GLfloat pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;

const int NUMPOINTS = 4;
const int NUMFACES = 6;

GLuint vao[NUMFACES];
GLuint buffer[NUMFACES];

void init()
{
  vec3 points[NUMFACES][NUMPOINTS] = {
      //bottom face
      {vec3(-0.5, 0.5, -0.5),
       vec3(-0.5, -0.5, -0.5),
       vec3(0.5, -0.5, -0.5),
       vec3(0.5, 0.5, -0.5)},

      //top face
      {vec3(-0.25, 0.25, 0.5),
       vec3(-0.25, -0.25, 0.5),
       vec3(0.25, -0.25, 0.5),
       vec3(0.25, 0.25, 0.5)},

      //west face
      {vec3(-0.25, 0.25, 0.5),
       vec3(-0.5, 0.5, -0.5),
       vec3(-0.5, -0.5, -0.5),
       vec3(-0.25, -0.25, 0.5)},

      //east face
      {vec3(0.25, -0.25, 0.5),
       vec3(0.5, -0.5, -0.5),
       vec3(0.5, 0.5, -0.5),
       vec3(0.25, 0.25, 0.5)},

      //north face
      {vec3(0.25, 0.25, 0.5),
       vec3(0.5, 0.5, -0.5),
       vec3(-0.5, 0.5, -0.5),
       vec3(-0.25, 0.25, 0.5)},

      //south face
      {vec3(-0.25, -0.25, 0.5),
       vec3(-0.5, -0.5, -0.5),
       vec3(0.5, -0.5, -0.5),
       vec3(0.25, -0.25, 0.5)}};

  glGenVertexArrays(NUMFACES, vao);
  glGenBuffers(NUMFACES, buffer);

  GLuint program = InitShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);
  GLuint loc = glGetAttribLocation(program, "vPosition");
  modelViewLoc = glGetUniformLocation(program, "uModelView");
  projLoc = glGetUniformLocation(program, "uProjection");
  colourLoc = glGetUniformLocation(program, "uColour");

  for (int i = 0; i < NUMFACES; i++)
  {
    glBindVertexArray(vao[i]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points[i]), points[i], GL_STATIC_DRAW);
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void)
{
  vec4 colours[NUMFACES] = {
      {1, 0, 0, 1},
      {0, 1, 0, 1},
      {0, 0, 1, 1},
      {1, 1, 1, 1},
      {0, 1, 1, 1},
      {1, 0, 1, 1},
  };

  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  mat4 model = Translate(pos_x, pos_y, pos_z) * Scale(scale) * RotateX(theta_x) * RotateY(theta_y) * RotateZ(theta_z);

  vec4 eye(0, -1, 1, 1), at(0, 0, 0, 1), up(0, 0, 1, 0);
  mat4 view = LookAt(eye, at, up);
  mat4 modelView = view * model;
  glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, modelView);

  mat4 proj = Ortho(-1, 1, -1, 1, -1, 100);
  glUniformMatrix4fv(projLoc, 1, GL_TRUE, proj);

  for (int i = 0; i < NUMFACES; i++)
  {
    glUniform4fv(colourLoc, 1, colours[i]);
    glBindVertexArray(vao[i]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }

  glutSwapBuffers();
}

void keyboard(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_PAGE_UP:
    scale *= 1.05;
    break;
  case GLUT_KEY_PAGE_DOWN:
    scale /= 1.05;
    break;
  case GLUT_KEY_HOME:
    theta_x += 0.5;
    break;
  case GLUT_KEY_END:
    theta_y += 0.5;
    break;
  case GLUT_KEY_INSERT:
    theta_z += 0.5;
    break;
  case GLUT_KEY_LEFT:
    pos_x -= 0.05;
    break;
  case GLUT_KEY_RIGHT:
    pos_x += 0.05;
    break;
  case GLUT_KEY_UP:
    pos_y += 0.05;
    break;
  case GLUT_KEY_DOWN:
    pos_y -= 0.05;
    break;
  }
  glutPostRedisplay();
}

void escapeKey(unsigned char key, int x, int y)
{
  if (key == 27)
  {
    exit(EXIT_SUCCESS);
  }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);

  // If you are using freeglut, the next two lines will check if
  // the code is truly 3.2. Otherwise, comment them out

  glutInitContextVersion(3, 2);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutCreateWindow("Mackenzie Broughton - CPSC 3710 - Assignment 1");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutSpecialFunc(keyboard);
  glutKeyboardFunc(escapeKey);

  glutMainLoop();
  return 0;
}
