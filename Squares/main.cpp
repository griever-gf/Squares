#pragma once
#include <cstdlib> 
#include <ctime> 
#include <sstream>
//#include <GL/freeglut.h>
#include <GL/glut.h>

const double WindowWidth = 600;
const double WindowHeight = 600;
int colors[3];
bool isGameCompleted = 0;

void drawMainScreen();
void mouse(int button, int state, int x, int y);

int main(int argc, char** argv) 
{ 
	glutInit(&argc, argv); 
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowSize (600, 600); 
	glutInitWindowPosition (200, 200); 
	glutCreateWindow ("SQUARES"); 

	glClearColor (0.0, 0.0, 0.0, 0.0); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(0, WindowWidth, 0, WindowHeight, -1.0f, 1.0f);

	glutDisplayFunc(drawMainScreen);
	glutMouseFunc(mouse);

	glutMainLoop(); 

	return 0;
} 

void drawSingleSquare(float x, float y, float len) 
{ 
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y+len);
		glVertex2f(x+len, y+len);
		glVertex2f(x+len, y);
	glEnd();
}

void setRandomSquareColor(int RedYellowBlue) 
{ 
	switch (RedYellowBlue)
	{
		case 0:
			glColor3f(1.0, 0.0, 0.0); //красный
			break;
		case 1:
			glColor3f(1.0, 1.0, 0.0); //желтый
			break;
		case 2:
			glColor3f(0.0, 0.0, 1.0); //синий
			break;
		default:
			glColor3f(0.0, 1.0, 1.0); //cyan
			break;
	}
}

void drawText(int x, int y, std::string textS)
{
	glColor3f(0, 0, 0); //чёрный
	glBegin(GL_POLYGON);
		glVertex2f(x, y-3);
		glVertex2f(x, y+10);
		glVertex2f(x+9*textS.size(), y+10);
		glVertex2f(x+9*textS.size(), y-3);
	glEnd();
	glColor3f(0, 1, 0); //зелёный

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
	glOrtho(0, WindowWidth, 0, WindowHeight, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);
    for (int i=0; i<textS.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, textS[i]);
    }
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawMainScreen() 
{ 
	glClear (GL_COLOR_BUFFER_BIT);

	srand((unsigned)time(0)); 

	int i = 0;
	while (i < 3)
	{
		int randomColor = rand()%3;
		bool sameExists = false;
		for (int j=i-1; j>=0; j--)
		{
			if (colors[j] == randomColor)
			{
				sameExists = true;
				break;
			}
		}

		if (!sameExists)
		{
			colors[i] = randomColor;
			i++;
		}
	}

	setRandomSquareColor(colors[0]);
	drawSingleSquare(30, 30, 180);

	setRandomSquareColor(colors[1]);
	drawSingleSquare(30, 210, 180);

	setRandomSquareColor(colors[2]);
	drawSingleSquare(30, 390, 180);

	drawText(300, 320, "use left mouse button");
	drawText(300, 300, "to change square color");

	glFlush (); 
}

void mouse(int button, int state, int x, int y) //обработчик мыши
{
  switch (button) {
    case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (!isGameCompleted)
			{
				if ((x>=30)&&(x<210)&&(y>=30)&&(y<570)) //если попадает в прямоугольник
				{
					int trueY = WindowHeight-y-1;
					int i = (trueY-30)/180;
					colors[i] = (colors[i] + 1) % 3;
					setRandomSquareColor(colors[i]);
					drawSingleSquare(30, 30 + i*180, 180);
					glFlush (); 
					isGameCompleted = ((colors[0] == colors[1])&&(colors[1] == colors[2]));
					if (isGameCompleted)
					{
						drawText(300, 320, "CONGRATULATIONS! YOU WON!");
						drawText(300, 300, "press LMB to start again");
					}
				}
			}
			else
			{
				drawMainScreen();
				isGameCompleted = false;
			}
			glFlush();
		}
        break;
      }
}
