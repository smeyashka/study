#include <GL/glut.h>
#include <stdlib.h>

static GLushort frontIndices[] = {4, 5, 6, 7};
static GLushort rightIndices[] = {1, 2, 6, 5};
static GLushort bottomIndices[] = {0, 1, 5, 4};
static GLushort backIndices[] = {0, 3, 2, 1};
static GLushort leftIndices[] = {0, 4, 7, 3};
static GLushort topIndices[]  = {2, 3, 7, 6};
static GLushort allIndices[]  = {4, 5, 6, 7, 1, 2, 6, 5, 0, 1, 5, 4, 0, 3, 2, 1, 0, 4, 7, 3, 2, 3, 7, 6}; 

/* static GLshort vertices[]={1, 1, 0, */
/* 			  5, 1, 0, */
/* 			  5, 5, 0, */
/* 			  1, 5, 0, */

/* 			  3, 3, 2, */
/* 			  7, 3, 2, */
/* 			  7, 10, 2, */
/* 			  3, 10, 2}; */

/* static GLshort vertices[]={1, 3, 2, */
/* 			  4, 5, 5, */
/* 			  4, 14, 5, */
/* 			  1, 12, 2, */

/* 			  5, 1, -1, */
/* 			  8, 3, 2, */
/* 			  8, 12, 2, */
/* 			  5, 10, -1 }; */

static GLshort vertices[]={1, 3, 4, 
			  4, 3, 4, 
			  4, 10, 4, 
			  1, 10, 4, 

			  1, 3, -1, 
			  4, 3, -1, 
			  4, 10, -1, 
			  1, 10, -1 };
void display()
{
 glLoadIdentity();
 gluLookAt(2.0, -1.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

 glClear(GL_COLOR_BUFFER_BIT);

 glPolygonMode(GL_BACK, GL_LINE);
 glPolygonMode(GL_FRONT, GL_FILL);

 // glEnableClientState(GL_COLOR_ARRAY);
 glEnableClientState(GL_VERTEX_ARRAY);
 // glColorPointer(3, GL_FLOAT, 0, colors);
 glVertexPointer(3, GL_SHORT, 0, vertices);

 glColor3f(1.0, 0.0, 0.0);
 glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, frontIndices);
 glColor3f(0.0, 0.0, 1.0);
 glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, leftIndices);
 glColor3f(0.5, 0.0, 0.5);
 glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, topIndices);
 glColor3f(0.0, 0.1, 0.0);
 glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, bottomIndices);
 glColor3f(0.0, 0.5, 0.5);
 glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, backIndices);
 glColor3f(0.5, 0.5, 0.0);
 glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, rightIndices);

// glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, allIndices);

/*  short i; */
/*  glBegin(GL_POLYGON); */
/*  for (i=4;i<8;i++) */
/*   glArrayElement(i); */
/*  glEnd(); */

 glFlush();
}

/* void mouse(int button, int state, int x, int y) */
/* { */
/*  switch(button) { */
/*  case GLUT_LEFT_BUTTON: */
/*   if (state==GLUT_DOWN) */

/*  } */
/* } */

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
 glOrtho(-5.0, 10.0, -3.0, 15.0, -10.0, 10.0);
 // gluOrtho2D(0.0, 15.0, 0.0, 15.0);
 glMatrixMode(GL_MODELVIEW);
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
 // glutMouseFunc(mouse);
 glutMainLoop();
 return 0;
}
