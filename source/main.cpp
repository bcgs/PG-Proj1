#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "ponto.h"
#include "mathutil.h"

const int WINDOW_W = 500;
const int WINDOW_H = 500;

std::vector <Ponto> pontos;
bool bezier = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if(pontos.size() > 0) {
        
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (pontos.size() > 1) {
            if (bezier) {
                Ponto p(0,0);
                for (float t = 0.0; t <= 1.0; t += 0.01) {
                    MathUtil::bezier(pontos, t, p);
                    glVertex2d(p.x, p.y);
                }
            } else {
                for (auto p : pontos)
                    glVertex2d(p.x, p.y);
            }
        }
        //else {
            //auto p = pontos.front();
            //glVertex2d(p.x, p.y);
        //}
        glEnd();
        
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        for(auto p : pontos)
            glVertex2d(p.x, p.y);
        glEnd();
    }
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_W, WINDOW_H, 0.0f, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
        case 'b':
            bezier = !bezier;
    }
    glutPostRedisplay();
}

void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN)
            pontos.push_back(Ponto(x, y));
    
    glutPostRedisplay();
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0); // upper left
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    
    glutCreateWindow("PG-Proj1");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW); // scene model matrix
    glLoadIdentity();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouseClick);
    glutReshapeFunc(reshape);
    
    glutMainLoop();
    
    return 0;
}
