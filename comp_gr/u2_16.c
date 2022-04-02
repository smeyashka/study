#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

GLfloat v0[]={10, 10, 0};
GLfloat v1[]={70, 10, 0};
GLfloat v2[]={40, 60, 0};

void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-5.0, 80.0, -15.0, 70.0);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //glBegin(GL_LINE_LOOP);
  //  glBegin(GL_POLYGON);
  glBegin(GL_TRIANGLES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3fv(v0);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3fv(v1);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3fv(v2);
  glEnd();
  glFlush();
}

int main(int argc,char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Maxwell triangle");
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
