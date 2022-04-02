#include <GL/glut.h>
#include <stdlib.h>

static GLint vertices[] = { 25,  25,
			    100, 325,
			    175, 25,
			    175, 325,
			    250, 25,
			    325, 325,
			    				    
			    25,  25,
			    100, 325,
			    175, 25,
			    175, 325,
			    250, 25,
			    325, 325 };

static GLfloat colors[] = { 1.0,  0.2,  0.2,
			    1.0,  1.0,  0.0,
			    1.0,  1.0,  0.0,
			    1.0,  1.0,  0.0,
			    0.35, 0.35, 0.35,
			    1.0,  1.0,  0.0,
 
			    1.0,  0.2,  0.2,
			    0.0,  0.0,  0.0,
			    0.0,  0.0,  0.0,
			    0.0,  0.0,  0.0,
			    0.35, 0.35, 0.35,
			    0.0,  0.0,  0.0 };


void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //  glColor3f(0.0, 1.0, 0.0);

  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(3, GL_FLOAT, 0, colors);
  glVertexPointer(2, GL_INT, 0, vertices);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBegin(GL_TRIANGLES);
  glArrayElement(2);
  glArrayElement(3);
  glArrayElement(5);
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  glLineWidth(5.0);
  glLineStipple(1, 0x00FF);
  glPolygonMode(GL_FRONT, GL_LINE);
  glPolygonMode(GL_BACK, GL_LINE);

/*   glBegin(GL_TRIANGLES); */
/*   glArrayElement(8); */
/*   glArrayElement(9); */
/*   glArrayElement(11); */
/*   glEnd(); */

  glDisableClientState(GL_COLOR_ARRAY);

  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_TRIANGLES);
  glArrayElement(2);
  glArrayElement(3);
  glArrayElement(5);
  glEnd();

  glDisable(GL_LINE_STIPPLE);
  glFlush();
}

void init()
{
  glClearColor(0.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 350, 0.0, 350);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
