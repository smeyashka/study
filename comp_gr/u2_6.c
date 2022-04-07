#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

typedef struct {
  GLfloat x;
  GLfloat y;
} point2;

int k;

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
/*   glColor3f(0.0, 1.0, 1.0); */
/*   glMatrixMode(GL_PROJECTION); */
/*   glLoadIdentity(); */
/*   //  gluOrtho2D(0.0, 100.0, 0.0, 100.0); */
/*   gluOrtho2D(-5.0, 80.0, -15.0, 70.0); */
/*   glMatrixMode(GL_MODELVIEW); */
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-5.0, 80.0, -15.0, 70.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void triangl1(point2 ver[3])
{
  glBegin(GL_LINE_STRIP);
  glVertex2f(ver[2].x, ver[2].y);
  glVertex2f(ver[0].x, ver[0].y);
  glVertex2f(ver[1].x, ver[1].y);
  glVertex2f(ver[2].x, ver[2].y);
  glEnd();
}

void triangl(point2 v1,point2 v2,point2 ver[3])
{
  glBegin(GL_LINE_STRIP);
  glVertex2f(v1.x, v1.y);
  glVertex2f(ver[0].x, ver[0].y);
  glVertex2f(ver[2].x, ver[2].y);
  glVertex2f(ver[1].x, ver[1].y);
  glVertex2f(v2.x, v2.y);
  glEnd();
}

point2 vertice(point2 v1,point2 v2) //возвр. вершину на пересечении 2-х линий, прох. под наклоном 60 градусов ч/з точки v1 и v1
{
  float slope;
  float k1, k2, b1, b2;
  point2 ver;

  slope = atan((v2.y-v1.y)/(v2.x-v1.x));

  k1 = slope + 60*M_PI/180;
  if (k1 > M_PI_2) {
    k1 = k1-M_PI;
  }
  b1 = v1.y - tan(k1)*v1.x;

  k2 = slope - 60*M_PI/180;
  if (k2 > M_PI_2) {
    k2 = -k2 + M_PI;
  }
  b2 = v2.y - tan(k2)*v2.x;

  ver.x = (b2-b1) / (tan(k1)-tan(k2));
  ver.y = tan(k1) * ver.x + b1;

  return ver;
}

void new_points(point2 v1, point2 v2, point2 v_new[3], int k) //правило замены прямой на кривую
{
  point2 v_tmp;
  /*float ugol1, ugol2, dlina; */
  point2 v_n[3];

  if (k > 0) {
    v_tmp.x = (v2.x - v1.x) / 3;
    v_tmp.y = (v2.y - v1.y) / 3;
    v_new[0].x = v1.x + v_tmp.x;
    v_new[0].y = v1.y + v_tmp.y;
    v_new[1].x = v2.x - v_tmp.x;
    v_new[1].y = v2.y - v_tmp.y;
    
    /* dlina = sqrt((v_new[0].x - v_new[1].x) * (v_new[0].x - v_new[1].x) + */
    /* 		 (v_new[0].y - v_new[1].y)*(v_new[0].y - v_new[1].y)); */

    v_new[2] = vertice(v_new[0], v_new[1]);
    
    if (k == 1) {
      new_points(v1, v2, v_new, k - 1);
    } else { 
      new_points(v1, v_new[0], v_n, k-1);
      new_points(v_new[0], v_new[2], v_n, k-1);
      new_points(v_new[2], v_new[1], v_n, k-1);
      new_points(v_new[1], v2, v_n, k-1);
    }   
  } else {
    triangl(v1, v2, v_new);
  }
}

void display()
{
  //  float ugol1, ugol2;

  point2 v[3] = {{10, 10}, {70, 10}, {40, 60}};
  point2 v1[3] = {{0,0}, {0,0}, {0,0}};

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 1.0, 1.0);
  if (k >= 1) {
    new_points(v[0], v[2], v1, k);
    new_points(v[2], v[1], v1, k);
    new_points(v[1], v[0], v1, k);
  } else {
    triangl1(v);
  }
  glFlush();
}



int main(int argc, char **argv)
{
  if (argc == 1) {
    k=3;
  } else {
    k=atoi(argv[1]);
  }
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  //  glutInitWindowSize(800, 800);
  glutInitWindowSize(400, 400);
  glutCreateWindow("Koch snowflake");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();

  return 0;
}
