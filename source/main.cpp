/*
---------------------------------------------
Controllers:

-Mouse:
	'left click': Creates control point
-Keyboard:
	'b': Draws bezier curve
	'n': Draws normal vectors on a Bezier curve
	'ESC': Exits program
---------------------------------------------
 */

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "math.h"
#include "point.h"

const int WINDOW_W = 700;
const int WINDOW_H = 700;

std::vector <Point> controlPoints;
std::vector <Point> pointsOnBezier;
std::vector <Point> firstDerivative;
std::vector <Point> secondDerivative;
bool bezier = false;
bool normal = false;
bool edges = true;
float eval;

void drawPoint();
void drawLines(const std::vector<Point>& pontos, float r, float g, float b);
void drawLine(Point a, Point b);
void drawBezierCurve(const std::vector<Point>& pontos);
void drawNormals();
Point deCasteljau(const std::vector<Point> pontos, double t);
Point orthogonalization(Point u, Point v);
std::vector<Point> getDerivativeControllers(const std::vector<Point> u);
double norm(Point p);


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawPoint();

	if(controlPoints.size() > 1)
	{
		if(edges) drawLines(controlPoints, 0.0f, 1.0f, 1.0f);
		if(bezier) drawBezierCurve(controlPoints);
		if(bezier && normal) drawNormals();
	}
	glFlush();
}

/* Draws a control point */
void drawPoint()
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(auto p : controlPoints)
		glVertex2d(p.x, p.y);
	glEnd();
}

/* Draws an array of lines */
void drawLines(const std::vector<Point>& pontos, float r, float g, float b)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(r, g, b);
	for(auto p : pontos)
		glVertex2d(p.x, p.y);
	glEnd();
}

/* Draws a single line */
void drawLine(Point a_, Point b_)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0.5f, 0.0f, 0.5f);
	glVertex2d(a_.x, a_.y);
	glVertex2d(b_.x, b_.y);
	glEnd();
}

/* Draws normal vectors on a Bezier curve */
void drawNormals()
{
	firstDerivative = getDerivativeControllers(controlPoints);
	if(controlPoints.size() > 2)
		secondDerivative = getDerivativeControllers(firstDerivative);
	else
		secondDerivative.push_back(Point(0,0));

	Point pd(0,0);
	Point sd(0,0);
	Point orth(0,0);

	int i = 0;

	for (double t = 0.0; t <= eval; ++t) {
		std::cout << "t = " << t << "  ";
		pd = deCasteljau(firstDerivative, t/eval);
		sd = deCasteljau(secondDerivative, t/eval);
		orth = orthogonalization(pd, sd);

		std::cout << "normal(" << norm(orth) << ")\n";

		drawLine(pointsOnBezier[t], Point(pointsOnBezier[t].x + orth.x, pointsOnBezier[t].y + orth.y));
		++i;
	}
	std::cout << "\ni = " << i << "\n-----------\n";

}

/* De Casteljau algorithm */
Point deCasteljau(const std::vector<Point> pontos, double t)
{
	std::vector<Point> p = pontos;

	for (int n = 0; n < pontos.size()-1; ++n) {
		for (int b = 0; b < pontos.size()-1 - n; ++b) {
			p[b].x = (1-t) * p[b].x + t * p[b+1].x;
			p[b].y = (1-t) * p[b].y + t * p[b+1].y;
		}
	}
	return p[0];
}

/* Draws a Bezier curve */
void drawBezierCurve(const std::vector<Point>& pontos)
{
	pointsOnBezier.clear();

	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 0.0f);
	for (double t = 0.0; t <= eval; ++t) {
		Point p = deCasteljau(pontos, t/eval);
		pointsOnBezier.push_back(p);
		glVertex2d(p.x,p.y);
	}
	glVertex2d(pontos.back().x, pontos.back().y);
	glEnd();
}

/* Obtains the derivate's control points of a Bezier curve */
std::vector<Point> getDerivativeControllers(const std::vector<Point> u)
{
	std::vector<Point> v;
	for (int i = 0; i < u.size()-1; ++i)
		v.push_back(Point(u[i+1].x - u[i].x, u[i+1].y - u[i].y));
	return v;
}

/* Computes scalar product */
float scalarProduct(Point u, Point v)
{
	return (u.x * v.x + u.y * v.y);
}

/* Computes vector's norm */
double norm(Point p)
{
	return sqrt(scalarProduct(p,p));
}

/* Orthogonalization of two vectors */
Point orthogonalization(Point u, Point v)
{
	Point w(0,0);

	if(u.x == w.x && u.y == w.y) return w;

	w.x = v.x - (scalarProduct(u,v)/scalarProduct(u,u)) * u.x;
	w.y = v.y - (scalarProduct(u,v)/scalarProduct(u,u)) * u.y;

	return w;
}

/* Set how many evaluation will be done */
void setEvaluation()
{
	do {
		std::cout << "Enter number of evaluations:\n";
		std::cin >> eval;
	} while(eval==0);
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
        case 'n':
        	normal = !normal;
        	break;
        case 'e':
        	edges = !edges;
        	break;
    }
    glutPostRedisplay();
}

void handleMouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {
            controlPoints.push_back(Point(x, y));
        }
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	setEvaluation();

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
