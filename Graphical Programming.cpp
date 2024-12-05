#include <gl\glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "fileFunctions.h"

using namespace std;

//floats for the body sizes
const float BODY_LENGTH = 2.0;
const float LEG_LENGTH = 0.5;
const float HEAD_HEIGHT = 0.6;
const float TAIL_LENGTH = 0.6;

float angle[3] = { 0.0, 0.0, 0.0 }; 

float xMove = 0; //used for moving along the x axis
float zMove = 0; //used for moving along the z axis


//corners for the cube to be made
GLfloat corners[][3] = { {-0.5,-0.5,0.5}, {-0.5,0.5,0.5}, 
                         {0.5,0.5,0.5}, {0.5,-0.5,0.5},
                       {-0.5,-0.5,-0.5}, {-0.5,0.5,-0.5}, 
                        {0.5,0.5,-0.5}, {0.5,-0.5,-0.5} };

//normals for mapping textures
GLfloat normals[][3] = { {0.0,0.0,1.0},
                                      {1.0,0.0,0.0},
                                      {0.0,-1.0,0.0},
                                      {0.0,1.0,0.0},
                                      {0.0,0.0,-1.0},
                                      {-1.0,0.0,0.0} };

// called by materials to assign values to the floats
typedef struct materialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};

//material for the dog
materialStruct brownMaterial = {
    { 0.33, 0.17, 0.12, 1.00 },
    { 0.60, 0.30, 0.15, 1.00 },
    { 0.40, 0.30, 0.20, 1.00 },
    0.00
};


materialStruct* currentMaterial;

//bools for animations on the dog moving
bool rightLegsForward;
bool leftLegsForward;
//bools for the dog moving
bool isWalking = false;
bool rotateRight = false;
bool rotateLeft = false;
bool isWalkingBack = false;

unsigned int furTexture; //fur texture will be assigned in display


//drawing a square face
void drawFace(int a, int b, int c, int d) {
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); //used for to map textures to coordinates. 1 for each angle.
    glVertex3fv(corners[a]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(corners[b]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(corners[c]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(corners[d]);
    glEnd();
}
//insted of having a long cube code, this makes it so it draws from draw face's corners and makes it a lot neater
void cube() {
    glNormal3fv(normals[0]);
    drawFace(0, 3, 2, 1);
    glNormal3fv(normals[1]);
    drawFace(2, 3, 7, 6);
    glNormal3fv(normals[2]);
    drawFace(0, 4, 7, 3);
    glNormal3fv(normals[3]);
    drawFace(1, 2, 6, 5);
    glNormal3fv(normals[4]);
    drawFace(4, 5, 6, 7);
    glNormal3fv(normals[5]);
    drawFace(0, 1, 5, 4);
}
// body
void drawTorso() {
    glPushMatrix();         //starts unique measurements. doesnt carry on from here.
    glEnable(GL_TEXTURE_2D);         //applies 2D texture
    glScalef(BODY_LENGTH, 0.5, 0.5);    //scales the body.
    cube();        //draws the cube with the scale measurements.
    glDisable(GL_TEXTURE_2D); //Stops applying 2D texture. So other models don't have the texture.
    glPopMatrix();                //ends the unique measurements.
}
//head
void drawHead() {
    glPushMatrix();
    glTranslatef(0.0, (HEAD_HEIGHT * 0.5), 0.0);
    glScalef(0.3, HEAD_HEIGHT, 0.7);
    cube();
    glPopMatrix();
}
//ears
void drawEars() {
    glPushMatrix();
    glTranslatef(0.0, (HEAD_HEIGHT), 0.0);
    glRotatef(15, 1.0, 0.0, 0.0);
    glScalef(0.3, 0.8, 0.05);
    cube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, HEAD_HEIGHT, 1);
    glRotatef(-15, 1.0, 0.0, 0.0);
    glScalef(0.3, 0.8, 0.05);
    cube();
    glPopMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~

//leg models

//Front Left Leg
void drawLeftLegFront() {
    glPushMatrix();
    glScalef(0.1, LEG_LENGTH, 0.1);
    glTranslatef(0.0, 0.5, 0.0);
    cube();
    glPopMatrix();
}
//Back Left Leg
void drawLeftLegBack() {
    glPushMatrix();
    glScalef(0.1, LEG_LENGTH, 0.1);
    glTranslatef(0.0, 0.5, 0.0);
    cube();
    glPopMatrix();
}
//Front right Leg
void drawRightLegFront() {
    glPushMatrix();
    glScalef(0.1, LEG_LENGTH, 0.1);
    glTranslatef(0.0, 0.5, 0.1);
    cube();
    glPopMatrix();
}
//Back Right Leg
void drawRightLegBack() {
    glPushMatrix();
    glScalef(0.1, LEG_LENGTH, 0.1);
    glTranslatef(0.0, 0.5, 0.1);
    cube();
    glPopMatrix();
}

//tail
void drawTail() {
    glPushMatrix();
    glRotatef(-45, 0.0, 0.0, 1.0);
    glTranslatef(0.0, (TAIL_LENGTH * 0.5), 0.0);
    glScalef(0.1, TAIL_LENGTH, 0.1);
    cube();
    glPopMatrix();
}
//drawing the dog from previous functions
void drawDog() {
    glPushMatrix();
    glTranslatef(xMove, 0.0, zMove); // used for moving the dog on button presses
    glRotatef(angle[0], 0.0, 1.0, 0.0); //used for rotating the dog on button presses
    drawTorso();  //torso is a different texture
    //set the material for the rest of the dog
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, brownMaterial.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brownMaterial.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, brownMaterial.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brownMaterial.shininess);

    glTranslatef(1.1, (HEAD_HEIGHT * -0.1), 0.0);
    drawHead();
    glTranslatef(0.0, -0.4, -0.5);
    drawEars();
    glTranslatef(-2.5, 0.2, 0.5);
    drawTail();
    glTranslatef(0.4, 0.0, -0.2);
    glPushMatrix();
    glRotatef(angle[2], 0.0, 0.0, 1.0); //used to rotate the rear left leg
    drawLeftLegBack();
    glPopMatrix();
    glTranslatef(BODY_LENGTH, 0.0, 0.0);
    glPushMatrix();
    glRotatef(angle[2], 0.0, 0.0, 1.0); // rotates front left leg
    drawLeftLegFront();
    glPopMatrix();
    glTranslatef(0.0, 0.05, 0.4);
    glPushMatrix();
    glRotatef(angle[1], 0.0, 0.0, 1.0); //rotates the front right leg
    drawRightLegFront();
    glPopMatrix();
    glTranslatef(-BODY_LENGTH, 0.0, 0.0);
    glPushMatrix();
    glRotatef(angle[1], 0.0, 0.0, 1.0); //rotates the rear right leg.
    drawRightLegBack();
    glPopMatrix();
    glPopMatrix();
}

//adds the face for the floor
void floor() {
        glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 1);
        glVertex3f(1, -1, 1);
        glVertex3f(1, -1, -1);
        glVertex3f(-1, -1, -1);
        glEnd();

}
//scales and draws the floor
void drawFloor() {
    glPushMatrix();
    glScalef(10.0, 1.0, 10.0);

    glEnable(GL_COLOR_MATERIAL);  // Enable color tracking for materials
    // assigns a material to the floor. 
    GLfloat floor_ambient[] = { 0.24725, 0.1995, 0.0745, 1.0 };
    GLfloat floor_diffuse[] = { 0.75164, 0.60648, 0.22648, 1.0 };
    GLfloat floor_specular[] = { 0.628281, 0.555802, 0.366065, 1.0 };
    GLfloat floor_shininess = 0.5;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floor_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floor_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floor_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, floor_shininess);


    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    floor();

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}


// animation used to rotate the body. will rotate until it reaches a certain value.
void rotate() {
    if (angle[0] == 360.0) {
        angle[0] -= 360.0;
    }
    if (angle[0] > 360.0) {
        angle[0] == 0.0;
    }
    if (rotateRight) {
        if (angle[0] != 360.0 && angle[0] != 0.0) {
            angle[0] -= 0.5;
        }
        
    }
    if (rotateLeft) {
        if (angle[0] != 180.0) {
            angle[0] += 0.5;
        }

    }
    if (isWalking) {
        if (angle[0] != 90.0) {
            angle[0] += 0.5;
        }

    }
    if (isWalkingBack) {
        if (angle[0] != 270.0) {
            angle[0] += 0.5;
        }

    }
    glutPostRedisplay();
}

//animation used to portray the dog walking.
void walking() {
    //left legs
    if (angle[2] < 100.0) { leftLegsForward = false; }
    if (angle[2] > 250.0) { leftLegsForward = true; }

    if (leftLegsForward == true) { //will check to see if the legs are forward and if they are then they will rotate back
        angle[2] -= 0.4;
    }
    else {
        angle[2] += 0.4;
    }
    //right legs
    if (angle[1] < 100.0) { rightLegsForward = false; }
    if (angle[1] > 250.0) { rightLegsForward = true; }

    if (rightLegsForward == true) {
        angle[1] -= 0.4;
    }
    else {
        angle[1] += 0.4;
    }


    glutPostRedisplay();
}
//for when a key is pressed
void keyPressed(unsigned char key, int x, int y) {
    if (key == 'w') {
        isWalking = true;
        zMove -=0.1; //moves along the z axis in a negative direction
        //--zMove;
    }
    if (key == 'd') {
        rotateRight = true;
        xMove +=0.1; //moves along the x axis in a positve direction
        //xMove++;
    }
    if (key == 'a') {
        rotateLeft = true;
        xMove -=0.1; //moves along the x axis in a negative direction
        //--xMove;
    }
    if (key == 's') {
        isWalkingBack = true;
        zMove +=0.1; //moves along the z axis in a positive direction
        //zMove++;
    }
}

//for when the key is lifted up after being pressed (not pressed anymore)
void keyUp(unsigned char key, int x, int y){
    if (key == 'w') {
        isWalking = false;
    }
    if (key == 'd') {
        rotateRight = false;
    }
    if (key == 'a') {
        rotateLeft = false;
    }
    if (key == 's') {
        isWalkingBack = false;
    }
}

//idle function used at all times
void idle() {
    if (isWalking) {
        rotate(); //calls rotate
        walking(); //calls walking
        drawDog(); //redraws the dog for movingg
        glTranslatef(xMove, 0.0, zMove); //moves the dog
       
        
        
    }
    if (rotateRight) {
        rotate();
        walking();
    }
    if (rotateLeft) {
        rotate();
        walking();
    }
    if (isWalkingBack) {
        rotate();
        walking();
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    //Lighting setup
    GLfloat light_pos[] = { 0.0, 10.0, 0.0, 1.0 }; //postion values
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 }; //ambient values
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    GLfloat specular[] = { 0.3, 0.3, 0.3, 1.0 }; //specular values
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    GLfloat diffuse[] = { 0.3, 0.3, 0.3, 1.0 }; //diffues values
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    GLfloat direction[] = { 0.0, -1.0, 0.0 }; //directional values
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10); //spotlight cutoff

    glPopMatrix();

    drawDog();

    drawFloor();
    
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING); //enables lighting
    glEnable(GL_LIGHT0); //enables 1 light
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &furTexture); 
    glBindTexture(GL_TEXTURE_2D, furTexture); //assigns texture to int

    //loading a bitmap image
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char* bitmapData;
    bitmapData = LoadBitmapFile("leather5.bmp", &bitmapInfoHeader);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        bitmapInfoHeader.biWidth,
        bitmapInfoHeader.biHeight,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        bitmapData);
    free(bitmapData);
    glDisable(GL_TEXTURE_2D); //disables 2d texture. ready to be enabled again and paste the texture.

    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -0.5, 1.2, 0.8, 10.0);
    //glOrtho(-4.0, 4.0, -1.0, 4.0, 0.0, 10.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    angle[0] = 90; //establishing variables
    angle[1] = 250;
    angle[2] = 100;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1080, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Walking Dog");
    glutDisplayFunc(display);
    glutIdleFunc(idle); //sets the idle fuction
    init();
    glutKeyboardFunc(keyPressed); //keyboard down function
    glutKeyboardUpFunc(keyUp); //keyboard up function
    glutMainLoop();
}