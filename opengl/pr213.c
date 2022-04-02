#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define x 0.525731112119133606
#define z 0.850650808352039932

void normalize(float v[3])
{
  GLfloat d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  if(d == 0.0) {
    printf("вектор нулевой длины");
    return;
  }
  v[0] /= d;
  v[1] /= d;
  v[2] /= d;
}

void normcrossprod(float v1[3], float v2[3], float out[3])
{
  out[0] = v1[1]*v2[2] - v1[2]*v2[1];
  out[1] = v1[2]*v2[0] - v1[0]*v2[2];
  out[2] = v1[0]*v2[1] - v1[1]*v2[0];
  normalize(out);
}

void display()
{
  static GLfloat vdata[12][3] = {
    {-x, 0.0, z}, {x, 0.0, z}, {-x, 0.0, -z}, {x, 0.0, -z},
    {0.0, z, x}, {0.0, z, -x}, {0.0, -z, x}, {0.0, -z, -x},
    {z, x, 0.0}, {-z, x, 0.0}, {z, -x, 0.0}, {-z, -x, 0.0}
  };
  static GLuint tindices [20][3] = {
    {1, 4, 0},  {4, 9, 0},  {4, 5, 9},  {8, 5, 4},  {1, 8, 4},
    {1, 10, 8}, {10, 3, 8}, {8, 3, 5},  {3, 2, 5},  {3, 7, 2},
    {3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
    {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9},  {11, 2, 7}
  };

  GLfloat d1[3], d2[3], norm[3];

  int idx, jdx;
  glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);

  glBegin(GL_TRIANGLES);
  for(idx = 1; idx < 20; idx++){
    glColor3f(idx/20.0 + 0.1, idx/30, 1.0);

    for(jdx = 0; jdx < 3; jdx++) {
      d1[jdx] = vdata[tindices[idx][0]][jdx] - vdata[tindices[idx][1]][jdx];
      d2[jdx] = vdata[tindices[idx][1]][jdx] - vdata[tindices[idx][2]][jdx];
    }
    normcrossprod(d1, d2, norm);
    glNormal3fv(norm);
    
    glVertex3fv(&vdata[tindices[idx][0]][0]);
    glVertex3fv(&vdata[tindices[idx][1]][0]);
    glVertex3fv(&vdata[tindices[idx][2]][0]);
  }
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
  glViewport(0.0, 0.0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, (GLdouble) 1,  -1.0, (GLdouble) 1);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(350, 350);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
