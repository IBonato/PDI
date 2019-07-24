//Compile: gcc -o CubePerspective CubePerspective.c -lglut -lGL -lGLU -lm

#include <GL/glut.h>

int anguloCubo;
GLdouble obsX = 20, obsY = 20, obsZ = 20;

//Função de timer para a rotação do cubo
void timer(int valor)
{
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

//Função que desenha o referencial de mundo
void desenharef()
{
    glLineWidth(3.0f);
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f); //Red
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(15.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f); //Green
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 15.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); //Blue
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 15.0f);

    glEnd();
}

//Função que desenha o cubo
void desenhaCubo(float centroEixoX, float centroEixoY, float centroEixoZ, float lado)
{
    float mlado = lado * 0.5f;

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f); //Orange
    // face superior
    glVertex3f(centroEixoX + mlado, centroEixoY + mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY + mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY + mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY + mlado, centroEixoZ + mlado);

    glColor3f(1.0f, 0.0f, 1.0f); //Magenta
    // face inferior
    glVertex3f(centroEixoX + mlado, centroEixoY - mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY - mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY - mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY - mlado, centroEixoZ - mlado);

    glColor3f(1.0f, 0.0f, 0.0f); //Red
    // face frontal
    glVertex3f(centroEixoX + mlado, centroEixoY + mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY + mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY - mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY - mlado, centroEixoZ + mlado);

    glColor3f(0.0f, 1.0f, 0.0f); //Green
                                 // face traseira
    glVertex3f(centroEixoX + mlado, centroEixoY - mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY - mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY + mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY + mlado, centroEixoZ - mlado);

    glColor3f(1.0f, 1.0f, 0.0f); //Yellow
    // face esquerda
    glVertex3f(centroEixoX - mlado, centroEixoY + mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY + mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY - mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX - mlado, centroEixoY - mlado, centroEixoZ + mlado);

    glColor3f(0.0f, 0.0f, 1.0f); //Blue
    // face direita
    glVertex3f(centroEixoX + mlado, centroEixoY + mlado, centroEixoZ - mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY + mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY - mlado, centroEixoZ + mlado);
    glVertex3f(centroEixoX + mlado, centroEixoY - mlado, centroEixoZ - mlado);

    glEnd();
}

//Função que muda a posição da câmera de acordo com as teclas pressionadas
void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:
        obsX = 10;
        obsY = 30;
        obsZ = 3;
        break;
    case GLUT_KEY_F2:
        obsX = 10;
        obsY = -30;
        obsZ = 3;
        break;
    case GLUT_KEY_F3:
        obsX = 30;
        obsY = 6;
        obsZ = 3;
        break;
    case GLUT_KEY_F4:
        obsX = -30;
        obsY = 6;
        obsZ = 3;
        break;
    case GLUT_KEY_F5:
        obsX = 10;
        obsY = 6;
        obsZ = 30;
        break;
    case GLUT_KEY_F6:
        obsX = 10;
        obsY = 6;
        obsZ = -30;
        break;
    }
    glLoadIdentity();
    gluLookAt(obsX, obsY, obsZ, 10, 6, 3, 0, 1, 0);
    glutPostRedisplay();
}

//Função call para fazer o desenho
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsX, obsY, obsZ, 0, 0, 0, 0, 0, 1); //Posição da Câmera
    desenharef();
    glEnable(GL_CULL_FACE);
    glRotatef(anguloCubo, 10.0f, 6.0f, 3.0f); //Rotaciona o cubo no seu centro de massa
    desenhaCubo(10.0f, 6.0f, 3.0f, 6.0f);
    glutSwapBuffers();
    anguloCubo -= 1.5f;
}

//Função que inicializa os parâmetros de rendering
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);              //Fundo preto e opaco
    glShadeModel(GL_SMOOTH);                           //Enable Smooth Shading
    glEnable(GL_DEPTH_TEST);                           //Enable Depth Test
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Perspective corrections
}

//Função que re-escala a janela
void reshape(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1; //Para não ter divisão por zero
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0f, aspect, 0.1f, 500.0f);
}

//Função Principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800.0f, 600.0f);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Exercicios 3.4");
    glutSpecialFunc(SpecialKeys);
    init();
    glutTimerFunc(30, timer, 0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
