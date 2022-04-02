#include <GL/glut.h>
#include <stdlib.h>

GLfloat k1[] = {-5.0, -5.0, 0.0};
GLfloat k2[] = {5.0, -5.0, 0.0};
GLfloat k3[] = {0.0, 5.0, 0.0};

void  draw_triangle()
{
  glBegin(GL_TRIANGLES);
  glVertex3fv(k1);
  glVertex3fv(k2);
  glVertex3fv(k3);
  glEnd();

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 1.0);

/*   glLoadIdentity(); */
/*   gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); */

  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  draw_triangle();
  glPopMatrix();
  glPushMatrix();
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xF0F0);
  glTranslatef(-5.0, 5.0, 0.0);
  draw_triangle();
  glPopMatrix();

  glPushMatrix();
  glLineStipple(1, 0xF00F);
  glScalef(0.5, 1.7, 1.0);
  draw_triangle();
  glPopMatrix();  

  glPushMatrix();
  glLineStipple(1, 0x8888);
  glRotatef(60.0, 0.0, 0.0, 1.0);
  draw_triangle();
  glPopMatrix();
  glDisable(GL_LINE_STIPPLE);
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
  // glOrtho(-10.0, 10.0, -10.0, 10.0,-5.0,5.0);
  gluPerspective(120.0,1.0,0.0,20.0);
  // glFrustum(-10.0, 10.0, -10.0, 10.0, 4.0, 6.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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
