#define FREEGLUT_STATIC   
#include <GL/freeglut.h>  
#include <math.h>
#include <cmath>
#define MAX_CHAR 128

//There are some consts and variables for the first stage
const int starsNumber = 11;
const float angleRadian = 3.1415926 / 180.0f;
float starsRotationAngle[starsNumber] = { 0.0f, 36.0f, 72.0f, 18.0f, 36.0f, 20.0f, 18.0f, 18.0f, 36.0f, 20.0f, 18.0f };
float starsPositionChange[starsNumber] = { 400.0f , 600.0f, 700.0f, 500.0f, 550.0f, 600.0f, 280.0f, 400.0f, 200.0f, 300.0f, 250.0f };
float starsHeights[starsNumber] = { 200.0f, 300.0f, 262.5f, 187.5f, 240.625f, 562.5f, 262.5f, 500.0f, 250.0f, 562.5f, 468.75f };
float kirbyStarPositionX = 800.0f;
float kirbyStarPositionY = 600.0f;

//There are some consts and variables for the second stage
//The shining stars animation will also be used in the third stage
float kirbyDistance = 0.0f;
float shiningStars = 0.0f;
float shiningTime = 0.0f;
bool shining = true;

//There are some consts and variables for the third stage
float cakeX = 447;
float cakeY = 300;
bool isDraggingCake = false;
float kirbyStickersX = 530;
float kirbyStickersY = 300;
bool kirbyCakeStickers = false;

//These variables are used to record the position of the mouse
int mouseX = 0;
int mouseY = 0;

//These variables are used for transtion animation
int transitionState = 0;
float transitionTime = 0.0f;
float transitionBackTime = 0.0f;

//These variables are used for showing the text
float stayTime = 0.0f;
bool textShown = true;

//It is used to change the stage
int stages = 0;

//This function is used for update the position of the mouse to draw the star following the mouse
//It also records the position when drag the cake in the third stage
void mouseMove(int x, int y) {
	mouseX = x;
	mouseY = 600 - y;
	if (isDraggingCake)
	{
		cakeX = x;
		cakeY = 600 - y;
		if (abs(cakeX - kirbyStickersX) < 10 && abs(cakeY - kirbyStickersY) < 10)
		{
			kirbyCakeStickers = true;
		}
	}
}

//This function is used to creat a new font and this font will be used with the function "drawString" to create the strings on the screen
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//This function is used to render the string on the screen using the font made by the function "selectFont"
void drawString(const char* str) {
	static GLuint lists;
	lists = glGenLists(MAX_CHAR);
	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}

//It change the variable "stages' to change the stage which should between the first transition animation and the second transition animation
//This function will be called when the first transition animation is over which is in the function "transitionBackStart"
//It also change the variable for the shining stars animation for the third stage
void stagesChange(void)
{
	stages += 1;

	stayTime = 0.0f;

	if (stages == 2)
	{
		shining = true;
		shiningStars = 0.0f;
	}
}

//It change the variable "transitionState' to change the transition animation
//There are 3 value for the variable "transitionState'
//value 0 means no transition animation
//value 1 means the first transition animation
//value 2 means the second transition animation
void transitionStatesChange(void)
{
	//To avoid skipping the second transition animation
	if (transitionState == 0)
	{
		transitionState += 1;
	}
}

//Update some values for the animation
void update(int value)
{
	//It change the value for showing the texts;
	stayTime += 0.2f;

	//It change the values for stars moving and rotation which will be used for all the stages
	kirbyStarPositionX -= 1.2f;
	kirbyStarPositionY -= 0.9f;
	for (int i = 0; i < starsNumber; i++) {
		starsRotationAngle[i] += 0.05f;
	}
	for (int i = 0; i < starsNumber; i++) {
		starsPositionChange[i] -= 0.4f;
		starsHeights[i] -= 0.3f;
		if (starsPositionChange[i] < 0 || starsHeights[i] < 0) {
			if (i < 2) {
				starsPositionChange[i] = 800;
				starsHeights[i] = 400;
			}
			else if (i < 4) {
				starsPositionChange[i] = 800;
				starsHeights[i] = 300;
			}
			else if (i < 5) {
				starsPositionChange[i] = 800;
				starsHeights[i] = 350;
			}
			else if (i < 7) {
				starsPositionChange[i] = 640;
				starsHeights[i] = 600;
			}
			else if (i < 9) {
				starsPositionChange[i] = 480;
				starsHeights[i] = 600;
			}
			else if (i < 11) {
				starsPositionChange[i] = 320;
				starsHeights[i] = 600;
			}
		}
	}

	//It change the values for stars shining for the second stage
	//This animation will control the stars shown to their biggest size and then narrow to unseen
	if (stages == 1)
	{
		if (shiningStars < 18.0f)
		{
			shiningStars += 0.2f;
		}
		else
		{
			shiningTime += 0.2f;
		}
		if (shiningTime >= 18.0f)
		{
			shiningStars -= 0.4f;
		}
		if (shiningStars < 0)
		{
			shining = false;
		}
	}

	//It change the values for stars shining for the third stage
	//This animation will control the stars shining which alternates between enlargement and contraction
	if (stages == 2)
	{
		if (shining)
		{
			shiningStars += 0.2f;
		}
		else
		{
			shiningStars -= 0.2f;
		}
		if (shiningStars <= 0)
		{
			shining = true;
		}
		if (shiningStars > 2)
		{
			shining = false;
		}
	}

	//It change the time for showing the transition animations
	if (transitionState == 1) {
		transitionTime += 1.0f;
	}
	if (transitionState == 2) {
		transitionBackTime += 1.0f;
	}

	glutPostRedisplay();
	//Set 60fps
	glutTimerFunc(1000 / 60, update, 0);
}

//The follwing functions are the basic functions for drawing the elements in this project
//This function is used to draw a hollow circle with a width of "lineWidth" and a radius of "r" at positions "x0" and "y0"
void hollowCircle(float x0, float y0, float r, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float theta = angleRadian * float(i);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(x0 + x, y0 + y);
	}
	glEnd();
}

//This function is used to draw a not whole hollow circle with a width of "lineWidth" and radius of "r" at positions "x0" and "y0", it will beign with "angleStart" and end with "angleEnd"
void holidCircleAngle(float x0, float y0, float r, int angleIStart, int angleEnd, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	for (int i = angleIStart; i < angleEnd; i++) {
		float theta = angleRadian * float(i);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(x0 + x, y0 + y);
	}
	glEnd();
}

//This function is used to draw a solid circle with a radius of "r" at positions "x0" and "y0"
void solidCircle(float x0, float y0, float r)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float theta = angleRadian * float(i);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(x0 + x, y0 + y);
	}
	glEnd();
}

//This function is used to draw a hollow circle with a width of "lineWidth" ,"a" major axis, and "b" minor axis at positions "x0" and "y0"
void hollowEllipse(float x0, float y0, float a, float b, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float theta = angleRadian * float(i);
		glVertex2f(x0 + a * cosf(theta), y0 + b * sinf(theta));
	}
	glEnd();
}

//This function is used to draw a hollow circle with a width of "lineWidth" ,"a" major axis, and "b" minor axis at positions "x0" and "y0" and it will rotate for "angle" degrees
void hollowEllipseOblique(float x0, float y0, float a, float b, int angle, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float theta = angleRadian * float(i);
		float omiga = angleRadian * float(angle);
		glVertex2f(x0 + a * cosf(theta) * cosf(omiga) - b * sinf(theta) * sinf(omiga), y0 + b * sinf(theta) * cos(omiga) + a * cosf(theta) * sinf(omiga));
	}
	glEnd();
}

//This function is used to draw a solid circle with "a" major axis, and "b" minor axis at positions "x0" and "y0"
void solidEllipse(float x0, float y0, float a, float b)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float theta = angleRadian * float(i);
		glVertex2f(x0 + a * cosf(theta), y0 + b * sinf(theta));
	}
	glEnd();
}

//This function is used to draw a solid circle with "a" major axis, and "b" minor axis at positions "x0" and "y0", it will beign with "angleStart" and end with "angleEnd"
void solidEllipseAngle(float x0, float y0, float a, float b, int angleStart, int angleEnd)
{
	glBegin(GL_POLYGON);
	for (int i = angleStart; i < angleEnd; i++) {
		float theta = angleRadian * float(i);
		glVertex2f(x0 + a * cosf(theta), y0 + b * sinf(theta));
	}
	glEnd();
}

//This function is used to draw a solid circle with "a" major axis, and "b" minor axis at positions "x0" and "y0" and it will rotate for "angle" degrees
void solidEllipseOblique(float x0, float y0, float a, float b, int angle)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float theta = angleRadian * float(i);
		float omiga = angleRadian * float(angle);
		glVertex2f(x0 + a * cosf(theta) * cosf(omiga) - b * sinf(theta) * sinf(omiga), y0 + b * sinf(theta) * cos(omiga) + a * cosf(theta) * sinf(omiga));
	}
	glEnd();
}

//This function is used to draw a line between the points ("x0" , "y0") and ("x1" , "y1") with a width of "lineWidth"
void line(float x0, float y0, float x1, float y1, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glEnd();
}

//This function is used to draw a rectangle with four points ("x0" , "y0"),("x1" , "y1"),("x2" , "y2") and ("x3" , "y3")
void rectangle(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	glBegin(GL_QUADS);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
}

//This function is used to draw a triangle with three points ("x0" , "y0"),("x1" , "y1")and("x2" , "y2")
void triangle(float x0, float y0, float x1, float y1, float x2, float y2)
{
	glBegin(GL_TRIANGLES);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

//Now use the previous basic functions to draw some components for the elements in this project
//This function is used to draw the eyes of Kirby
void kirbyEyes(float x0, float y0)
{
	solidEllipse(x0 - 12, y0 + 22, 7, 14);
	solidEllipse(x0 + 12, y0 + 22, 7, 14);

	glColor3f(1, 1, 1);
	solidEllipse(x0 - 12, y0 + 29, 4, 6);
	solidEllipse(x0 + 12, y0 + 29, 4, 6);
}

//This function is used to draw the body of Kirby's stickers
void kirbyEyesStickers(float x0, float y0)
{
	glColor3f(54 / 255.0f, 137 / 255.0f, 191 / 255.0f);
	solidEllipse(x0 - 8, y0 + 8, 4, 10);
	solidEllipse(x0 + 8, y0 + 8, 4, 10);

	glColor3f(41 / 255.0f, 35 / 255.0f, 142 / 255.0f);
	solidEllipse(x0 - 8, y0 + 11, 3, 7);
	solidEllipse(x0 + 8, y0 + 11, 3, 7);

	glColor3f(1, 1, 1);
	solidEllipse(x0 - 8, y0 + 14, 2, 4);
	solidEllipse(x0 + 8, y0 + 14, 2, 4);
}   

//This function is used to draw the face of Kirby
void kirbyFace(float x0, float y0)
{
	glColor3f(246 / 255.0f, 130 / 255.0f, 156 / 255.0f);
	solidEllipse(x0 - 22, y0 + 5, 7, 5);
	solidEllipse(x0 + 22, y0 + 5, 7, 5);
}

//This function is used to draw the face of Kirby's stickers
void kirbyFaceStickers(float x0, float y0)
{
	glColor3f(247 / 255.0f, 107 / 255.0f, 149 / 255.0f);
	solidEllipse(x0 - 20, y0, 5, 3);
	solidEllipse(x0 + 20, y0, 5, 3);
}

//This function is used to draw the mouth of Kirby
void kirbyMouth(float x0, float y0)
{
	glColor3f(128 / 255.0f, 94 / 255.0f, 93 / 255.0f);
	solidEllipse(x0, y0 - 5, 10, 3);
	solidEllipseAngle(x0, y0 - 5, 10, 12, 180, 360);
	glColor3f(246 / 255.0f, 130 / 255.0f, 156 / 255.0f);
	solidEllipse(x0, y0 - 8, 8, 3);
	solidEllipseAngle(x0, y0 - 8, 8, 8, 180, 360);
}

//This function is used to draw the mouth of Kirby's stickers
void kirbyMouthShouting(float x0, float y0)
{
	glColor3f(133 / 255.0f, 32 / 255.0f, 78 / 255.0f);
	solidCircle(x0, y0 - 8, 3);
}

//This function is used to draw the hands of Kirby
void kirbyHands(float x0, float y0)
{
	glColor3f(254 / 255.0f, 184 / 255.0f, 196 / 255.0f);
	solidEllipseOblique(x0 + 45 * cosf(40.0f * 3.1415926 / 180.0f), y0 + 45 * sinf(40.0f * 3.1415926 / 180.0f), 20, 15, 45);
	solidEllipseOblique(x0 - 45, y0, 20, 15, 15);
	glColor3f(128 / 255.0f, 94 / 255.0f, 93 / 255.0f);
	hollowEllipseOblique(x0 + 45 * cosf(40.0f * 3.1415926 / 180.0f), y0 + 45 * sinf(40.0f * 3.1415926 / 180.0f), 20, 15, 45, 2);
	hollowEllipseOblique(x0 - 45, y0, 20, 15, 15, 2);
}

//This function is used to draw the connections between the body and the hands for Kirby
void kirbyHandsConnections(float x0, float y0)
{
	glColor3f(254 / 255.0f, 184 / 255.0f, 196 / 255.0f);
	solidCircle(x0 + 50 * cosf(40.0f * 3.1415926 / 180.0f), y0 + 50 * sinf(40.0f * 3.1415926 / 180.0f), 8);
	solidCircle(x0 - 50, y0, 8);
}

//This function is used to draw the right foot of Kirby
void kirbyRightFoot(float x0, float y0)
{
	glColor3f(228 / 255.0f, 95 / 255.0f, 138 / 255.0f);
	solidEllipse(x0 + 25, y0 - 40, 30, 20);
	glColor3f(128 / 255.0f, 94 / 255.0f, 93 / 255.0f);
	hollowEllipse(x0 + 25, y0 - 40, 30, 20, 2);
	glColor3f(247 / 255.0f, 129 / 255.0f, 160 / 255.0f);
	solidCircle(x0 + 50, y0 - 40, 3);
}

//This function is used to draw the left foot of Kirby
void kirbyLeftFoot(float x0, float y0)
{
	glColor3f(228 / 255.0f, 95 / 255.0f, 138 / 255.0f);
	solidEllipseOblique(x0 - 50 * sinf(45.0f * 3.1415926 / 180.0f), y0 - 50 * cosf(45.0f * 3.1415926 / 180.0f), 30, 20, 315);
	glColor3f(128 / 255.0f, 94 / 255.0f, 93 / 255.0f);
	hollowEllipseOblique(x0 - 50 * sinf(45.0f * 3.1415926 / 180.0f), y0 - 50 * cosf(45.0f * 3.1415926 / 180.0f), 30, 20, 315, 2);
	glColor3f(247 / 255.0f, 129 / 255.0f, 160 / 255.0f);
	solidCircle(x0 - 50 * cosf(20.0f * 3.1415926 / 180.0f), y0 - 50 * sinf(20.0f * 3.1415926 / 180.0f), 3);
}

//Using the previous functions to draw a size of "size" Kirby at positions "x0" and "y0"
void kirby(float x0, float y0, float size)
{
	kirbyRightFoot(x0, y0);
	kirbyHands(x0, y0);
	glColor3f(254 / 255.0f, 184 / 255.0f, 196 / 255.0f);
	solidCircle(x0, y0, size);
	glColor3f(128 / 255.0f, 94 / 255.0f, 93 / 255.0f);
	hollowCircle(x0, y0, size, 2);
	kirbyEyes(x0, y0);
	kirbyHandsConnections(x0, y0);
	kirbyFace(x0, y0);
	kirbyMouth(x0, y0);
	kirbyLeftFoot(x0, y0);
}

//Using the previous functions to draw a size of "size" Kirby's stickers at positions "x0" and "y0"
void kirbyStickers(float x0, float y0, float size)
{
	glColor3f(245 / 255.0f, 146 / 255.0f, 191 / 255.0f);
	solidCircle(x0, y0, size + 5);
	glColor3f(255 / 255.0f, 174 / 255.0f, 215 / 255.0f);
	solidCircle(x0, y0, size);
	glColor3f(133 / 255.0f, 32 / 255.0f, 78 / 255.0f);
	hollowCircle(x0, y0, size, 2);
	kirbyEyesStickers(x0, y0);
	kirbyFaceStickers(x0, y0);
	
	//If the mouse is higher than Kirby, Kirby will laught
	//If the mouse is lower than Kirby, Kirby will shout
	if (mouseX < 800 && mouseY > 300) {
		kirbyMouth(x0, y0 - 5);
	}
	else {
		kirbyMouthShouting(x0, y0);
	}
}

//Using the previous functions to draw a size of "R" star at positions "x0" and "y0"
void stars(float x0, float y0, float R)
{
	float r = R / (sinf(36.0f * 3.1415926 / 180.0f) * cosf(18.0f * 3.1415926 / 180.0f) / sinf(18.0f * 3.1415926 / 180.0f) + cosf(36.0f * 3.1415926 / 180.0f));
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x0, y0);
	for (int i = 0; i < 5; i++) {
		float theta = i * 72.0f * 3.1415926 / 180.0f;
		glVertex2f(x0 + R * sinf(theta), y0 + R * cosf(theta));
		glVertex2f(x0 + r * sinf(theta + 36.0f), y0 + r * cosf(theta + 36.0f));
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x0, y0);
	for (int i = 0; i < 5; i++) {
		float theta = i * 72.0f * 3.1415926 / 180.0f;
		glVertex2f(x0 + R * sinf(theta), y0 + R * cosf(theta));
		glVertex2f(x0 + r * sinf(theta - 36.0f), y0 + r * cosf(theta - 36.0f));
	}
	glEnd();
}

//Using the previous functions to draw a size of "R" round star at positions "x0" and "y0"
void roundStars(float x0, float y0, float R)
{
	float r = R / (sinf(36.0f * 3.1415926 / 180.0f) * cosf(18.0f * 3.1415926 / 180.0f) / sinf(18.0f * 3.1415926 / 180.0f) + cosf(36.0f * 3.1415926 / 180.0f));
	for (int i = 0; i < 5; i++) {
		float theta = 90.0f - 72.0f * i;
		float omiga = i * 72.0f * 3.1415926 / 180.0f;
		solidEllipseOblique(x0 + R / 2 * sinf(omiga), y0 + R / 2 * cosf(omiga), R / 1.8, r, theta);
	}
}

//Using the precious functions and variable to draw a size of "R" round star at positions "x0" and "y0" and it will rotate for "obliqueAngle" degrees
void movingStars(float x0, float y0, float R, float obliqueAngle)
{
	float r = R / (sinf(36.0f * 3.1415926 / 180.0f) * cosf(18.0f * 3.1415926 / 180.0f) / sinf(18.0f * 3.1415926 / 180.0f) + cosf(36.0f * 3.1415926 / 180.0f));
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x0, y0);
	for (int i = 0; i < 5; i++) {
		float theta = i * 72.0f * 3.1415926 / 180.0f + obliqueAngle;
		glVertex2f(x0 + R * sinf(theta), y0 + R * cosf(theta));
		glVertex2f(x0 + r * sinf(theta + 36.0f), y0 + r * cosf(theta + 36.0f));
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x0, y0);
	for (int i = 0; i < 5; i++) {
		float theta = i * 72.0f * 3.1415926 / 180.0f + obliqueAngle;
		glVertex2f(x0 + R * sinf(theta), y0 + R * cosf(theta));
		glVertex2f(x0 + r * sinf(theta - 36.0f), y0 + r * cosf(theta - 36.0f));
	}
	glEnd();
}

//Using the previous functions to draw a size of "size" round flower at positions "x0" and "y0"
void flower(float x0, float y0, float size)
{
	float R = size / 4;
	float r = size / 10;

	glColor3f(193 / 255.0f, 215 / 255.0f, 107 / 255.0f);
	line(x0, y0, x0, y0 + size - r, 2);
	solidEllipseOblique(x0 + size / 8, y0 + size / 8, size / 5, size / 9, 45);
	solidEllipseOblique(x0 - size / 8, y0 + size / 2, size / 5, size / 9, -45);
	glColor3f(254 / 255.0f, 174 / 255.0f, 174 / 255.0f);
	roundStars(x0, y0 + size, R);
	glColor3f(255 / 255.0f, 249 / 255.0f, 215 / 255.0f);
	solidCircle(x0, y0 + size, r);
	glColor3f(193 / 255.0f, 215 / 255.0f, 107 / 255.0f);
}

//Using the previous functions to draw a cake at positions "x0" and "y0"
void cake(float x0, float y0)
{
	glColor3f(144 / 255.0f, 57 / 255.0f, 44 / 255.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x0 - 25, y0 + 10);
	glVertex2f(x0 - 15, y0 - 40);
	glVertex2f(x0 + 15, y0 - 40);
	glVertex2f(x0 + 25, y0 + 10);
	glEnd();

	glColor3f(1, 1, 1);
	solidCircle(x0 - 10, y0 + 25, 15);
	solidCircle(x0 + 10, y0 + 25, 15);
	solidCircle(x0, y0 + 10, 15);
	solidCircle(x0 - 25, y0 + 10, 15);
	solidCircle(x0 + 25, y0 + 10, 15);

	glColor3f(0, 0, 0);
	holidCircleAngle(x0 - 10, y0 + 25, 15, 90, 180, 2);
	holidCircleAngle(x0 + 10, y0 + 25, 15, 0, 90, 2);

	holidCircleAngle(x0 - 25, y0 + 10, 15, 90, 330, 2);
	holidCircleAngle(x0, y0 + 10, 15, 210, 330, 2);
	holidCircleAngle(x0 + 25, y0 + 10, 15, 210, 450, 2);

	glColor3f(187 / 255.0f, 2 / 255.0f, 19 / 255.0f);
	solidCircle(x0, y0 + 40, 12);

}

//Using the previous functions to draw a birthday card
void bithdayCard(void)
{
	glColor3f(228 / 255.0f, 224 / 255.0f, 199 / 255.0f);
	rectangle(150, 150, 650, 150, 650, 450, 150, 450);

	glColor3f(225 / 255.0f, 71 / 255.0f, 51 / 255.0f);
	rectangle(198, 198, 602, 198, 602, 402, 198, 402);

	glColor3f(223 / 255.0f, 224 / 255.0f, 218 / 255.0f);
	rectangle(200, 200, 600, 200, 600, 400, 200, 400);

	glColor3f(0, 0, 0);
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");
	glRasterPos2f(220, 320);
	drawString("Happy");
	glRasterPos2f(260, 280);
	drawString("Birthday!");

	//If the cake is closed to the Kirby's sticker,Kirby will become to cake Kirby
	if (kirbyCakeStickers == false)
	{
		kirbyStickers(kirbyStickersX, kirbyStickersY, 35);
		cake(cakeX, cakeY);
	}
	else
	{
		cake(kirbyStickersX, kirbyStickersY);
		kirbyEyesStickers(kirbyStickersX, kirbyStickersY + 5);
		kirbyFaceStickers(kirbyStickersX, kirbyStickersY + 5);
		kirbyMouth(kirbyStickersX, kirbyStickersY - 5);

		glColor3f(253 / 255.0f, 199 / 255.0f, 53 / 255.0f);
		roundStars(kirbyStickersX - 40, kirbyStickersY + 40, 10 + shiningStars);
		roundStars(kirbyStickersX + 40, kirbyStickersY + 40, 10 + shiningStars);
		roundStars(kirbyStickersX - 40, kirbyStickersY - 40, 10 + shiningStars);
		roundStars(kirbyStickersX + 40, kirbyStickersY - 40, 10 + shiningStars);
		glColor3f(255 / 255.0f, 240 / 255.0f, 121 / 255.0f);
		roundStars(kirbyStickersX - 40, kirbyStickersY + 40, 10);
		roundStars(kirbyStickersX + 40, kirbyStickersY + 40, 10);
		roundStars(kirbyStickersX - 40, kirbyStickersY - 40, 10);
		roundStars(kirbyStickersX + 40, kirbyStickersY - 40, 10);
	}


}

//Using the previous functions and variables to draw a star following the mouse
void mouseStar(float x0, float y0)
{
	glPushMatrix();
	glTranslatef(x0, y0, 0);
	glScalef(1, 1, 0);
	glRotatef(30, 0, 0, 1);
	glTranslatef(-x0, -y0, 0);
	glColor3f(255 / 255.0f, 240 / 255.0f, 121 / 255.0f);
	roundStars(x0, y0, 12);
	glPopMatrix();
}

//Using the "movingStars" to draw the scene of the moving stars and use the "triangle" to draw the tail of the moving stars
void movingStarsScne(void)
{
	for (int i = 0; i < starsNumber; i++)
	{
		glColor3f(253 / 255.0f, 199 / 255.0f, 53 / 255.0f);
		movingStars(starsPositionChange[i], starsHeights[i], 20, starsRotationAngle[i]);
		glColor3f(255 / 255.0f, 240 / 255.0f, 121 / 255.0f);
		movingStars(starsPositionChange[i], starsHeights[i], 18, starsRotationAngle[i]);
		triangle(starsPositionChange[i] + 30 * cosf(36.87f * angleRadian), starsHeights[i] + 30 * sinf(36.87f * angleRadian), starsPositionChange[i] + 4, starsHeights[i] - 5, starsPositionChange[i] - 4, starsHeights[i] + 5);
	}
}

//Using the "solidCircle" to draw the scene of the static stars
void starsScene(void)
{
	glColor3f(255 / 255.0f, 240 / 255.0f, 121 / 255.0f);
	solidCircle(50, 50, 2);
	solidCircle(200, 100, 2);
	solidCircle(700, 500, 2);
	solidCircle(400, 120, 2);
	solidCircle(750, 420, 2);
	solidCircle(310, 480, 2);
	solidCircle(70, 350, 2);
	solidCircle(40, 440, 2);
	solidCircle(510, 520, 2);
	solidCircle(730, 70, 2);
	solidCircle(36, 530, 2);
	solidCircle(500, 120, 2);
	solidCircle(270, 510, 2);
}

//Using the "rectangles" to draw the scene of the grass
void grassScene(void)
{
	glColor3f(136 / 255.0f, 200 / 255.0f, 121 / 255.0f);
	rectangle(0, 0, 800, 0, 800, 300, 0, 300);
	glColor3f(78 / 255.0f, 121 / 255.0f, 88 / 255.0f);
	rectangle(0, 298, 800, 298, 800, 300, 0, 300);
	glColor3f(138 / 255.0f, 190 / 255.0f, 117 / 255.0f);
	rectangle(0, 298, 800, 298, 800, 295, 0, 295);
	glColor3f(138 / 255.0f, 192 / 255.0f, 118 / 255.0f);
	rectangle(0, 290, 800, 290, 800, 295, 0, 295);
	glColor3f(138 / 255.0f, 194 / 255.0f, 119 / 255.0f);;
	rectangle(0, 290, 800, 290, 800, 280, 0, 280);
	glColor3f(138 / 255.0f, 195 / 255.0f, 120 / 255.0f);
	rectangle(0, 270, 800, 270, 800, 280, 0, 280);
	glColor3f(137 / 255.0f, 195 / 255.0f, 120 / 255.0f);
	rectangle(0, 270, 800, 270, 800, 260, 0, 260);
	glColor3f(136 / 255.0f, 195 / 255.0f, 120 / 255.0f);
	rectangle(0, 250, 800, 250, 800, 260, 0, 260);
	glColor3f(136 / 255.0f, 200 / 255.0f, 120 / 255.0f);
	rectangle(0, 250, 800, 250, 800, 240, 0, 240);
}

//This function shift the "transitionState" to start the first transition animation
void transitionStart(void)
{
	transitionState = 1;
	transitionTime = 0;
	transitionBackTime = 0;
}

//This function shift the "transitionState" to start the second transition animation
//Before showing the second transition animation,it changes the stages to load the next stages in advance
void transitionBackStart(void)
{
	stagesChange();
	transitionState = 2;
	transitionTime = 0;
	transitionBackTime = 0;
}

//This function is the first transmition animation
//Parameters "speed" and "transitionTime" are used to control the animation time
void starTransition(float speed, float transitionTime)
{
	float changeLength = speed * transitionTime;
	float R = 300 - changeLength;
	float r = R / (sinf(36.0f * 3.1415926 / 180.0f) * cosf(18.0f * 3.1415926 / 180.0f) / sinf(18.0f * 3.1415926 / 180.0f) + cosf(36.0f * 3.1415926 / 180.0f));

	glColor3f(0.0, 0.0, 0.0);
		
	glBegin(GL_POLYGON);
	glVertex2f(400 - r * sinf(36.0f * angleRadian), 300 + r * cosf(36.0f * angleRadian));
	glVertex2f(400, 300 + R);
	glVertex2f(400, 600);
	glVertex2f(0, 600);
	glVertex2f(400 - R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(400 + r * sinf(36.0f * angleRadian), 300 + r * cosf(36.0f * angleRadian));
	glVertex2f(400, 300 + R);
	glVertex2f(400, 600);
	glVertex2f(800, 600);
	glVertex2f(400 + R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(400, 300 - r);
	glVertex2f(400 + R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glVertex2f(800, 0);
	glVertex2f(0, 0);
	glVertex2f(400 - R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(400 + r * cosf(18.0f * angleRadian), 300 - r * sinf(18.0f * angleRadian));
	glVertex2f(400 + R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glVertex2f(800, 600);
	glVertex2f(800, 0);
	glVertex2f(400 + R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(400 - r * cosf(18.0f * angleRadian), 300 - r * sinf(18.0f * angleRadian));
	glVertex2f(400 - R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glVertex2f(0, 600);
	glVertex2f(0, 0);
	glVertex2f(400 - R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glEnd();

	//The first animation is end so it call the "transitionBackStart" to start the second animation
	if (R < 0)
	{
		transitionBackStart();
	}
}

//This function is the second transmition animation
//Parameters "speed" and "transitionTime" are used to control the animation time
void starTransitionBack(float speed, float transitionTime)
{
	float changeLength = speed * transitionTime;
	float R = changeLength;
	float r = R / (sinf(36.0f * 3.1415926 / 180.0f) * cosf(18.0f * 3.1415926 / 180.0f) / sinf(18.0f * 3.1415926 / 180.0f) + cosf(36.0f * 3.1415926 / 180.0f));

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(400 - r * sinf(36.0f * angleRadian), 300 + r * cosf(36.0f * angleRadian));
	glVertex2f(400, 300 + R);
	glVertex2f(400, 600);
	glVertex2f(0, 600);
	glVertex2f(400 - R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(400 + r * sinf(36.0f * angleRadian), 300 + r * cosf(36.0f * angleRadian));
	glVertex2f(400, 300 + R);
	glVertex2f(400, 600);
	glVertex2f(800, 600);
	glVertex2f(400 + R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(400, 300 - r);
	glVertex2f(400 + R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glVertex2f(800, 0);
	glVertex2f(0, 0);
	glVertex2f(400 - R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(400 + r * cosf(18.0f * angleRadian), 300 - r * sinf(18.0f * angleRadian));
	glVertex2f(400 + R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glVertex2f(800, 600);
	glVertex2f(800, 0);
	glVertex2f(400 + R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(400 - r * cosf(18.0f * angleRadian), 300 - r * sinf(18.0f * angleRadian));
	glVertex2f(400 - R * cosf(18.0f * angleRadian), 300 + R * sinf(18.0f * angleRadian));
	glVertex2f(0, 600);
	glVertex2f(0, 0);
	glVertex2f(400 - R * sinf(36.0f * angleRadian), 300 - R * cosf(36.0f * angleRadian));
	glEnd();

	//The second animation is end so it control the "transitionState" back to 0
	if (R >= 300)
	{
		transitionState = 0;
	}
}

//This function controls the interaction with keyboard
void keyboardInteraction(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
		exit(0);

	//Press "e" to start transition animation;
	if (transitionState == 0 && (key == 'e' || key == 'E'))
		transitionStatesChange();
	
	//Press "w" to get closer to Kirby in the second stage
	if (key == 'w' || key == 'W')
	{
		if (stages == 1 && kirbyDistance <= 50)
		{
			textShown = false;
			kirbyDistance += 10;
		}
		else
		{
			transitionStatesChange();
		}
		
	}
	//Press "s" to get away to Kirby in the second stage
	if (key == 's' || key == 'S')
	{
		if (stages == 1 && kirbyDistance > 0)
		{
			textShown = false;
			kirbyDistance -= 10;
		}
	}
}

//This function controls the interaction with mouse
void mouseInteraction(int button, int state, int x, int y)
{
	//Click to start transition animation;
	if (transitionState == 0 &&  stages != 2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		transitionStatesChange();
	}

	//If click the cake,then cake will be followed with the mouse
	if (stages == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (abs(cakeX - x) < 50 && abs(cakeY - (600 - y)) < 50) {
			isDraggingCake = true;
		}
	}
	
}

//This function is showing the first stage
void stage0(void)
{
	//Draw the static stars
	starsScene();

	//Draw the moving stars
	movingStarsScne();

	//Draw the rainbow of kirbystar
	glColor3f(252 / 255.0f, 1 / 255.0f, 0 / 255.0f);
	line(kirbyStarPositionX - 72 / 5, kirbyStarPositionY + 96 / 5, 800 - 72 / 5, 600 + 96 / 5, 12);
	line(kirbyStarPositionX - 60 / 5, kirbyStarPositionY + 80 / 5, 800 - 60 / 5, 600 + 80 / 5, 12);
	glColor3f(251 / 255.0f, 153 / 255.0f, 1 / 255.0f);
	line(kirbyStarPositionX - 48 / 5, kirbyStarPositionY + 64 / 5, 800 - 48 / 5, 600 + 64 / 5, 12);
	line(kirbyStarPositionX - 36 / 5, kirbyStarPositionY + 48 / 5, 800 - 36 / 5, 600 + 48 / 5, 12);
	glColor3f(255 / 255.0f, 251 / 255.0f, 6 / 255.0f);
	line(kirbyStarPositionX - 24 / 5, kirbyStarPositionY + 32 / 5, 800 - 24 / 5, 600 + 32 / 5, 12);
	line(kirbyStarPositionX - 12 / 5, kirbyStarPositionY + 16 / 5, 800 - 12 / 5, 600 + 16 / 5, 12);
	glColor3f(51 / 255.0f, 252 / 255.0f, 5 / 255.0f);
	line(kirbyStarPositionX, kirbyStarPositionY, 800, 600, 12);
	line(kirbyStarPositionX + 12 / 5, kirbyStarPositionY - 16 / 5, 800 + 12 / 5, 600 - 16 / 5, 12);
	glColor3f(34 / 255.0f, 223 / 255.0f, 201 / 255.0f);
	line(kirbyStarPositionX + 24 / 5, kirbyStarPositionY - 32 / 5, 800 + 24 / 5, 600 - 32 / 5, 12);
	line(kirbyStarPositionX + 36 / 5, kirbyStarPositionY - 48 / 5, 800 + 36 / 5, 600 - 48 / 5, 12);
	glColor3f(0 / 255.0f, 153 / 255.0f, 255 / 255.0f);
	line(kirbyStarPositionX + 48 / 5, kirbyStarPositionY - 64 / 5, 800 + 48 / 5, 600 - 64 / 5, 12);
	line(kirbyStarPositionX + 60 / 5, kirbyStarPositionY - 80 / 5, 800 + 60 / 5, 600 - 80 / 5, 12);
	glColor3f(89 / 255.0f, 55 / 255.0f, 235 / 255.0f);
	line(kirbyStarPositionX + 72 / 5, kirbyStarPositionY - 96 / 5, 800 + 72 / 5, 600 - 96 / 5, 12);
	line(kirbyStarPositionX + 84 / 5, kirbyStarPositionY - 112 / 5, 800 + 84 / 5, 600 - 112 / 5, 12);

	//Draw the Kirbystar
	glColor3f(255 / 255.0f, 240 / 255.0f, 121 / 255.0f);
	roundStars(kirbyStarPositionX, kirbyStarPositionY, 40);

	//Show the text
	if (stayTime > 20.0f)
	{
		glColor3f(1, 1, 1);
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(180, 75);
		drawString("Click or Press \"e\" to continue");
	}
}

//This function is showing the second stage
void stage1(void)
{
	//Draw the static stars
	starsScene();

	//Draw the moving stars
	movingStarsScne();

	//Draw the grass and flowers
	grassScene();
	flower(270 - kirbyDistance / 3, 290 - kirbyDistance, 40 + kirbyDistance / 5);
	flower(560 + kirbyDistance / 3, 280 - kirbyDistance, 40 + kirbyDistance / 5);
	flower(210 - kirbyDistance / 3, 270 - kirbyDistance, 40 + kirbyDistance / 5);

	flower(100 - kirbyDistance / 3, 150 - kirbyDistance, 40 + kirbyDistance / 5);
	flower(80 - kirbyDistance / 3, 280 - kirbyDistance, 40 + kirbyDistance / 5);
	flower(600 + kirbyDistance / 3, 100 - kirbyDistance, 40 + kirbyDistance / 5);
	flower(650 + kirbyDistance / 3, 200 - kirbyDistance, 40 + kirbyDistance / 5);
	flower(720 + kirbyDistance / 3, 240 - kirbyDistance, 40 + kirbyDistance / 5);

	//Draw Kirby
	kirby(400, 300 - kirbyDistance, 50);

	//Draw the shining stars
	if (shining)
	{
		glColor3f(253 / 255.0f, 199 / 255.0f, 53 / 255.0f);
		roundStars(600, 450 - kirbyDistance, shiningStars + 2);
		roundStars(600, 150 - kirbyDistance, shiningStars + 2);
		roundStars(200, 450 - kirbyDistance, shiningStars + 2);
		roundStars(200, 150 - kirbyDistance, shiningStars + 2);
		glColor3f(255 / 255.0f, 240 / 255.0f, 121 / 255.0f);
		roundStars(600, 450 - kirbyDistance, shiningStars);
		roundStars(600, 150 - kirbyDistance, shiningStars);
		roundStars(200, 450 - kirbyDistance, shiningStars);
		roundStars(200, 150 - kirbyDistance, shiningStars);
	}
	
	//Show the text
	if (stayTime < 30.0f)
	{
		glColor3f(1, 1, 1);
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(310, 75);
		drawString("Oh! Kirby!");
	}
	else if (textShown)
	{
		glColor3f(1, 1, 1);
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(120, 75);
		drawString("Try to use \"w\" to get closer to Kirby");
	}
}

//This function is showing the third stage
void stage2(void)
{
	//Draw the static stars
	starsScene();

	//Draw the moving stars
	movingStarsScne();

	//Draw the birthday card
	bithdayCard();

	//Show the text
	if (stayTime > 100.0f && kirbyCakeStickers != true)
	{
		glColor3f(1, 1, 1);
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(250, 75);
		drawString("Feed cake to Kirby");
	}
}

//This function controls all the stages and animations will be shown in the windows
void display(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 50 / 255.0f, 99 / 255.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Show the stages according to the variable "stages"
	if (stages == 0)
	{
		stage0();
	}
	if (stages == 1)
	{
		stage1();
	}
	if (stages == 2)
	{
		stage2();
	}

	//Show the transition animation  according to the variable "transitionState"
	if (transitionState == 1) {
		starTransition(10, transitionTime);
	}


	if (transitionState == 2) {
		starTransitionBack(10, transitionBackTime);
	}
	
	//Show the star following the mouse
	mouseStar(mouseX, mouseY);

	glutSwapBuffers();
}

//Set the windown size and some callback function
int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Happy Birthday");

	glutDisplayFunc(display);

	glutPassiveMotionFunc(mouseMove);

	glutTimerFunc(0, update, 0);

	glutKeyboardFunc(keyboardInteraction);
	glutMouseFunc(mouseInteraction);

	glutMainLoop();
	return 0;
}
