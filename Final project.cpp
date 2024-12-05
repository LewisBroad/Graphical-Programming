#include <gl\glut.h>

const float BODY_LENGTH = 2.0;
const float LEG_LENGTH = 0.5;
const float HEAD_HEIGHT = 0.6;
const float TAIL_LENGTH = 0.6;
float angle[3] = { 0.0, 0.0, 0.0 };

using namespace std;

void cube() { //Function for making a solid cube
    //Far face
    glColor3f(0.33, 0.17, 0.12);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glEnd();
    //Left Face
    glColor3f(0.33, 0.17, 0.12);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();
    //Bottom Face
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
    //Right Face
    glColor3f(0.33, 0.17, 0.12);
    glBegin(GL_POLYGON);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glEnd();
    //Front Face
    glColor3f(0.33, 0.17, 0.12);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();
    //Top Face
    glColor3f(0.33, 0.17, 0.12);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glEnd();
}
void drawTorso() {
    glPushMatrix();                                     //starts unique measurements. doesnt carry on from here.
    glScalef(BODY_LENGTH, 0.5, 0.5);                        //scales the body.
    cube();                                             //draws the cube with the scale measurements.
    //glutWireCube(1.0);
    glPopMatrix();                                      //ends the unique measurements.
}

void drawHead() {
    glPushMatrix();
    glTranslatef(0.0, (HEAD_HEIGHT * 0.5), 0.0);
    glScalef(0.3, HEAD_HEIGHT, 0.7);
    cube();
    //glutWireCube(1.0);
    glPopMatrix();
}

void drawEars() {
    glPushMatrix();
    glTranslatef(0.0, (HEAD_HEIGHT), 0.0);
    glRotatef(15, 1.0, 0.0, 0.0);
    glScalef(0.3, 0.8, 0.05);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, HEAD_HEIGHT, 1);
    glRotatef(-15, 1.0, 0.0, 0.0);
    glScalef(0.3, 0.8, 0.05);
    glutWireCube(1.0);
    glPopMatrix();
}

void drawLegs() {
    glPushMatrix();
    glScalef(0.1, LEG_LENGTH, 0.1);
    glutWireCube(1.0);
    glTranslatef((BODY_LENGTH * 10 ), 0.0, 0.0);
    glutWireCube(1.0);
    glTranslatef(0.0, 0.0, 4);
    glutWireCube(1.0);
    glTranslatef((-BODY_LENGTH * 10), 0.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
}

void drawTail() {
    glPushMatrix();
    glRotatef(-45, 0.0, 0.0, 1.0);
    glTranslatef(0.0, (TAIL_LENGTH * 0.5), 0.0);
    glScalef(0.1, TAIL_LENGTH, 0.1);
    glutWireCube(1.0);
    glPopMatrix();
}

void drawDog() {
    glRotatef(angle[0], 0.0, 1.0, 0.0);
    drawTorso();
    glTranslatef(1.1, (HEAD_HEIGHT * -0.1), 0.0);
    drawHead();
    glTranslatef(0.0, -0.4, -0.5);
    drawEars();
    glTranslatef(-2.5, 0.2, 0.5);
    drawTail();
    glTranslatef(0.4, -0.2, -0.2);
    drawLegs();
}

void rotate() {
    angle[0] += 0.03;
    if (angle[0] > 360.0) { angle[1] -= 360.0; }
    glutPostRedisplay();
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);
    drawDog();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -0.5, 1.2, 0.8, 10.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    angle[0];
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1080, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Transformations");
    glutDisplayFunc(display);
    glutIdleFunc(rotate);
    init();
    glutMainLoop();
}