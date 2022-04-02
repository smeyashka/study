#include <GL/glut.h>
#include <stdio.h>

typedef GLfloat point3[3];

//point3 v[4]={{0.0, 0.0, 0.0}, {250.0, 500.0, 100.0}, {500.0, 250.0, 250.0}, {250.0, 100.0, 250.0}};
int n;

void myinit()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5.0, 55.0,  -5.0, 55.0,  -50.0, 50.0);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.8, 0.8, 0.8, 0.0);
  glColor3f(1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
}

void triangle(point3 a, point3 b, point3 c)
{
  glBegin(GL_TRIANGLES);
  glNormal3fv(a);
  glVertex2fv(a);
  glVertex2fv(b);
  glVertex2fv(c);
  glEnd();
}

void divide_triangle(point3 a, point3 b, point3 c, int k)
{
  point3 ab, ac, bc;
  int j;
 
  if (k > 0) {
    for (j = 0; j < 3; j++) ab[j] = (a[j] + b[j])/2;
    for (j = 0; j < 3; j++) ac[j] = (a[j] + c[j])/2;
    for (j = 0; j < 3; j++) bc[j] = (b[j] + c[j])/2;

    divide_triangle(a, ab, ac, k-1);
    divide_triangle(c, ac, bc, k-1);
    divide_triangle(b, bc, ab, k-1);
  } else triangle(a, b, c);

}

void tetrahedron(int n)
{
  // point3 v[4]={{0.0, 0.0, 0.0}, {25.0, 50.0, 0.0}, {50.0, 0.0, 0.0}, {25.0, 25.0, 50.0}};
  point3 v[4] = {{0.0, 0.0, 0.0}, {50.0, 0.0, 0.0}, {25.0, 50.0, 0.0}, {25.0, 25.0, 30.0}};
  glColor3f(1.0, 0.0, 0.0);
  divide_triangle(v[0], v[1], v[2], n);
  glColor3f(0.0, 1.0, 0.0);
  divide_triangle(v[3], v[2], v[1], n);
  glColor3f(0.0, 0.0, 1.0);
  divide_triangle(v[0], v[3], v[1], n);
  glColor3f(0.0, 0.0, 0.0);
  divide_triangle(v[0], v[2], v[3], n);
}

void display()
{
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
  
  tetrahedron(n);
  glFlush();
}

int main(int argc, char **argv)
{
  if (argc > 1) {
    n = atoi(argv[1]);
  } else {
    n=4;
  }
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB| GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Gasket");
  glutDisplayFunc(display);
  myinit();
  glutMainLoop();

  return 0;
}
