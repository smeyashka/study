#include <GL/glut.h>
#include <stdlib.h>

static int year = 0, day = 0;

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 0.8, 0.0);
  glPushMatrix();
  glutWireSphere(1.0, 25, 20);

  glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
  glTranslatef(2.0, 0.0, 0.0);
  glPushMatrix();

  glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);
  glutWireSphere(0.2, 10, 10);
  glPopMatrix();

  glColor3f(1.0, 1.0, 1.0);
  glRotatef((GLfloat)day, 0.0,  0.0, 1.0);
  glTranslatef(0.4, 0.0, 0.0);
  glutWireSphere(0.1, 8, 8);
  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 20);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.3, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'd': 
    day = (day + 10) % 360;
    glutPostRedisplay();
    break;
  case 'D': 
    day = (day - 10) % 360;
    glutPostRedisplay();
    break;
  case 'y': 
    year = (year + 5) % 360;
    glutPostRedisplay();
    break;
  case 'Y': 
    year = (year - 5) % 360;
    glutPostRedisplay();
    break;
  default: {}
  }
}

void spinDisplay()
{
  day = (day + 10) % 360;
  year = (year + 5) % 360;
  glutPostRedisplay();
}

void mouse(int button, int state,  int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      glutIdleFunc(spinDisplay);  
    } else {
      glutIdleFunc(NULL);
    }
    break;

/*   case GLUT_RIGHT_BUTTON: */
/*     if (state==GLUT_DOWN) { */
/*       day=(day-10)%360; */
/*       year=(year-5)%360; */
/*       glutPostRedisplay(); */
/*     } */
/*     break; */
  default: break;
  }
}

int main(int argc,  char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  //  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();

  return 0;
}
