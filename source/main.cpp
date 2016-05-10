#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "ponto.h"
#include "mathutil.h"

const int WINDOW_W = 500;
const int WINDOW_H = 500;

std::vector <Ponto> pontos;
std::vector <Ponto> primeiraDerivada;;
std::vector <Ponto> segundaDerivada;
bool bezier = false;

void drawPoints();
void drawLines(const std::vector<Ponto>& pontos);
void drawBezierCurve(const std::vector<Ponto>& pontos);
void orthogonalization(Ponto u, Ponto v, Ponto& w);
std::vector<Ponto> getDerivativeControllers(const std::vector<Ponto> u);

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawPoints();

	pontos.push_back(Ponto(100.0f,5.0f));
	pontos.push_back(Ponto(200.0f,150.0f));
	pontos.push_back(Ponto(10.0f,300.0f));
	pontos.push_back(Ponto(190.0f,125.0f));

	drawLines(pontos);

	primeiraDerivada = getDerivativeControllers(pontos);
	segundaDerivada = getDerivativeControllers(primeiraDerivada);

	Ponto p(0,0);
	MathUtil::bezier(pontos, 0.5f, p);
	std::cout << "(" << p.x << "," << p.y << ")\n";

	Ponto u(0,0);
	MathUtil::bezier(primeiraDerivada, 0.5f, u);
	std::cout << "pd(" << u.x << "," << u.y << ")\n";

	Ponto v(0,0);
	MathUtil::bezier(segundaDerivada, 0.5f, v);
	std::cout << "sd(" << v.x << "," << v.y << ")\n";

	Ponto w(0,0);
	orthogonalization(u,v,w);
	std::cout << "ortho(" << w.x << "," << w.y << ")\n";


	if(pontos.size() > 1) {

		drawLines(pontos);
		if(bezier) drawBezierCurve(pontos);
/*
		primeiraDerivada = getDerivativeControllers(pontos);
		segundaDerivada = getDerivativeControllers(primeiraDerivada);

		for(auto p : pontos)
			std::cout << "(" << p.x << "," << p.y << ")\n";

		Ponto p(0,0);
		MathUtil::bezier(pontos, 0.5f, p);
		std::cout << "bz(" << p.x << "," << p.y << ")\n";

		Ponto u(0,0);
		MathUtil::bezier(primeiraDerivada, 0.5f, u);
		std::cout << "pd(" << u.x << "," << u.y << ")\n";

		Ponto v(0,0);
		MathUtil::bezier(segundaDerivada, 0.5f, v);
		std::cout << "sd(" << v.x << "," << v.y << ")\n";

		Ponto w(0,0);
		orthogonalization(u,v,w);
		std::cout << "ortho(" << w.x << "," << w.y << ")\n";
*/
	}
	glFlush();
}

void drawPoints()
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(auto p : pontos)
		glVertex2d(p.x, p.y);
	glEnd();
}

void drawLines(const std::vector<Ponto>& pontos)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 1.0f, 1.0f);
	for(auto p : pontos)
		glVertex2d(p.x, p.y);
	glEnd();
}

void drawBezierCurve(const std::vector<Ponto>& pontos)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 0.0f);
	Ponto p(0,0);
	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		MathUtil::bezier(pontos, t, p);
		glVertex2d(p.x,p.y);
	}
	glEnd();
}

std::vector<Ponto> getDerivativeControllers(const std::vector<Ponto> u)
{
	std::vector<Ponto> v;
	for (int i = 0; i < u.size()-1; ++i)
		v.push_back(Ponto(u[i+1].x - u[i].x, u[i+1].y - u[i].y));
	return v;
}

float scalarProduct(Ponto u, Ponto v)
{
	return (u.x * v.x + u.y * v.y);
}

void orthogonalization(Ponto u, Ponto v, Ponto& w)
{
	w.x = v.x - (scalarProduct(u,v)/scalarProduct(u,u)) * u.x;
	w.y = v.y - (scalarProduct(u,v)/scalarProduct(u,u)) * u.y;
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_W, WINDOW_H, 0.0f, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleKeypress(unsigned char key, int x, int y)
{
    switch (key) {
        case 27: // ESC
            exit(0);
        case 'b':
            bezier = !bezier;
            break;
    }
    glutPostRedisplay();
}

void handleMouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {
            pontos.push_back(Ponto(x, y));
            std::cout << "(" << x << "," << y << ")\n";
        }
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0); // upper left
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
