#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "ponto.h"
#include "mathutil.h"

const int WINDOW_W = 500;
const int WINDOW_H = 500;

std::vector <Ponto> pontos;
std::vector <Ponto> pontos_;
std::vector <Ponto> primeiraDerivada;;
std::vector <Ponto> segundaDerivada;
std::vector <Ponto> normals;
bool bezier = false;

void drawPoints();
void drawLines(const std::vector<Ponto>& pontos);
void drawDeCasteljau(const std::vector<Ponto>& pontos);
void drawNormals();

//Ponto deCasteljau(const std::vector<Ponto> pontos, int n, int i, float t);
Ponto deCasteljau(const std::vector<Ponto> pontos, float t);
Ponto orthogonalization(Ponto u, Ponto v);
std::vector<Ponto> getDerivativeControllers(const std::vector<Ponto> u);
std::vector<Ponto> vectorsNorm(std::vector<Ponto> ps);
double norm(Ponto p);

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawPoints();
/*
	pontos.push_back(Ponto(1.0f,-1.0f));
	pontos.push_back(Ponto(2.0f,1.0f));
	pontos.push_back(Ponto(0.0f,3.0f));
	pontos.push_back(Ponto(-1.0f,1.0f));

	drawLines(pontos);

	primeiraDerivada = getDerivativeControllers(pontos);
	Ponto p = deCasteljau(primeiraDerivada, 0.5f);

	//std::cout << "(" << u.x << "," << u .y << ")\n";

	for (auto p : primeiraDerivada) {
		std::cout << "primeira derivada(" << p.x << "," << p.y << ")\n";
	}
	std::vector<Ponto> pd = vectorsNorm(primeiraDerivada);
	for (auto p : pd) {
		std::cout << "pd(" << p.x << "," << p.y << ")\n";
	}

	segundaDerivada = getDerivativeControllers(primeiraDerivada);
	std::vector<Ponto> sd = vectorsNorm(segundaDerivada);

	//Ponto p(0,0);
	//MathUtil::bezier(pontos, 0.5f, p);
	Ponto p = deCasteljau(pontos, 0.5f);
	std::cout << "(" << p.x << "," << p.y << ")\n";

	//Ponto u(0,0);
	//MathUtil::bezier(primeiraDerivada, 0.5f, u);
	Ponto u = deCasteljau(pd, 0.5f);
	std::cout << "pd(" << u.x << "," << u.y << ")\n";

	//Ponto v(0,0);
	//MathUtil::bezier(segundaDerivada, 0.5f, v);
	Ponto v = deCasteljau(sd, 0.5f);
	std::cout << "sd(" << v.x << "," << v.y << ")\n";

	Ponto w(0,0);
	orthogonalization(u,v,w);
	std::cout << "ortho(" << w.x << "," << w.y << ")\n";
*/

	if(pontos.size() > 1)
	{
		drawLines(pontos);
		if(bezier) drawDeCasteljau(pontos);
		//if(bezier) drawBezierCurve(pontos);
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

void drawNormals()
{
	primeiraDerivada = getDerivativeControllers(pontos);
	segundaDerivada = getDerivativeControllers(primeiraDerivada);

	Ponto pd(0,0);
	Ponto sd(0,0);
	Ponto orth(0,0);
	float t = 0.5f;

	if(pontos.size() > 2) {
		for (float t = 0.0f; t < 1.0f; t += 0.001f) {
			pd = deCasteljau(primeiraDerivada, t);
			sd = deCasteljau(segundaDerivada, t);
			orth = orthogonalization(pd, sd);
			normals.push_back(Ponto(pontos_[t].x + orth.x, pontos_[t].y + orth.y));
		}

		drawLines(normals);
	}
}

Ponto deCasteljau(const std::vector<Ponto> pontos, float t)
{
	std::vector<Ponto> p = pontos;

	for (int n = 0; n < pontos.size()-1; ++n) {
		for (int b = 0; b < pontos.size()-1 - n; ++b) {
			p[b].x = (1-t) * p[b].x + t * p[b+1].x;
			p[b].y = (1-t) * p[b].y + t * p[b+1].y;
		}
	}
	return p[0];
}

void drawDeCasteljau(const std::vector<Ponto>& pontos)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 0.0f);
	for (float t = 0.0f; t < 1.0f; t += 0.001f) {
		//Ponto p = deCasteljau(pontos, pontos.size()-1, 0, t);
		Ponto p = deCasteljau(pontos, t);
		pontos_.push_back(p);
		glVertex2d(p.x,p.y);
	}
	glEnd();

	drawNormals();
}

/*
Ponto deCasteljau(const std::vector<Ponto> pontos, int n, int i, float t)
	if(n==0) return pontos[i];

	Ponto p1 = deCasteljau(pontos, n-1, i, t);
	Ponto p2 = deCasteljau(pontos, n-1, i+1, t);

	return Ponto((1-t) * p1.x + t * p2.x, (1-t) * p1.y + t * p2.y);
*/

// Guarda os pontos de controle da derivada
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

// Norma de um vetor
double norm(Ponto p)
{
	return sqrt(scalarProduct(p,p));
}

// Norma para um conj de vetores
std::vector<Ponto> vectorsNorm(std::vector<Ponto> ps)
{
	std::vector<Ponto> vectorsNorm_;
	for(auto p : ps)
		vectorsNorm_.push_back(Ponto(p.x/norm(p), p.y/norm(p)));
	return vectorsNorm_;
}

Ponto orthogonalization(Ponto u, Ponto v)
{
	Ponto w(0,0);

	//if(u == w) return w;

	w.x = v.x - (scalarProduct(u,v)/scalarProduct(u,u)) * u.x;
	w.y = v.y - (scalarProduct(u,v)/scalarProduct(u,u)) * u.y;

	return Ponto(w.x, w.y);
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
    if(button == GLUT_LEFT_BUTTON)
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
