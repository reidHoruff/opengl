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
#include <stdlib.h>
#include <time.h>

float height = -3.0;
int seed = 0;

float sv = 1.0;
float vx = 3.0;
float vy = 3.0;
float fric = 0.95;

float angle1 = 34.0f;
float angle2 = 34.0f;
float color_angle = 0.0;
int startx = 0;
int starty = 0;
int mouse_down = 0;

unsigned prob(float p) {
  return ((float)rand() / RAND_MAX) < p;
}

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

static float face_colors[] = {
  0.8, 0.4, 0.2,
  0.2, 7.0, 0.4,
  0.2, 0.3, 1.0,
  0.2, 0.2, 0.3,
  0.1, 0.9, 0.9,
  0.3, 0.9, 0.2
};

void draw_square() {
}


void recur_cube(GLfloat x, GLfloat y, GLfloat z, float s, int depth) {
  int die = 0;
  if (depth > 2 && prob(0.03)) {
    die = 1;
  }

  if (depth > 4 || die) {
    int i;
    int face_color_index = 0;
    for (i=0; i<72; i+=12) {
      glColor3f(
          face_colors[face_color_index+0],
          face_colors[face_color_index+1],
          face_colors[face_color_index+2]
        );
      face_color_index += 3;
      face_color_index %= sizeof(face_colors) / sizeof(float);
      /*
      color_angle += 0.0001;
      glColor3f(
          fabs(sin(color_angle + 10)),
          fabs(sin(color_angle + 20)),
          fabs(sin(color_angle + 30))
      );
      */
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
  srand(seed);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen and Depth Buffer
  glLoadIdentity();

  float c = 0.5;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0, 0, height);
    glRotatef(angle1, 0.0, 1.0, 0.0);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, -0.5);

    glBegin(GL_TRIANGLES);
    recur_cube(0, 0, 0, 1.0, 1);
    glEnd();
  glPopMatrix();

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
  /*
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT_MODEL_AMBIENT );
  glEnable( GL_LIGHT0 );
  GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
  */
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
      mouse_down = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      mouse_down = 0;
    }
  }

  else if ((button == 3) || (button == 4)) {
    if (state == GLUT_UP) return;

    //up
    const float scroll_factor = 0.3;
    if (button == 3) {
      height += scroll_factor;
    }
    //down
    else if (button == 4) {
      height -= scroll_factor;
    }
  }
}

static void motion(int x, int y) {
  if (mouse_down) {
    float dx = x-startx;
    float dy = y-starty;

    vx += dx * 0.01;
    vy += dy * 0.01;

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
  angle1 += vx;
  angle2 += vy;

  vx *= fric;
  vy *= fric;
  //inc_color(0.01);
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  seed = time(NULL);

  win.width = 1920;
  win.height = 1080;
  win.title = "OpenGL/GLUT Example. Visit http://openglsamples.sf.net ";
  win.field_of_view_angle = 25;
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
