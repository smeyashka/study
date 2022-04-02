#include <GL/glut.h>
#include <stdlib.h>

void drawOneLine(float x1, float y1, float x2, float y2)
{
  glBegin(GL_LINES);
  glVertex2f((x1),(y1));
  glVertex2f((x2),(y2));
  glEnd();
}

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void display()
{
  int idx;

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0x0101);
  drawOneLine(50.0, 125.0, 150.0, 125.0);
  glLineStipple(1, 0x00FF);
  drawOneLine(150.0, 125.0, 250.0, 125.0);
  glLineStipple(1, 0x1C47);
  drawOneLine(250.0, 125.0, 350.0, 125.0);

  glLineWidth(5.0);
  glLineStipple(1, 0x0101);
  drawOneLine(50.0, 100.0, 150.0, 100.0);
  glLineStipple(1, 0x00FF);
  drawOneLine(150.0, 100.0, 250.0, 100.0);
  glLineStipple(1, 0x1C47);
  drawOneLine(250.0, 100.0, 350.0, 100.0);

  glLineStipple(1, 0x1C47);
  glBegin(GL_LINE_STRIP);
  for (idx = 0; idx < 7; idx++) {
    glVertex2f(50.0 + ((GLfloat)idx*50.0), 75.0);
  }
  glEnd();

  for (idx = 0; idx < 6; idx++) {
    drawOneLine(50.0 + ((GLfloat)idx*50.0), 50.0, 50.0+((GLfloat)(idx+1)*50.0), 50.0); 
  }

  glLineStipple(5, 0x1C47);
  drawOneLine(50.0, 25.0, 350.0, 25.0);
  glDisable(GL_LINE_STIPPLE);

  glColor3f(0.0, 1.0, 1.0);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glPolygonMode(GL_FRONT,  GL_LINE);
  glPolygonMode(GL_BACK, GL_FILL);
  glRectf(50.0,  150.0,  300.0,  200.0);

  //glFrontFace(GL_CW);
  glPolygonMode(GL_FRONT,  GL_POINT);
  glPolygonMode(GL_BACK, GL_LINE);
  glRectf(50.0,  230.0,  300.0,  270.0);
  glDisable(GL_CULL_FACE);

  glFlush();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //  gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
  gluOrtho2D(0.0, 400, 0.0, 400);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(400, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
