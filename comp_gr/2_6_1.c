#include <GL/glut.h>
#include <stdio.h>

typedef GLfloat point2[2] ;

point2 v[3] = {{0.0,   0.0},
	       {250.0, 500.0},
	       {500.0, 0.0}};
int n;

void myinit()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 500.0, 0.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.2, 0.0, 0.5, 0.0);
  glColor3f(1.0, 1.0, 0.0);
}

void triangle(point2 a, point2 b, point2 c)
{
  glBegin(GL_TRIANGLES);
  glVertex2fv(a);
  glVertex2fv(b);
  glVertex2fv(c);
  glEnd();
}

void divide_triangle(point2 a, point2 b, point2 c, int k)
{
  point2 ab, ac, bc;
  int j;
 
  if (k > 0) {
    for (j = 0; j < 2; j++) ab[j] = (a[j]+b[j])/2;
    for (j = 0; j < 2; j++) ac[j] = (a[j]+c[j])/2;
    for (j = 0; j < 2; j++) bc[j] = (b[j]+c[j])/2;

    divide_triangle(a, ab, ac, k-1);
    divide_triangle(c, ac, bc, k-1);
    divide_triangle(b, bc, ab, k-1);
  } else triangle(a, b, c);

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  divide_triangle(v[0], v[1], v[2], n);
  glFlush();
}

int main(int argc, char **argv)
{
  if (argc > 1) {
    n = atoi(argv[1]);
  } else {
    n = 2;
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Gasket");
  glutDisplayFunc(display);
  myinit();
  glutMainLoop();

  return 0;
}
