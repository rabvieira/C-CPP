/* Iluminacao.c
 Iluminação de um bule
*/

#include <stdio.h>
#include <GL/glut.h>

GLfloat angle=45;
GLfloat fAspect;
GLint eixox=0, eixoy=0, eixoz=0;
int cafe0=0, cafe1=0, count=0, flip=0;
int animation = 0, cup = 0, cup_filled[2] = {0,0};
GLfloat y_up=0.0, x_right=0.0, fill_cup=0.0;

static int patchdata[][16] =
{
    // rim //
  {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15},

    // body //
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27},
  {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40},
    // lid //
//  {96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101,
//    101, 0, 1, 2, 3,},
//  {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112,
//    113, 114, 115, 116, 117},
    // bottom //
  {118, 118, 118, 118, 124, 122, 119, 121, 123, 126,
    125, 120, 40, 39, 38, 37},
    // handle //
  {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56},
  {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    28, 65, 66, 67},
    // spout //
//  {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
//    80, 81, 82, 83},
//  {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
//    92, 93, 94, 95}
};
// *INDENT-OFF* //

static float cpdata[][3] =
{
    {  0.2000,  0.0000, 2.70000 }, {  0.2000, -0.1120, 2.70000 },
    {  0.1120, -0.2000, 2.70000 }, {  0.0000, -0.2000, 2.70000 },
    {  1.3375,  0.0000, 2.53125 }, {  1.3375, -0.7490, 2.53125 },
    {  0.7490, -1.3375, 2.53125 }, {  0.0000, -1.3375, 2.53125 },
    {  1.4375,  0.0000, 2.53125 }, {  1.4375, -0.8050, 2.53125 },
    {  0.8050, -1.4375, 2.53125 }, {  0.0000, -1.4375, 2.53125 },
    {  1.5000,  0.0000, 2.40000 }, {  1.5000, -0.8400, 2.40000 },
    {  0.8400, -1.5000, 2.40000 }, {  0.0000, -1.5000, 2.40000 },
    {  1.7500,  0.0000, 1.87500 }, {  1.7500, -0.9800, 1.87500 },
    {  0.9800, -1.7500, 1.87500 }, {  0.0000, -1.7500, 1.87500 },
    {  2.0000,  0.0000, 1.35000 }, {  2.0000, -1.1200, 1.35000 },
    {  1.1200, -2.0000, 1.35000 }, {  0.0000, -2.0000, 1.35000 },
    {  2.0000,  0.0000, 0.90000 }, {  2.0000, -1.1200, 0.90000 },
    {  1.1200, -2.0000, 0.90000 }, {  0.0000, -2.0000, 0.90000 },
    { -2.0000,  0.0000, 0.90000 }, {  2.0000,  0.0000, 0.45000 },
    {  2.0000, -1.1200, 0.45000 }, {  1.1200, -2.0000, 0.45000 },
    {  0.0000, -2.0000, 0.45000 }, {  1.5000,  0.0000, 0.22500 },
    {  1.5000, -0.8400, 0.22500 }, {  0.8400, -1.5000, 0.22500 },
    {  0.0000, -1.5000, 0.22500 }, {  1.5000,  0.0000, 0.15000 },
    {  1.5000, -0.8400, 0.15000 }, {  0.8400, -1.5000, 0.15000 },
    {  0.0000, -1.5000, 0.15000 }, { -1.6000,  0.0000, 2.02500 },
    { -1.6000, -0.3000, 2.02500 }, { -1.5000, -0.3000, 2.25000 },
    { -1.5000,  0.0000, 2.25000 }, { -2.3000,  0.0000, 2.02500 },
    { -2.3000, -0.3000, 2.02500 }, { -2.5000, -0.3000, 2.25000 },
    { -2.5000,  0.0000, 2.25000 }, { -2.7000,  0.0000, 2.02500 },
    { -2.7000, -0.3000, 2.02500 }, { -3.0000, -0.3000, 2.25000 },
    { -3.0000,  0.0000, 2.25000 }, { -2.7000,  0.0000, 1.80000 },
    { -2.7000, -0.3000, 1.80000 }, { -3.0000, -0.3000, 1.80000 },
    { -3.0000,  0.0000, 1.80000 }, { -2.7000,  0.0000, 1.57500 },
    { -2.7000, -0.3000, 1.57500 }, { -3.0000, -0.3000, 1.35000 },
    { -3.0000,  0.0000, 1.35000 }, { -2.5000,  0.0000, 1.12500 },
    { -2.5000, -0.3000, 1.12500 }, { -2.6500, -0.3000, 0.93750 },
    { -2.6500,  0.0000, 0.93750 }, { -2.0000, -0.3000, 0.90000 },
    { -1.9000, -0.3000, 0.60000 }, { -1.9000,  0.0000, 0.60000 },
    {  1.7000,  0.0000, 1.42500 }, {  1.7000, -0.6600, 1.42500 },
    {  1.7000, -0.6600, 0.60000 }, {  1.7000,  0.0000, 0.60000 },
    {  2.6000,  0.0000, 1.42500 }, {  2.6000, -0.6600, 1.42500 },
    {  3.1000, -0.6600, 0.82500 }, {  3.1000,  0.0000, 0.82500 },
    {  2.3000,  0.0000, 2.10000 }, {  2.3000, -0.2500, 2.10000 },
    {  2.4000, -0.2500, 2.02500 }, {  2.4000,  0.0000, 2.02500 },
    {  2.7000,  0.0000, 2.40000 }, {  2.7000, -0.2500, 2.40000 },
    {  3.3000, -0.2500, 2.40000 }, {  3.3000,  0.0000, 2.40000 },
    {  2.8000,  0.0000, 2.47500 }, {  2.8000, -0.2500, 2.47500 },
    {  3.5250, -0.2500, 2.49375 }, {  3.5250,  0.0000, 2.49375 },
    {  2.9000,  0.0000, 2.47500 }, {  2.9000, -0.1500, 2.47500 },
    {  3.4500, -0.1500, 2.51250 }, {  3.4500,  0.0000, 2.51250 },
    {  2.8000,  0.0000, 2.40000 }, {  2.8000, -0.1500, 2.40000 },
    {  3.2000, -0.1500, 2.40000 }, {  3.2000,  0.0000, 2.40000 },
    {  0.0000,  0.0000, 3.15000 }, {  0.8000,  0.0000, 3.15000 },
    {  0.8000, -0.4500, 3.15000 }, {  0.4500, -0.8000, 3.15000 },
    {  0.0000, -0.8000, 3.15000 }, {  0.0000,  0.0000, 2.85000 },
    {  1.4000,  0.0000, 2.40000 }, {  1.4000, -0.7840, 2.40000 },
    {  0.7840, -1.4000, 2.40000 }, {  0.0000, -1.4000, 2.40000 },
    {  0.4000,  0.0000, 2.55000 }, {  0.4000, -0.2240, 2.55000 },
    {  0.2240, -0.4000, 2.55000 }, {  0.0000, -0.4000, 2.55000 },
    {  1.3000,  0.0000, 2.55000 }, {  1.3000, -0.7280, 2.55000 },
    {  0.7280, -1.3000, 2.55000 }, {  0.0000, -1.3000, 2.55000 },
    {  1.3000,  0.0000, 2.40000 }, {  1.3000, -0.7280, 2.40000 },
    {  0.7280, -1.3000, 2.40000 }, {  0.0000, -1.3000, 2.40000 },
};
static float tex[2][2][2] =
{
  { {0, 0},
    {1, 0}},
  { {0, 1},
    {1, 1}}
};

static void
teacup(GLint grid, GLdouble scale, GLenum type, GLdouble x_pos)
{
  float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
  long i, j, k, l;

  glPushAttrib(GL_ENABLE_BIT | GL_EVAL_BIT);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_MAP2_VERTEX_3);
  glEnable(GL_MAP2_TEXTURE_COORD_2);
  glPushMatrix();
  glColor3f(1.0f, 0.2f, 0.2f);
  if(cup_filled[0] == 1 && cafe0==1 && count==1)
    glColor3f(0.0f, 0.1f, 0.1f);
  if(cup_filled[1] == 1 && cafe1==1)
    glColor3f(0.0f, 0.1f, 0.1f);
  glTranslatef(x_pos, 0.0, 0.0);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glScalef(0.5 * scale, 0.5 * scale, 0.5 * scale);
  glTranslatef(0.0, 0.0, -4.0);
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 4; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 3; l++) {
          p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
          q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
          if (l == 1)
            q[j][k][l] *= -1.0;
          if (i < 6) {
            r[j][k][l] =
              cpdata[patchdata[i][j * 4 + (3 - k)]][l];
            if (l == 0)
              r[j][k][l] *= -1.0;
            s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
            if (l == 0)
              s[j][k][l] *= -1.0;
            if (l == 1)
              s[j][k][l] *= -1.0;
          }
        }
      }
    }
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2,
      &tex[0][0][0]);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &p[0][0][0]);
    glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
    glEvalMesh2(type, 0, grid, 0, grid);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &q[0][0][0]);
    glEvalMesh2(type, 0, grid, 0, grid);
    if (i < 6) {
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &r[0][0][0]);
      glEvalMesh2(type, 0, grid, 0, grid);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &s[0][0][0]);
      glEvalMesh2(type, 0, grid, 0, grid);
    }
  }
  glPopMatrix();
  glPopAttrib();
}

void glassCup()
{
    glEnable (GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(19.0, 0.0, 15.0);
    glRotatef(90., 1 ,0 ,0);
        glPushMatrix();
        glTranslatef(0.0, 0.0, 5.0);
        glRotatef(180., 1 ,0 ,0);
        glutSolidCone(2.0,5.0,20,20);
        glPopMatrix();
    glutSolidCone(2.0,5.0,20,20);
    glPopMatrix();
}

void table(GLdouble width, GLdouble height, GLdouble depth)
{
    glPushMatrix();
        glColor3f(0.51f, 0.32f, 0.0f);
        glTranslatef(0.0, -7.0, 0.0);
        glScalef(width, height, depth);
        //glutWireCube(10.0);
        glutSolidCube(10.0);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.51f, 0.32f, 0.0f);
        glTranslatef(-25.0, -15.0, -25.0);
        glScalef(1.0, 2.0, 1.0);
        //glutWireCube(10.0);
        glutSolidCube(10.0);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.51f, 0.32f, 0.0f);
        glTranslatef(25.0, -15.0, 25.0);
        glScalef(1.0, 2.0, 1.0);
        //glutWireCube(10.0);
        glutSolidCube(10.0);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.51f, 0.32f, 0.0f);
        glTranslatef(25.0, -15.0, -25.0);
        glScalef(1.0, 2.0, 1.0);
        //glutWireCube(10.0);
        glutSolidCube(10.0);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.51f, 0.32f, 0.0f);
        glTranslatef(-25.0, -15.0, 25.0);
        glScalef(1.0, 2.0, 1.0);
        //glutWireCube(10.0);
        glutSolidCube(10.0);
    glPopMatrix();

}

void teapot(GLdouble size)
{
    glPushMatrix();
        glColor3f(0.11f, 0.32f, 0.75f);
        glTranslatef(-15.0, 0.0, 0.0);
        //glutWireTeapot(size);
        glutSolidTeapot(size);
    glPopMatrix();
}

void up()
{
    y_up += 0.1;
    glutPostRedisplay();
}

void right()
{
    x_right += 0.1;
    glutPostRedisplay();
}

void fill_cup_rotate()
{
    fill_cup -= 0.1;
    glutPostRedisplay();
}

/* Chamada para fazer o desenho*/
void RenderScene(void) {
    int limiar;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(0.5f, 0.5f, 0.5f);

    if(animation==0)
    {
    /* Desenha o teapot com a cor corrente (solid) */
    glPushMatrix();
      glRotatef(eixox, 1 ,0 ,0);
          glRotatef(eixoy, 0 ,1 ,0);
          glRotatef(eixoz, 0 ,0 ,1);
          teapot(5.0);
          glassCup();
        teacup(14, 2, GL_FILL,20.0);
        teacup(14, 2, GL_FILL,10.0);
      table(8.0,0.55,8.0);
    glPopMatrix();
    }

    if(animation==1)
    {
    /* Desenha o teapot com a cor corrente (solid) */
    glPushMatrix();
      glRotatef(eixox, 1 ,0 ,0);
          glRotatef(eixoy, 0 ,1 ,0);
          glRotatef(eixoz, 0 ,0 ,1);
        teacup(14, 2, GL_FILL,20.0);
        teacup(14, 2, GL_FILL,10.0);
      table(8.0,0.55,8.0);
      glassCup();
      glPushMatrix();
        glTranslatef(0.0,y_up,0.0);
        teapot(5.0);
        up();
      glPopMatrix();
      if(y_up>10)
      {
            animation = 2;
            glutPostRedisplay();
      }
    glPopMatrix();
    }
    if(animation==2)
    {
    /* Desenha o teapot com a cor corrente (solid) */
    glPushMatrix();
      glRotatef(eixox, 1 ,0 ,0);
          glRotatef(eixoy, 0 ,1 ,0);
          glRotatef(eixoz, 0 ,0 ,1);
        teacup(14, 2, GL_FILL,20.0);
        teacup(14, 2, GL_FILL,10.0);
      table(8.0,0.55,8.0);
      glassCup();
      glPushMatrix();
        glTranslatef(x_right,10.0,0.0);
        teapot(5.0);
        right();
      glPopMatrix();
        limiar = 15;
      if(cup)
        limiar = 25;
      if(x_right>=limiar)
      {
            animation = 3;
            glutPostRedisplay();
      }
    glPopMatrix();
    }
    if(animation==3)
    {
    /* Desenha o teapot com a cor corrente (solid) */
    glPushMatrix();
      glRotatef(eixox, 1 ,0 ,0);
          glRotatef(eixoy, 0 ,1 ,0);
          glRotatef(eixoz, 0 ,0 ,1);
        teacup(14, 2, GL_FILL,20.0);
        count++;
        teacup(14, 2, GL_FILL,10.0);
      table(8.0,0.55,8.0);
      glassCup();
      glPushMatrix();
        glTranslatef(x_right,y_up,0.0);
        glRotatef(fill_cup,0.0,0.0,1.0);
        teapot(5.0);
        fill_cup_rotate();
      glPopMatrix();
      if(fill_cup<=-25)
      {
            animation = 0;
            glutPostRedisplay();
      }
    glPopMatrix();
    if(cup_filled[0] == 1)
        cafe0=1;
    if(cup_filled[1] == 1)
        cafe1=1;
    }
    glutPostRedisplay();
    glutSwapBuffers();
}


/* Inicialização */
void SetupRC(void) {
    GLfloat luzAmbiente[4]={0.5,0.5,0.5,1.0};
    GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};
    GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

    GLfloat objeto_ambiente[4] = {0.5,0.0,0.0,1.0};
    GLfloat objeto_difusa[4] = {1.0,0.0,0.0,1.0};

    /* Capacidade de brilho do material */
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 30;


/* Especifica que a cor de fundo da janela será preta */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

/* Ativa o uso da luz ambiente */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

/* Define os parâmetros da luz de número 0 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );


/* define as propriedades do material */
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambiente);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
/* Define a refletância do material */
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
/* Define a concentração do brilho */
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

/* Habilita a definição da cor do material a partir da cor corrente */
    glEnable(GL_COLOR_MATERIAL);
/* Habilita o uso de iluminação*/
    glEnable(GL_LIGHTING);
/* Habilita a luz de número 0 */
    glEnable(GL_LIGHT0);
/* Habilita o depth-buffering */
    glEnable(GL_DEPTH_TEST);
/*glShadeModel(GL_FLAT);*/

}


/* Função usada para especificar o volume de visualização */
void Viewing(void)    {
    /* Especifica sistema de coordenadas de projeção */
    glMatrixMode(GL_PROJECTION);
    /* Inicializa sistema de coordenadas de projeção */
    glLoadIdentity();

    /* Especifica a projeção perspectiva */
    gluPerspective(angle,fAspect,0.1,500);

    /* Especifica sistema de coordenadas do modelo */
    glMatrixMode(GL_MODELVIEW);
    /* Inicializa sistema de coordenadas de projeção */
    glLoadIdentity();

    /* Especifica posição do observador e do alvo */
    gluLookAt(0,80,200, 0,0,0, 0,1,0);
}


/* Chamada pela GLUT quando a janela é redimensionada */
void ChangeSize(GLsizei w, GLsizei h)    {
    /* Para previnir uma divisão por zero */
    if ( h == 0 )
        h = 1;

    /* Especifica o tamanho da viewport */
    glViewport(0, 0, w, h);

    /* Calcula a correção de aspecto */
    fAspect = (GLfloat)w/(GLfloat)h;

    Viewing();
}


/* Callback para gerenciar eventos do mouse */
void HandleMouse(int button, int state, int x, int y)    {
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {
            if (angle >= 10)
                angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {
            if (angle <= 130)
                angle += 5;
        }
    Viewing();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 'x':
    eixox = (eixox + 5) % 360;
    glutPostRedisplay();
    break;
  case 'y':
    eixoy = (eixoy + 5) % 360;
    glutPostRedisplay();
    break;
  case 'z':
    eixoz = (eixoz + 5) % 360;
    glutPostRedisplay();
    break;
  case 'X':
    eixox = (eixox - 5) % 360;
    glutPostRedisplay();
    break;
  case 'Y':
    eixoy = (eixoy - 5) % 360;
    glutPostRedisplay();
    break;
  case 'Z':
    eixoz = (eixoz - 5) % 360;
    glutPostRedisplay();
    break;
  case 'r':
  case 'R':
    if(cup_filled[0]==1 && cup_filled[1]==1)
        break;

    if(cup && y_up >=0)
    {
        cup = 0;
        y_up=0.0;
        x_right=0.0;
        fill_cup=0.0;
    }

    if(y_up==0)
    {
        cup = 0;
        cup_filled[0] = 1;
    }
    else
    {
        cup_filled[1] = 1;
        cup = 1;
        y_up=0.0;
        x_right=0.0;
        fill_cup=0.0;
    }

    animation = 1;
    glutPostRedisplay();
    break;
    case 'c':
    case 'C':
        //empty cup
        if(flip==0)
        {
            cup_filled[0] = 0;
            flip = 1;
        }
        else
        {
            cup_filled[1] = 0;
            flip=0;
        }
    break;
  }
}

/* Programa Principal */
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("Visualizacao 3D");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(HandleMouse);
    glutKeyboardFunc(keyboard);
    SetupRC();
    glutMainLoop();
}