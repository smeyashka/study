#include <GL/glut.h>
#include <stdlib.h>

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, 7.0, -1.0, 4.0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);

  //  glBegin(GL_POLYGON);
  //  glBegin(GL_POINTS);
  //  glBegin(GL_LINES);
  //  glBegin(GL_LINE_STRIP);
  glBegin(GL_LINE_LOOP);
  //  glBegin(GL_TRIANGLES);
  //  glBegin(GL_TRIANGLE_STRIP);
  // glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0.0, 0.0);
  glVertex2f(0.0, 3.0);
  glVertex2f(4.0, 3.0);
  glVertex2f(6.0, 1.5);
  glVertex2f(4.0, 0.0);

  glEnd();
  glFlush();
}

int main(int argc,  char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(250, 250);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
