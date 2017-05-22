/*-----------------------------------------------------------------------------
 Name      : main.c
 Date      : September 6, 2016
 Class     : Computacao Grafica
 Author    : Renan A. B. Vieira
 R. A.     : 70212
 Descript. : This program draws several objects into the screen and allows
             its color changing and others parameters through keyboard and mouse
-----------------------------------------------------------------------------*/
/*Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/*Constants*/
#define PI 3.141592654

/*Global Variables*/
int w_size = 600; //screen width
int h_size = 600; //screen height

float R_color = 0.0; //red color
float G_color = 0.0; //green color
float B_color = 1.0; //blue color

float begin[2] = {0.0, 0.0}; //first click coordinate
float end[2]   = {0.0, 0.9}; //second click coordinate

int flag_click=0;  //flags the second click
int flag_screen=0; //flags screen for clean start
int figure=-1;     //stores which figure should be print (line, circle, rectangle)
int thickness=0;   //flag which tells whether thickness should be changed
int color=0;       //flag which tells whether color should be changed
int size_trace=1;  //defines the size of trace
int flag_clear=0;  //keeps track whether the screen should be clean

//clean screen with white background
void clean_screen()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
 }

//draws a line to the screen
void line()
{
    glLineWidth(size_trace);
    glColor3f(R_color, G_color, B_color);
    glBegin(GL_LINES);
        glVertex2f(begin[0], begin[1]);
        glVertex2f(end[0], end[1]);
    glEnd();
    glFlush();
}

//draws a circle to the screen
void circle()
{
    int i;
    float angulo;
    float raio=0.1;
    float x_pos=begin[0];
    float y_pos=begin[1];
    glLineWidth(size_trace);
    glColor3f(R_color, G_color, B_color);
    glBegin(GL_LINE_LOOP);
        raio = sqrt(pow(end[0] - begin[0], 2) + pow(end[1] - begin[1], 2));
        for (i=0 ; i<=100 ; i++)
        {
            angulo = ((2*PI*i) / (100.0));
            glVertex2f(x_pos + raio*(0.5*cos(angulo)), y_pos + raio*(0.5*sin(angulo)));
        }
    glEnd();
    glFlush();
}

//draws a rectangle to the screen
void rectangle()
{
    glLineWidth(size_trace);
    glColor3f(R_color, G_color, B_color);
    glRectf(begin[0], begin[1], end[0], end[1]);
    glFlush();
}

//displays figures to the display
void display()
{
    //first screen should be white
    if(!flag_screen)
    {
        clean_screen();
        flag_screen=1;
    }
    //chooses which figure will be showed
    switch(figure)
    {
        case 0:
            line();
        break;
        case 1:
            circle();
        break;
        case 2:
            rectangle();
        break;
    }
    //if requested by the user, the screen is cleared
    if(flag_clear)
    {
        clean_screen();
    }
}

//it is called to figured out which keyboard key was pressed
void Keyboard(unsigned char key, int x, int y)
{
    //thickness change was requested
    if(key == 'e' || key == 'E')
    {
        thickness = 1;
        color = 0;
    }
    //color change was requested
    if(key == 'k' || key == 'K')
    {
        thickness = 0;
        color = 1;
    }
    //line drawing was requested
    if(key == 'r' || key == 'R')
    {
        figure=0;
        flag_clear=0;
    }
    //circle drawing was requested
    if(key == 'c' || key == 'C')
    {
        figure=1;
        flag_clear=0;
    }
    //rectangle drawing was requested
    if(key == 'b' || key == 'B')
    {
        figure=2;
        flag_clear=0;
    }
    //clear screen was requested
    if(key == 'l' || key == 'L')
    {
        flag_clear=1;
    }
    //re-prints the screen with figures drawn
    if(figure!=-1)
    {
        glutPostRedisplay();
    }
    //decides which color was chosen
    if(color)
    {
        switch(key)
        {
            case '0': R_color = 1.0; G_color = 0.0; B_color = 0.0; //red
                 break;
            case '1': R_color = 0.0; G_color = 1.0; B_color = 0.0; //green
                 break;
            case '2': R_color = 0.0; G_color = 0.0; B_color = 1.0; //blue
                 break;
            case '3': R_color = 1.0; G_color = 1.0; B_color = 0.0; //yellow
                 break;
            case '4': R_color = 1.0; G_color = 0.0; B_color = 1.0; //pink
                 break;
            case '5': R_color = 0.0; G_color = 1.0; B_color = 1.0; //cyan
                 break;
            case '6': R_color = 0.0; G_color = 0.0; B_color = 0.0; //black
                 break;
            case '7': R_color = 0.9; G_color = 0.9; B_color = 0.9; //gray
                 break;
            case '8': R_color = 0.5; G_color = 0.5; B_color = 0.5; //dark gray
                 break;
            case '9': R_color = 0.25; G_color = 0.75; B_color = 0.25; //dark green
                 break;
        }
    }
    //sets thickness for figures
    if(thickness)
    {
        size_trace = key - 48;
    }
}

//it is called to figured out which mouse key was pressed
void MouseInt (int button, int state, int x, int y)
{
    switch(button)
    {
        //if left button pressed, gets x,y coordinates
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN)
            {
                if(!flag_click)
                {
                    begin[0] = ((float)x/w_size)*2 -1;
                    begin[1] = (-1)*(((float)y/w_size)*2 -1);
                    flag_click=1;
                }
                else
                {
                    end[0] = ((float)x/w_size)*2 -1;;
                    end[1] = (-1)*(((float)y/w_size)*2 -1);
                    //displays in the second click
                    if(figure!=-1)
                        glutPostRedisplay();
                    flag_click=0;
                }
            }
        break;
    }
}

//Main program
int main(int argc, char *argv[ ] )
{
   glutInit(&argc, argv);
       //sets display mode
       glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
       //arrange windows position
       glutInitWindowPosition(50,100);
       //sets windows size
       glutInitWindowSize(w_size,h_size);
       //create a new windows
       glutCreateWindow("Computacao Grafica");
       //calls display function
       glutDisplayFunc(display);
       //watches keyboard
       glutKeyboardFunc(Keyboard);
       //watches mouse
       glutMouseFunc(MouseInt);
   glutMainLoop();
   return 0;
}