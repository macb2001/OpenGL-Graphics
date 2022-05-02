/*
 * Mackenzie Broughotn
 * CPSC 3710 - Spring 2022
 * Assignment 2
 */

#include <Angel.h>
#include <iostream>

using namespace std;

// Global variable declarations
GLfloat theta_x = 0.0, theta_y = 0.0, theta_z = 0.0;
vec4 pos(0.0, 0.0, 0.0, 0.0);
vec3 scale(1.0, 1.0, 1.0);
GLint faceColourLoc, modelViewLoc, projLoc;
GLuint vao[6], buffer[6];
bool usePerspective = false;
GLfloat cameraTheta = 3 * M_1_PI/2, cameraRadius = 1.0;

// Vertex declarations
const vec4 point[8] = {
  vec4(-0.5, -0.5, -0.5, 1),
  vec4(0.5, -0.5, -0.5, 1),
  vec4(0.5, 0.5, -0.5, 1),
  vec4(-0.5, 0.5, -0.5, 1),
  vec4(-0.25, -0.25, 0.5, 1),
  vec4(0.25, -0.25, 0.5, 1),
  vec4(0.25, 0.25, 0.5, 1),
  vec4(-0.25, 0.25, 0.5, 1)
};

// Colour declarations
const vec4 colour[6] = {
  vec4(1,0,0,1), // red
  vec4(1,1,0,1), // yellow
  vec4(0,1,0,1), // green
  vec4(0,1,1,1), // cyan
  vec4(0,0,1,1), // blue
  vec4(1,0,1,1)  // magenta
};

// Face declarations
const int face[6][4] = {
  {0,1,2,3},  // bot
  {4,5,6,7},  // top
  {0,1,5,4},  // back
  {2,3,7,6},  // front
  {2,6,5,1},  // left
  {0,4,7,3}   // right
};

// Initialize method. Makes and binds vertex array objects, loads shader files.
void init() {
  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);
  faceColourLoc = glGetUniformLocation(program, "uFaceColour");
  modelViewLoc = glGetUniformLocation(program, "uModelView");
  projLoc = glGetUniformLocation(program, "uProjection");

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation( program, "vPosition" );

  // Create a vertex array object for each face
  glGenVertexArrays(6, vao);
  glGenBuffers(6, buffer);

  for (int i = 0; i < 6; i++) {
    glBindVertexArray(vao[i]);
  
    // initialize a buffer object
    glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);

    vec4 A[4];
    for (int j = 0; j < 4; j++) {
      A[j] = point[face[i][j]];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(A), A, GL_STATIC_DRAW);

    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  }
  
  glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background

  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
}

// Method to return the alternate projection view of the current one.
mat4 getProjection(mat4 modelView) {
    GLfloat z1 = 1e10, z2 = -1e10;

    for (int i = 0; i < 8; i++) {
        auto p = modelView * point[i];
        z1 = min(z1, -p.z);
        z2 = max(z2, -p.z);
    }

    GLfloat near, far;

    if (usePerspective) {       // If current projection is orthogonal, return perspective.
        near = z1 - 0.01;
        far = z2 + 0.01;
        return Perspective(90, 1.0, near, far);
    } else {        // If current projection is perspective, return orthogonal.
        near = z1 - 0.5;
        far = z2 + 0.5;;
        return Ortho(-1.0, 1.0, -1.0, 1.0, near, far);
    }
}

// Method to draw the object
void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  mat4 rotate = RotateX(theta_x) * RotateY(theta_y) * RotateZ(theta_z);
  mat4 model = Translate(pos) * rotate * Scale(scale);

  GLfloat eye_x = cameraRadius * cos(cameraTheta);
  GLfloat eye_y = cameraRadius * sin(cameraTheta);
  vec4 eye(eye_x, eye_y, 1, 1);
  vec4 at(0,0,0,1);
  vec4 up(0,0,1,0);
  mat4 view = LookAt(eye, at, up);
  mat4 modelview = view * model;
  glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, modelview);

  mat4 proj = getProjection(modelview);
  glUniformMatrix4fv(projLoc, 1, GL_TRUE, proj);
  
  for (int i = 0; i < 6; i++) {
    glUniform4fv(faceColourLoc, 1, colour[i]);
    glBindVertexArray(vao[i]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    // draw the square
  }
  glutSwapBuffers();
}

// Method for glutKeyboardFunc parameter
// Performs action based on what key is pressed (for 'esc' and 'p')
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
      // When 'esc' is pressed, exit the program
      case 033:
          exit( EXIT_SUCCESS );
          break;
      // When 'p' is pressed, switch between orthogonal and perspective projection
      case 'p':
          usePerspective = !usePerspective;
          glutPostRedisplay();
          break;
  }
}

// Method for glutSpecialFunc parameter
// Perform actions based on which special keyboard keys are pressed
void arrow(int key, int x, int y) {
  switch (key) {
      case GLUT_KEY_LEFT:
        pos -= vec4(0.05, 0, 0, 0);
        break;
      case GLUT_KEY_RIGHT:
        pos += vec4(0.05, 0, 0, 0);
        break;
      case GLUT_KEY_UP:
        pos += vec4(0, 0, 0.05, 0);
        break;
      case GLUT_KEY_DOWN:
        pos -= vec4(0, 0, 0.05, 0);
        break;
      case GLUT_KEY_PAGE_UP:
        scale *= 1.05;
        break;
      case GLUT_KEY_PAGE_DOWN:
        scale *= 1.0/1.05;
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
  }
  glutPostRedisplay();
}

// Method for glutMouseFunc
// Performs actions based on which mouse button is clicked
void mouseButtons(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch(button) {
            case GLUT_LEFT_BUTTON: // Move camera counter-clockwise along circle by 0.2 radians
                cameraTheta -= 0.2;
                break;
            case GLUT_RIGHT_BUTTON: // Move camera clockwise along circle by 0.2 radians
                cameraTheta += 0.2;
                break;
            case 3: // Decrease camera radius by 0.1
               if (cameraRadius > 1.0) { // Ensure that radius is not less than 1.0
                    cameraRadius -= 0.1;
               }
               break;
            case 4: // Increase camera radius by 0.1
                cameraRadius += 0.1;
                break;
        }
        glutPostRedisplay();
    }
}

// Main method
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
    
  glutInitContextVersion(3, 2);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutCreateWindow("Mackenzie Broughton - Assignment 2");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow);
  glutMouseFunc(mouseButtons);
  
  glutMainLoop();

  return 0;
}
