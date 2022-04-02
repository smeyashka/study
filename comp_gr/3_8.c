#include <GL/glut.h>
#include <stdlib.h>

//#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE 3
#define POINTS 4
#define TEXT 5

GLsizei wh = 500, ww = 500;
GLfloat size = 3.0;
int draw_mode = 0;
int rx, ry;

GLfloat r = 1.0, g = 1.0, b = 1.0;
int fill = 0;

void display();
int pick(int x, int y);

void drawSquare(int x, int y)
{
  y = wh - y;
  glColor3ub((char) rand()%256,
	     (char) rand()%256,
	     (char) rand()%256);
  glBegin(GL_POLYGON);
  glVertex2f(x + size, y + size);
  glVertex2f(x - size, y + size);
  glVertex2f(x - size, y - size);
  glVertex2f(x + size, y - size);
  glEnd();
}

void reshape(GLsizei w,GLsizei h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glViewport(0, 0, w, h);
  glClearColor(0.8, 0.8, 0.8, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  display();
  glFlush();
}

void mouse(int btn, int state, int x, int y)
{
  static int count;
  int where;
  static int xp[2], yp[2];
  if (btn == GLUT_LEFT && state == GLUT_DOWN) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    where = pick(x,y);
    glColor3f(r,g,b);
    if (where != 0) {
      count = 0;
      draw_mode = where;
    } else {
      switch(draw_mode) {
      case(LINE):
	if (count == 0) {
	  count++;
	  xp[0] = x;
	  yp[0] = y;
	} else {
	  glBegin(GL_LINES);
	  glVertex2i(x, wh - y);
	  glVertex2i(xp[0], wh - yp[0]);
	  glEnd();
	  draw_mode = 0;
	  count = 0;
	}
	break;
      case (RECTANGLE):
	if (count == 0) {
	  count++;
	  xp[0] = x;
	  yp[0] = y;
	} else {
	  if (fill) glBegin(GL_POLYGON);
	  else glBegin(GL_LINE_LOOP);
	  glVertex2i(x, wh - y);
	  glVertex2i(x, wh - yp[0]);
	  glVertex2i(xp[0], wh - yp[0]);
	  glVertex2i(xp[0], wh - y);
	  glEnd();
	  draw_mode = 0;
	  count = 0;
	}
	break;
      case (TRIANGLE):
	switch(count) {
	case (0):
	  count++;
	  xp[0] = x;
	  yp[0] = y;
	  break;
	case (1):
	  count++;
	  xp[1] = x;
	  yp[1] = y;
	  break;
	case (2):
	  if (fill) glBegin(GL_POLYGON);
	  else glBegin(GL_LINE_LOOP);
	  glVertex2i(xp[0], wh-yp[0]);
	  glVertex2i(xp[1], wh-yp[1]);
	  glVertex2i(x, wh-y);
	  glEnd();
	  draw_mode = 0;
	  count = 0;
	}
	break;
      case (POINTS): 
	drawSquare(x, y);
	count++;
	break;
      case (TEXT):
	rx = x;
	ry = wh - y;
	glRasterPos2i(rx, ry);
	count=0;
      }
    }
  }
  glPopAttrib();
  glFlush();
}
  
int pick(int x,int y)
{
  y = wh - y;
  if (y < wh - ww/10) return 0;
  else if (x < ww/10) return LINE;
  else if (x < ww/5)  return RECTANGLE;
  else if (x < 3*ww/10) return TRIANGLE;
  else if (x < 2*ww/5)  return POINTS;
  else if (x < ww/2)    return TEXT;
  else return 0;
}

void screen_box(int x,int y,int s)
{
  glBegin(GL_QUADS);
  glVertex2i(x,   y);
  glVertex2i(x+s, y);
  glVertex2i(x+s, y+s);
  glVertex2i(x,   y+s);
  glEnd();
}

void right_menu(int id)
{
  if (id==1) exit(0);
  else display();
}

void middle_menu(int id) 
{}

void color_menu(int id)
{
  if      (id == 1) {r = 1.0; g = 0.0; b = 0.0;}
  else if (id == 2) {r = 0.0; g = 1.0; b = 0.0;}
  else if (id == 3) {r = 0.0; g = 0.0; b = 1.0;}
  else if (id == 4) {r = 0.0; g = 1.0; b = 1.0;}
  else if (id == 5) {r = 1.0; g = 0.0; b = 1.0;}
  else if (id == 6) {r = 1.0; g = 1.0; b = 0.0;}
  else if (id == 7) {r = 1.0; g = 1.0; b = 1.0;}
  else if (id == 8) {r = 0.0; g = 0.0; b = 0.0;}
}

void pixel_menu(int id)
{
  if (id == 1) size *= 2;
  else if (size > 1) size /= 2;
}

void fill_menu(int id)
{
  if (id == 1) fill = 1;
  else fill = 0;
}

void display()
{
  int shift=0;

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClearColor(0.8, 0.8, 0.8, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  screen_box(0, wh-ww/10, ww/10);
  glColor3f(1.0, 0.0, 0.0);
  screen_box(ww/10, wh-ww/10, ww/10);
  glColor3f(0.0, 1.0, 0.0);
  screen_box(ww/5, wh-ww/10, ww/10);
  glColor3f(0.0, 0.0, 1.0);
  screen_box(3*ww/10, wh-ww/10, ww/10);
  glColor3f(1.0, 1.0, 0.0);
  screen_box(2*ww/5, wh-ww/10, ww/10);
  glColor3f(0.0, 0.0, 0.0);

  glBegin(GL_LINES);
  glVertex2i(wh/40, wh-ww/20);
  glVertex2i(wh/40+ww/20, wh-ww/20);
  glEnd();

  glBegin(GL_QUADS);
  glVertex2i(ww/10+wh/40, wh-ww/10+ww/40);
  glVertex2i(ww/10+3*wh/40, wh-ww/10+ww/40);
  glVertex2i(ww/10+3*wh/40, wh-ww/40);
  glVertex2i(ww/10+wh/40, wh-ww/40);
  glEnd();


  glBegin(GL_TRIANGLES);
  glVertex2i(ww/5+ww/40, wh-ww/10+ww/40);
  glVertex2i(ww/5+ww/20, wh-ww/40);
  glVertex2i(ww/5+3*ww/40, wh-ww/10+ww/40);
  glEnd();

  glPointSize(3.0);

  glBegin(GL_POINTS);
  glVertex2i(3*ww/10+ww/20, wh-ww/20);
  glEnd();

  glRasterPos2i(2*ww/5+wh/40, wh-ww/20);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
  shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
  glRasterPos2i(2*ww/5+wh/40+shift, wh-ww/20);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
  shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
  glRasterPos2i(2*ww/5+wh/40+shift, wh-ww/20);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
  glFlush();
  glPopAttrib();
}

void init()
{
 /*  int i; */
/*   base=glGenLists(128); */
/*   for(i=0;i<128;i++) { */
/*     glNewList(base+i,GL_COMPILE); */
/*     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,i); */
/*     glEndList(); */
/*   } */
/*   glListBase(base); */
  glViewport(0,0, ww, wh);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0,(GLdouble)ww,0.0,(GLdouble)wh, -1.0, 1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void key(unsigned char k, int xx, int yy)
{
  if (draw_mode != TEXT) return;
  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2i(rx, ry);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15,k);
/*   glCallList(k); */
  rx+=glutBitmapWidth(GLUT_BITMAP_9_BY_15,k);
}

int main(int argc, char **argv)
{
  int c_menu, p_menu, f_menu;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(wh, ww);
  glutCreateWindow("paint");
  glutDisplayFunc(display);
  c_menu=glutCreateMenu(color_menu);
  glutAddMenuEntry("Red", 1);
  glutAddMenuEntry("Green", 2);
  glutAddMenuEntry("Blue", 3);
  glutAddMenuEntry("Cyan", 4);
  glutAddMenuEntry("Magenta", 5);
  glutAddMenuEntry("Yellow", 6);
  glutAddMenuEntry("White", 7);
  glutAddMenuEntry("Black", 8);
  p_menu=glutCreateMenu(pixel_menu);
  glutAddMenuEntry("increase pixel size", 1);
  glutAddMenuEntry("decrease pixel size", 2);
  f_menu=glutCreateMenu(fill_menu);
  glutAddMenuEntry("fill on", 1);
  glutAddMenuEntry("fill off", 2);
  glutCreateMenu(right_menu);
  glutAddMenuEntry("quit", 1);
  glutAddMenuEntry("clear", 2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutCreateMenu(middle_menu);
  glutAddSubMenu("Colors", c_menu);
  glutAddSubMenu("PixelSize", p_menu);
  glutAddSubMenu("Fill", f_menu);
  glutAttachMenu(GLUT_MIDDLE_BUTTON);
  init();
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(key);
  glutMainLoop();

  return 0;
}
