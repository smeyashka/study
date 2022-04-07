#include <GL/glut.h>

void myinit()
{
  glClearColor(0.2, 0.0, 0.5, 0.0);
  glColor3f(1.0, 1.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 500.0, 0.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
}

void display()
{
  typedef GLfloat point2[2] ;

  point2 vertices[3] = {{0.0, 0.0}, {250.0, 500.0}, {500.0, 0.0}};
  int j, k;
  rand();
  point2 p = {75.0, 75.0};

  glClear(GL_COLOR_BUFFER_BIT);

  for (k = 0; k< 8000; k++) {
    j = rand()%3;

    p[0] = (p[0]+vertices[j][0])/2.0;
    p[1] = (p[1]+vertices[j][1])/2.0;

    glBegin(GL_POINTS);
    glVertex2fv(p);
    glEnd();
  }
  glFlush();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Gasket");
  glutDisplayFunc(display);
  myinit();
  glutMainLoop();

  return 0;
}
