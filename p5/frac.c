/*
 * OpenGLSamples (openglsamples.sf.net) Examples
 * VC++ users should create a Win32 Console project and link
 * the program with glut32.lib, glu32.lib, opengl32.lib
 *
 * GLUT can be downloaded from http://www.xmission.com/~nate/glut.html
 * OpenGL is by default installed on your system.
 * For an installation of glut on windows for MS Visual Studio 2010 see: http://nafsadh.wordpress.com/2010/08/20/glut-in-ms-visual-studio-2010-msvs10/
 *
 */

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

float height = -3.0;
float angle1 = 34.0f;
float angle2 = 34.0f;
float color_angle = 0.0;
int startx = 0;
int starty = 0;
int moving = 0;

typedef struct {
  int width;
  int height;
  char* title;

  float field_of_view_angle;
  float z_near;
  float z_far;
} glutWindow;

glutWindow win;

static const GLfloat cubev[] = {
  0, 0, 0,
  1, 0, 0,
  1, 0, 1,
  0, 0, 1,

  1, 0, 0,
  1, 1, 0,
  1, 1, 1,
  1, 0, 1,

  1, 1, 0,
  0, 1, 0,
  0, 1, 1,
  1, 1, 1,

  0, 1, 0,
  0, 0, 0,
  0, 0, 1,
  0, 1, 1,

  0, 0, 1,
  1, 0, 1,
  1, 1, 1,
  0, 1, 1,

  0, 0, 0,
  0, 1, 0,
  1, 1, 0,
  1, 0, 0
};

void draw_square() {
}

void recur_cube(GLfloat x, GLfloat y, GLfloat z, float s, int depth) {
  if (depth > 3) {
    int i;
    for (i=0; i<72; i+=12) {
      color_angle += 0.001;
      glColor3f(
          fabs(sin(color_angle + 10)),
          fabs(sin(color_angle + 20)),
          fabs(sin(color_angle + 30))
      );
      glVertex3f(cubev[i+0]*s+x, cubev[i+1]*s+y, cubev[i+2]*s+z);
      glVertex3f(cubev[i+3]*s+x, cubev[i+4]*s+y, cubev[i+5]*s+z);
      glVertex3f(cubev[i+6]*s+x, cubev[i+7]*s+y, cubev[i+8]*s+z);

      glVertex3f(cubev[i+6]*s+x, cubev[i+7]*s+y, cubev[i+8]*s+z);
      glVertex3f(cubev[i+0]*s+x, cubev[i+1]*s+y, cubev[i+2]*s+z);
      glVertex3f(cubev[i+9]*s+x, cubev[i+10]*s+y, cubev[i+11]*s+z);
    }

  }
  else {
    float ns = s/3.0;
    int i, j, k;
    for (i=0; i<3; i++) {
      for (j=0; j<3; j++) {
        for (k=0; k<3; k++) {
          int ones = 0;
          if (i==1) ones++;
          if (j==1) ones++;
          if (k==1) ones++;
          //no centers
          if (ones < 2) {
            recur_cube(x+ns*i, y+ns*j, z+ns*k, ns, depth+1);
          }
        }
      }
    }
  }
}

void display() {
  printf("display()\n");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen and Depth Buffer
  glLoadIdentity();

  glTranslatef(0.0f,0.0f,height);

  glRotatef(angle1, 0.0, 1.0, 0.0);
  glRotatef(angle2, 1.0, 0.0, 0.0);

  /*
   * Triangle code starts here
   * 3 verteces, 3 colors.
   */

  glBegin(GL_TRIANGLES);
  recur_cube(0, 0, 0, 1.0, 0);
  glEnd();

  glutSwapBuffers();
}

void initialize () {
  glMatrixMode(GL_PROJECTION);  // select projection matrix
  glViewport(0, 0, win.width, win.height); // set the viewport
  glMatrixMode(GL_PROJECTION); // set matrix mode
  glLoadIdentity();	// reset projection matrix
  GLfloat aspect = (GLfloat) win.width / win.height;
  // set up a perspective projection matrix
  gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
  glMatrixMode(GL_MODELVIEW); // specify which matrix is the current matrix
  glShadeModel( GL_SMOOTH );
  glClearDepth( 1.0f ); // specify the clear value for the depth buffer
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); // specify implementation-specific hints
  glClearColor(0.0, 0.0, 0.0, 1.0); // specify clear values for the color buffers
}

void keyboard ( unsigned char key, int mousePositionX, int mousePositionY ) {
  switch ( key ) {
    case 107:
      height -= 0.5;
      break;
    case 106:
      height += 0.5;
      break;
    default:
      break;
  }
  display();
}

static void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}

static void motion(int x, int y) {
  if (moving) {
    angle1 = angle1 + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}

void inc_color(float delta) {
  color_angle += delta;
  if (color_angle > 360) {
    color_angle = 0.0;
  }
}

void idle() {
  inc_color(0.01);
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  win.width = 640;
  win.height = 480;
  win.title = "OpenGL/GLUT Example. Visit http://openglsamples.sf.net ";
  win.field_of_view_angle = 45;
  win.z_near = 1.0f;
  win.z_far = 500.0f;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize(win.width,win.height);
  glutCreateWindow(win.title);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  initialize();
  glutMainLoop();
  return 0;
}
