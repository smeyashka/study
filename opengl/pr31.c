#include <GL/glut.h>
#include <stdlib.h>

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glLoadIdentity();
  gluLookAt(0.8, 0.8, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  //glTranslatef(0.0, 0.0, -5.0);
  //    glScalef(1.0, 1.0, 2.0);
  //  glutWireSphere(2.0,20,20);
  glutWireCube(1.0);
  //  glutWireCone(2.0,3.0,30.0,2.0);
  //  glutSolidTeapot(1.0);
/*   glLoadIdentity(); */
/*   gluLookAt(-1.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); */
/*   glScalef(1.0, 1.0, 2.0); */
/*   glTranslatef(1.0, 0.0, 0.0); */
/*   glutWireCube(1.0); */
  glFlush();
}

void reshape(int w, int h)
{
  glViewport(0.0, 0.0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
  // gluPerspective(60.0, 1.0, 0.0, 20.0);
  glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
