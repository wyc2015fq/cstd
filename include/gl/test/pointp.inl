/*
 *  pointp.c
 *  This program demonstrates point parameters and their effect
 *  on point primitives.
 *  250 points are randomly generated within a 10 by 10 by 40
 *  region, centered at the origin.  In some modes (including the
 *  default), points that are closer to the viewer will appear larger.
 *
 *  Pressing the 'l', 'q', and 'c' keys switch the point
 *  parameters attenuation mode to linear, quadratic, or constant,
 *  respectively.
 *  Pressing the 'f' and 'b' keys move the viewer forward
 *  and backwards.  In either linear or quadratic attenuation
 *  mode, the distance from the viewer to the point will change
 *  the size of the point primitive.
 *  Pressing the '+' and '-' keys will change the current point
 *  size.  In this program, the point size is bounded, so it
 *  will not get less than 2.0, nor greater than GL_POINT_SIZE_MAX.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef GL_VERSION_1_4
static GLfloat psize = 7.0;
static GLfloat pmax[1];
static GLfloat constant[3] = {1.0, 0.0, 0.0};
static GLfloat linear[3] = {0.0, 0.12, 0.0};
static GLfloat quadratic[3] = {0.0, 0.0, 0.01};
void init(void)
{
  int i;
  srand(12345);
  glNewList(1, GL_COMPILE);
  glBegin(GL_POINTS);

  for (i = 0; i < 250; i++) {
    glColor3f(1.0, ((rand() / (float) RAND_MAX) * 0.5) + 0.5,
        rand() / (float) RAND_MAX);
    /*  randomly generated vertices:
        -5 < x < 5;  -5 < y < 5;  -5 < z < -45  */
    glVertex3f(((rand() / (float)RAND_MAX) * 10.0) - 5.0,
        ((rand() / (float)RAND_MAX) * 10.0) - 5.0,
        ((rand() / (float)RAND_MAX) * 40.0) - 45.0);
  }

  glEnd();
  glEndList();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPointSize(psize);
  glGetFloatv(GL_POINT_SIZE_MAX_EXT, pmax);
  glPointParameterfvEXT(GL_DISTANCE_ATTENUATION_EXT, linear);
  glPointParameterfEXT(GL_POINT_FADE_THRESHOLD_SIZE_EXT, 2.0);
}
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glCallList(1);
  glutSwapBuffers();
}
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(35.0, 1.0, 0.25, 200.0);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0.0, 0.0, -10.0);
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'b':
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0, 0.0, -0.5);
    glutPostRedisplay();
    break;

  case 'c':
    glPointParameterfvEXT(GL_DISTANCE_ATTENUATION_EXT, constant);
    glutPostRedisplay();
    break;

  case 'f':
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0, 0.0, 0.5);
    glutPostRedisplay();
    break;

  case 'l':
    glPointParameterfvEXT(GL_DISTANCE_ATTENUATION_EXT, linear);
    glutPostRedisplay();
    break;

  case 'q':
    glPointParameterfvEXT(GL_DISTANCE_ATTENUATION_EXT, quadratic);
    glutPostRedisplay();
    break;

  case '+':
    if (psize < (pmax[0] + 1.0)) {
      psize = psize + 1.0;
    }

    glPointSize(psize);
    glutPostRedisplay();
    break;

  case '-':
    if (psize >= 2.0) {
      psize = psize - 1.0;
    }

    glPointSize(psize);
    glutPostRedisplay();
    break;

  case 27:
    exit(0);
    break;
  }
}
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
#else
int main(int argc, char** argv)
{
  fprintf(stderr, "This program demonstrates a feature which is not in OpenGL Version 1.4.\n");
  fprintf(stderr, "If your implementation of OpenGL Version 1.4 has the right extensions,\n");
  fprintf(stderr, "you may be able to modify this program to make it run.\n");
  return 0;
}
#endif

