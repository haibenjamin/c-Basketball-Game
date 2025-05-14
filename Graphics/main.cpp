#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <vector>
using namespace std;

const int HEIGHT = 600;
char score[] = "00";
const int WIDTH = 600;
double startingX = 0.5;
double startingY = 0;
const double PI = 3.14;
double offset = 0.1;
double offset2 = 0.1;
double ballX = 0.5, ballY = 0;
double Vx = 0, Vy = 0;
double t = 0;
double a = 0;
bool flag = false;
double rotationAngle = 0.0;
bool isMoving = false;
bool overRing = false;
bool hitFloorOnce = true;
const double speedThreshold = 0.0250;

typedef struct {
	double	r;
	double g;
	double b;
}MY_COLOR;

MY_COLOR color = { 0,0,0 };
MY_COLOR color2 = { 0,0,0 };
void incScore();

void init()
{
	int d;
	//  R   G    B
	glClearColor(0.3, 0.3, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

}
void DrawCircle(int num_pts, MY_COLOR color) {
	double alpha;
	double teta = 2 * PI / num_pts;
	double x, y;
	glColor3d(color.r, color.g, color.b);
	glBegin(GL_POLYGON);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);

	}
	glEnd();


}




void drawParabula(double a,double offsetX,double c,double x1,double x2) {
	double x, y;
	glBegin(GL_LINE_STRIP);
	for (x = x1; x <= x2; x += 0.01)
	{
		y = a * pow(x-offsetX, 2) + c;
		glVertex2d(x, y);
	}
	glEnd();
}
void drawParabula1() {
	double x, y;
	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (x = -0.75; x <= 0.75; x += 0.01)
	{
		y = -0.5 * pow(x, 2) - 0.4;
		glVertex2d(x, y);
	}
	glEnd();
}





void drawScore(char score[]) {
	//char welcomeMessage[] = score;
	int len = strlen(score);
	

	glColor3d(0.4, 0.2, 0.6); // Set the text color to white


	// Get window dimensions
	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// Calculate the position of the text in the top left corner
	float aspectRatio = (float)windowWidth / (float)windowHeight;
	float textWidth = len * 13.0; // Adjust the value (13.0) as needed to control the text width

	float x = -1.0 + (0.05 * aspectRatio); // 0.05 is a small offset from the left edge
	float y = 1.0 - (0.1 * aspectRatio);  // 0.1 is a small 


	glRasterPos2d(x, y);

	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);
	}
}
// put all drawings here
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer


	
	//court

	
	glColor3d(0.8, 0.3, 0.3);
	glBegin(GL_POLYGON); //main court
	glVertex2d(1, -0.2);
	glVertex2d(-0.5, -0.2);
	glVertex2d(-1, -1);
	glVertex2d(1, -1);
	glEnd();

	glColor3d(1,0.8,0.6);
	glBegin(GL_POLYGON); //left wall
	glVertex2d(1, -0.2);
	glVertex2d(-0.5, -0.2);
	glVertex2d(-0.5, 1);
	glVertex2d(1, 1);
	glEnd();

	glColor3d(1, 0.8, 0.6); //back wall
	glBegin(GL_POLYGON);
	glVertex2d(-0.5, -0.2);
	glVertex2d(-0.5, 1);
	glVertex2d(-1, 1);
	glVertex2d(-1, -1);
	glEnd();

	glColor3d(1, 1, 1);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex3d(-0.7, -0.5, 0);
	glVertex3d(0.5, -0.5, 0);

	glVertex3d(-0.9, -0.8, 0);
	glVertex3d(0.3, -0.8, 0);

	glVertex3d(0.3, -0.8, 0);
	glVertex3d(0.3, -1, 0);
	

	glEnd();

	glPushMatrix();
	glTranslated(0.5, -0.5, 0);
	glColor3d(1, 1, 1);
	drawParabula(-1.5, 0, 0,0,1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.3, -0.8, 0);
	glColor3d(1, 1, 1);
	drawParabula(-4, 0, 0, 0, 1);
	glPopMatrix();

	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(1, -0.2); //horizontal line
	glVertex2d(-0.5, -0.2);

	glVertex2d(-0.5, -0.2); 
	glVertex2d(-1, -1);

	glVertex2d(-0.5, -0.2);
	glVertex2d(-0.5, 1);
	glEnd();

	
	

	//draw ball
	glPushMatrix();
	glTranslated(ballX, ballY, 0);
	glRotated(rotationAngle, 0.0, 0.0, 1.0);
	glScaled(0.05, 0.05, 1);

	color.r = 0.8;
	color.g = 0.4;
	color.b = 0.1;

	DrawCircle(360, color);
	glColor3d(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2d(-1, 0);
	glVertex2d(1, 0);

	glVertex2d(0, -1);
	glVertex2d(0, 1);
	glEnd();
	drawParabula(0.5,0,0.4,-0.75,0.75);
	drawParabula( -0.5,0, - 0.4,-0.75,0.75);
	glPopMatrix();



	//draw basket


	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON); //main tube
	glVertex2d(-0.9, 0.3);
	glVertex2d(-0.85, 0.3);
	glVertex2d(-0.85, -1);
	glVertex2d(-0.9, -1);
	glEnd();

	glBegin(GL_POLYGON); //angle
	glVertex2d(-0.9, 0.3);
	glVertex2d(-0.85, 0.3);
	glVertex2d(-0.70, 0.4);
	glVertex2d(-0.75, 0.4);
	glEnd();

	glBegin(GL_POLYGON); //board
	glVertex2d(-0.75, 0.2);
	glVertex2d(-0.7, 0.2);
	glVertex2d(-0.7, 0.5);
	glVertex2d(-0.75, 0.5);
	glEnd();

	glColor3d(1, 0, 0); //ring
	glBegin(GL_POLYGON);
	glVertex2d(-0.7, 0.25);
	glVertex2d(-0.4, 0.25);
	glVertex2d(-0.4, 0.27);
	glVertex2d(-0.7, 0.27);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(-0.7, 0.2);
	glVertex2d(-0.7, 0.25);
	glVertex2d(-0.65, 0.25);
	glEnd();

	glColor4d(1.0, 1.0, 1.0, 0.5); // Adjust the alpha value as needed
	glBegin(GL_POLYGON); //net
	glVertex2d(-0.65, 0.25);
	glVertex2d(-0.4, 0.25);
	glVertex2d(-0.47, 0.09);
	glVertex2d(-0.58, 0.09);
	glEnd();

	drawScore(score);
	glutSwapBuffers(); // show all


}
void hitLeft(double x) {
	startingX = x;
	startingY = ballY;
	Vx = -0.5 * Vx;
	Vy = 0.5 * (Vy + 2 * a * t);
	t = 0;
}
void hitRight(double x) {
	startingX = x;
	startingY = ballY;
	Vx = -0.5 * Vx;
	Vy = 0.5 * (Vy + 2 * a * t);
	t = 0;
}
void hitFloor(double y) {
	startingX = ballX;
	startingY = y;
	Vy = -0.5 * (Vy + 2 * a * t);
	Vx = 0.999 * Vx;
	t = 0;
}
void hitRing() {
	startingX = ballX;
	startingY = ballY;
	Vx = 0.8 * Vx;
	Vy = -0.8 * (Vy + 2 * a * t);
	t = 0;

}
void incScore() {
	int scoreInt= atoi(score);
	if (++scoreInt<10)
	{
		score[1] = scoreInt+'0';
		drawScore(score);
		overRing = false;

	}
	else
	{
		char unit;
		char ten;
		unit = scoreInt % 10+'0';
		ten = scoreInt / 10+'0';
		

		score[0] =ten;
		score[1] = unit;
		score[2] = '\0';
		drawScore(score);
		overRing = false;

	} 
	
	
}
bool wentThroughRing() {
	if ((ballX >= -0.65 && ballX <= -0.37 && ballY <= 0.251 && ballY >= 0.25))
		return true;
	else
		return false;
}

void idle()
{
	glutPostRedisplay(); // calls indirectly display
	isMoving = (sqrt(Vx * Vx + Vy * Vy) > speedThreshold);

	if (isMoving && Vx < 0) {

		offset2 += 0.00000000000000000000000000000000000000000000000000001;
		rotationAngle += offset2;
	}

	if (isMoving && Vx > 0) {
		offset2 -= 0.000000000000000000000000000000000000000000000000000001;
		rotationAngle -= offset2;
	}

	if (flag)
	{

		t += 0.001;
		
	}

	if (ballY >= 0.3 &&hitFloorOnce) //hit floor once is initiatted as true , after one time set to false  and will set back to true after hitting the ground
		//checking if ball is over the ring for score
	{
		overRing = true;
		hitFloorOnce = false;
	}
	else if (ballY<0.2)
	{
		overRing = false;
		hitFloorOnce = true;
	}

	if (overRing&&wentThroughRing())
	{
		incScore();

	}

	

	ballX = startingX + Vx * t;
	ballY = startingY + Vy * t + a * pow(t, 2);

	if (( - 0.43 <= ballX && ballX <= -0.37 && ballY >= 0.27 && ballY <= 0.3)||(-0.8 <= ballX && ballX <= -0.6 && ballY >= 0.27 && ballY <= 0.3)) //top of ring
	{
		hitFloor(0.3);
	}
	 if (-0.43 <= ballX && ballX <= -0.37 && ballY >= 0.23 && ballY <= 0.29) //right end of ring
	{

		hitLeft(-0.36);

	}
	 if (-0.76 <= ballX && ballX <= -0.69 && ballY >= 0.19 && ballY <= 0.51) //board
	 {
		 hitLeft(-0.68);

	 }
	 if (-0.81 <= ballX && ballX <= -0.74 && ballY >= 0.19 && ballY <= 0.51) //board
	 {
		 hitRight(-0.82);

	 }

	if (ballX <= -0.96) //left wall
	{
		hitLeft(-0.96);

	}

	 if (ballX >= 0.96) //right wall
	{
		hitRight(0.96);

	}

	 if (ballY <= -0.96) //floor
	{
		 hitFloorOnce = true;
		hitFloor(-0.96);

	}

	
}

// click callback function
// NB: origin is in the left top corner
double distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));

}
void MouseClick(int button, int state, int x, int y)
{
	double F;
	double alpha, deltaX, deltaY;
	double normallizeX, normallizeY;


	normallizeX = x / 300.0 - 1;
	normallizeY = (HEIGHT - y) / 300.0 - 1;

	startingX = ballX;
	startingY = ballY;
	t = 0;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		F = distance(normallizeX, normallizeY, ballX, ballY);
		deltaX = ballX - normallizeX;
		deltaY = ballY - normallizeY;
		alpha = atan2(deltaY, deltaX);
		Vy = -1 *F* sin(alpha);
		Vx = -1 *F* cos(alpha);
		offset2 = 0.2;
		a = -0.4;
		flag = true;
		isMoving = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//startingX = 0.5;
	//	startingY = 0;
		flag = false;
		rotationAngle = 0;
		isMoving = false;
		Vx = 0;
		Vy = 0;
		t = 0;

	}
}



void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("2D graphics Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(MouseClick);
	init();
	glutMainLoop();

}