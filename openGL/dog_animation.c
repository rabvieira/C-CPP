#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>

int shoulderAngle = 0, elbowAngle = 0;
int animation=0;
int rotate_inc=0;
int scale=0;
float scale_inc=0.0;
float posx=0.0, posy=0.0;
float rotate_x=0.0, rotate_y=0.0, rotate_z=0.0;
int dice=0;

void Keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        //case '1': (elbowAngle += 5); elbowAngle = elbowAngle%30; break;
        //case '2': (elbowAngle -= 5); elbowAngle = elbowAngle%30; break;
        //case '9': (shoulderAngle += 5); shoulderAngle = shoulderAngle%15; break;
        //case '0': (shoulderAngle -= 5); shoulderAngle = shoulderAngle%15; break;

        case 'r':
        case 'R':
            animation=1;        //glutPostRedisplay();
        break;
        case 'p':
        case 'P':
            animation=0;
        break;
        case 'e': scale=-1;     //glutPostRedisplay();
        break;
        case 'E': scale=1;      //glutPostRedisplay();
        break;
        case 'x':
        case 'X':
            rotate_x=1.0;  rotate_y=0.0;  rotate_z=0.0; rotate_inc+=10; rotate_inc = rotate_inc%360;     glutPostRedisplay();
        break;
        case 'y':
        case 'Y':
            rotate_x=0.0;  rotate_y=1.0;  rotate_z=0.0; rotate_inc+=10; rotate_inc = rotate_inc%360;     glutPostRedisplay();
        break;
        case 'z':
        case 'Z':
            rotate_x=0.0;  rotate_y=0.0;  rotate_z=1.0; rotate_inc+=10; rotate_inc = rotate_inc%360;     glutPostRedisplay();
        break;
    }
    //glutPostRedisplay();
    printf("elbowAngle=%d\nshoulderAngle=%d\n\n",elbowAngle, shoulderAngle);
}

void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if(scale>0)
                scale_inc = scale_inc + 0.1;
            if(scale<0)
                scale_inc = scale_inc - 0.1;
            if(scale_inc>=0.2)
                scale_inc = scale_inc - 0.1;

            else if(scale_inc<=-0.8)
                scale_inc = scale_inc + 0.1;
            glutPostRedisplay();
        break;
    }
    //glutPostRedisplay();
}

void wireBox(GLdouble width, GLdouble height, GLdouble depth)
{
    glPushMatrix();
        glScalef(width, height, depth);
        glutWireSphere(0.25,20,20);
        //glutSolidSphere(0.25,20,20);
    glPopMatrix();
}

void legs(float pos_x, float pos_z)
{
    if(pos_z<0)
    {
        shoulderAngle = shoulderAngle*(-1);
        elbowAngle = elbowAngle*(-1);
    }

    glRotatef((GLfloat)shoulderAngle, 0.0, 0.0, 1.0);
    glTranslatef(pos_x, -0.25, pos_z);
    wireBox(0.35, 1.0, 0.25); //upper leg

    glTranslatef(0.0, -0.125, 0.0);
    glRotatef((GLfloat)elbowAngle, 0.0, 0.0, 1.0);

    wireBox(0.30, 0.5, 0.25); //bottom leg

    //if(pos_x>=0)
        glTranslatef(-0.075, -0.05, 0.0);
    //else
    //    glTranslatef(-0.075, -0.05, 0.0);
    wireBox(0.3, 0.1, 0.25); //feet
}

int detect_border(int dice)
{
    printf("posx=%f\n", posx);

    switch(dice)
    {
        case 0:
            if(posx <=-0.75) return 1;
        break;
        case 1:
            if(posx >=0.75) return 1;
        break;
        case 2:
            if(posy <=-0.75) return 1;
        break;
        case 3:
            if(posy >=0.75) return 1;
        break;
    }
    return 0;
}

void uptade_moviments(int dice)
{
    (elbowAngle += 5); elbowAngle = elbowAngle%30;
    (shoulderAngle += 5); shoulderAngle = shoulderAngle%15;

    switch(dice)
    {
        case 0: //left
            posx = posx - 0.01; posy = 0;
        break;
        case 1: //right
            posx = posx + 0.01; posy = 0;
        break;
        case 2: //down
            posx = 0; posy = posy - 0.01;
        break;
        case 3: //up
            posx = 0; posy = posy + 0.01;
        break;
    }
    //glutPostRedisplay();
}

int set_direction(int old_dice)
{
    //int dice=0;
    while(1)
    {
        dice = rand()%4;
        if(dice!=old_dice)
            break;
    }
    posx=0;
    posy=0;
    return dice;
}

void display(void)
{
    //int dice=rand()%4;
    int border=0;
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    if(!animation)
        glRotatef((GLfloat)rotate_inc, rotate_x, rotate_y, rotate_z);

    border = detect_border(dice);
    printf("\ndice%d\n",dice);
    if(border==0)
        uptade_moviments(dice);
    else
        set_direction(dice);

    glPushMatrix();
        if(scale>0)
            glScalef(1.0+scale_inc, 1.0+scale_inc, 1.0+scale_inc);
        if(scale<0)
            glScalef(1.0+scale_inc, 1.0+scale_inc, 1.0+scale_inc);
        glTranslatef(posx, posy, 0.0);

        glPushMatrix();
            //right eye
            glPushMatrix();
                glTranslatef(-0.83, 0.55, 0.1);
                glColor3f(0.0, 1.0 ,0.0);
                wireBox(0.2, 0.2, 0.1);
            glPopMatrix();
            //left eye
            glPushMatrix();
                glTranslatef(-0.83, 0.55, -0.1);
                glColor3f(0.0, 1.0 ,0.0);
                wireBox(0.2, 0.2, 0.1);
            glPopMatrix();
            //tongue
            glPushMatrix();
                glTranslatef(-0.81, 0.25, 0.00);
                glColor3f(1.0, 0.0 ,0.0);
                wireBox(0.1, 0.5, 0.1);
            glPopMatrix();
            //nose
            glPushMatrix();
                glTranslatef(-0.85, 0.45, 0.0);
                glColor3f(0.0, 1.0 ,1.0);
            wireBox(0.1, 0.1, 0.1);
            glPopMatrix();
            //tail
            glPushMatrix();
                glTranslatef(0.65, 0.15, 0.0);
                glColor3f(0.0, 1.0 ,1.0);
            wireBox(0.6, 0.1, 0.1);
            glPopMatrix();
            //right ear
            glPushMatrix();
                glTranslatef(-0.55, 0.35, 0.25);
                glColor3f(0.0, 0.0 ,1.0);
                wireBox(0.2, 0.9, 0.2);
            glPopMatrix();
            //left ear
            glPushMatrix();
                glTranslatef(-0.55, 0.35, -0.25);
                glColor3f(0.0, 0.0 ,1.0);
                wireBox(0.2, 0.9, 0.2);
            glPopMatrix();
            //head
            glPushMatrix();
            glTranslatef(-0.5, 0.45, 0.0);
            glColor3f(1.0, 1.0 ,0.0);
            wireBox(1.3, 1.3, 1.0);
            glPopMatrix();
        glPopMatrix();

        //body
        glPushMatrix();
            wireBox(2.6, 1.2, 1.0);
        glPopMatrix();

        //left back leg
        glPushMatrix();
            legs(0.25, 0.10);
        glPopMatrix();
        //right back leg
        glPushMatrix();
            legs(0.35, -0.10);
        glPopMatrix();
        //left front leg
        glPushMatrix();
            legs(-0.25, 0.10);
        glPopMatrix();
        //right front leg
        glPushMatrix();
            legs(-0.35, -0.10);
        glPopMatrix();

    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    if (w >= h)
        glViewport(0, 0, (GLsizei)h, (GLsizei)h);
    else
        glViewport(0, 0, (GLsizei)w, (GLsizei)w);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;
    gluOrtho2D(-1.0,1.0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void timer(int value)
{
    if(animation)
        glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

/*
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,1.0, 0.0, 1.0);
}
*/

int main(int argc, char * argv[])
{
    srand(time(NULL));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(500,500);
    glutCreateWindow("CG");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(mouse);
    timer(0);
    //init();
    glutMainLoop();
    return 0;
}