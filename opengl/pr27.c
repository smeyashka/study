#include <GL/glut.h>
#include <stdlib.h>

GLfloat v0[] = {3.0,  3.0,  0.0},
        v1[] = {17.0, 12.0, 0.0},
	v2[] = {10.0, 19.0, 0.0},
	v5[] = {25.0, 25.0, 0.0};

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_POLYGON);
  //  glBegin(GL_TRIANGLE_FAN);
  glEdgeFlag(GL_TRUE);
  glVertex3fv(v0);
  glEdgeFlag(GL_FALSE);
  glVertex3fv(v1);
  glEdgeFlag(GL_TRUE);
  glVertex3fv(v2);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3fv(v1); 
  glVertex3fv(v5);
  glVertex3fv(v2);
  glEnd();
  glFlush();
}

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 30, 0.0, 30);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(300, 300);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
