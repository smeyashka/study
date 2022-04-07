#include <GL/glut.h>
#include <stdlib.h>

GLsizei wh = 500, ww = 500;
GLfloat size = 3.0;

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
}


void init()
{
  glViewport(0, 0, ww, wh);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble) wh);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void drawSquare(int x, int y)
{
  y = wh - y;
  glColor3ub((char)rand()%256, (char)rand()%256, (char)rand()%256);
  glBegin(GL_POLYGON);
  glVertex2f(x+size, y+size);
  glVertex2f(x-size, y+size);
  glVertex2f(x-size, y-size);
  glVertex2f(x+size, y-size);
  glEnd();
  glFlush();
}

void reshape(GLsizei w, GLsizei h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  wh = h;
  ww = w;
}

void mouse(int btn, int state, int x, int y)
{
  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    drawSquare(x, y);
  }
  
  if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    exit(0);
  }
}

void menu(int id)
{
  if (id == 1) exit(0);
  else if (id == 2) size = 2*size;
  else if (size > 1) size /= 2.0;
  glutPostRedisplay();
  // glutDestroyWindow(2);
}

void size_menu(int id)
{
  if (id == 2) {
    size = 2*size;
  } else if (size > 1) {
    size /= 2.0;
  }
  glutPostRedisplay();
}

void top_menu(int id)
{
  if (id == 1) exit(0);
}

int main(int argc, char **argv)
{
  //  int id;
  short sub_menu;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("square");
  init();
 /*  id=glutCreateWindow("second window"); */
/*   printf("id=%d\n", id); */
/*   glutSetWindow(id); */
//  glutCreateMenu(menu);
  sub_menu = glutCreateMenu(size_menu);
  //  glutAddMenuEntry("quit", 1);
  glutAddMenuEntry("increase square size", 2);
  glutAddMenuEntry("decrease square size", 3);
  glutCreateMenu(top_menu);
  glutAddMenuEntry("quit", 1);
  glutAddSubMenu("Resize", sub_menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
 /*  glutSetWindow(1); */
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(drawSquare);
 
  glutMainLoop();

  return 0;
}
