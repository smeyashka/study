#include <GL/glut.h>
#include <stdio.h>

typedef GLfloat point3[3];


point3 verticies[4] = {{0.0, 0.0, 0.0},
		       {50.0, 0.0, 0.0},
		       {25.0, 50.0, 0.0},
		       {25.0, 25.0, 30.0}};
point3 p = {250.0, 100.0, 250.0};
int j_rand = 0;

void myinit()
{
  glClearColor(0.5, 0.0, 0.8, 0.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 50.0,  0.0, 50.0,  -50.0, 50);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT);
}

void display()
{
  int rand();
  int k;

  glClear(GL_COLOR_BUFFER_BIT);
  for (k = 0; k < 50000; k++) {
    j_rand = rand()%4;
    p[0] = (p[0] + verticies[j_rand][0]) / 2.0;
    p[1] = (p[1] + verticies[j_rand][1]) / 2.0;
    p[2] = (p[2] + verticies[j_rand][2]) / 2.0;
    
    glBegin(GL_POINTS);
    glColor3f(p[0]/25.0, p[1]/25.0, p[2]/25.0);
    glVertex3fv(p);
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
