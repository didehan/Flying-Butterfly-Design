

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables */
; bool up = false, down = false, right = false, left = false, mode = true, timer = true, sunmove = false;
int  winWidth, winHeight, butterfly = 0, u = 0, r = 0, flow = 0, red = 0, green = 0, blue = 0, xx = 0;
char direction = 'n';


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}
void displayButterfly() {
    //wings
    glColor3f(1, 204 / 255.0, 229 / 255.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(80+r, -50+u);
    glVertex2f(80+r, 100+u);
    glVertex2f(10+r, 0+u);
    glEnd();

    
    glBegin(GL_TRIANGLES);
    glVertex2f(-80+r, -50+u);
    glVertex2f(-80+r, 100+u);
    glVertex2f(-10+r, 0+u);
    glEnd();

    glColor3f(1, 153 / 255.0, 204 / 255.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100+r, 150+u);
    glVertex2f(100+r, 30+u);
    glVertex2f(10+r, 0+u);
    glEnd();


    glBegin(GL_TRIANGLES);
    glVertex2f(-100+r, 150+u);
    glVertex2f(-100+r, 30+u);
    glVertex2f(-10+r, 0+u);
    glEnd();

    //body
    if (timer) {//to randomly generate colors for body
      
        glColor3d(red/ 255.,green/ 255., blue / 255.);
        glRectf(10 + r, 80 + u, -10 + r, -30 + u);
        circle(0 + r, 80 + u, 10);
        circle(0 + r, -30 + u, 10);
    }
    //eyes
    if (r % 4 == 0 || u % 4 == 0)//for blinking at a certain location
    {
        glColor3d(1, 1, 1);
        circle(5 + r, 78 + u, 3);
        circle(-5 + r, 78 + u, 3);
        glColor3d(0, 0, 0);
        circle(5 + r, 78 + u, 1);
        circle(-5 + r, 78 + u, 1);
    }
    else {
        glColor3d(0, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2d(5+r, 80 + u);
        glVertex2d(-5 + r, 80 + u);
        glEnd();
        glBegin(GL_LINES);
        glVertex2d(5+ r, 80 + u);
        glVertex2d(-5 + r, 80 + u);
        glEnd();
        
    }



    //antennas
    glColor3d(red/ 255., green / 255., blue/255.);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2d(0 + r, 80 + u);
    glVertex2d(20 + r, 100 + u);
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2d(0 + r, 80 + u);
    glVertex2d(-20 + r, 100 + u);
    glEnd();


}
void displayFlowers(int xx, int yy) {
    
    glColor3d(153 / 255.,0 ,153/255.);
    circle(xx-10+flow, yy-10, 10);
    circle(xx+10+flow, yy+10, 10);
    circle(xx-10+flow, yy+10, 10);
    circle(xx+10+flow, yy-10, 10);
    glColor3d(1,204/255. , 153/255.);
    circle(xx+flow, yy, 10);
}
void displayInfo()
{
    glColor3d(153 / 255., 0, 153 / 255.);
    vprint(250, 380, GLUT_BITMAP_8_BY_13, "Didehan Topsakal");  /// Name-Surname
    vprint(250, 360, GLUT_BITMAP_8_BY_13, "22003293");   ///ID

    glColor3d(153 / 255., 0, 153 / 255.);
    if (mode == true)
    {
        vprint(-400, 380, GLUT_BITMAP_9_BY_15, "Mode : ");   /// To display mode information
        vprint(-330, 380, GLUT_BITMAP_9_BY_15, "MANUAL");
    }
    else
    {
        vprint(-400, 380, GLUT_BITMAP_9_BY_15, "Mode : ");  
        vprint(-330, 380, GLUT_BITMAP_9_BY_15, "AUTONOMOUS");
    }
    vprint(-400, 360, GLUT_BITMAP_9_BY_15, "Direction : ");
    
   
        switch (direction)  
        {
        case 'U':

            vprint(-300, 360, GLUT_BITMAP_9_BY_15, "UP");
            break;
        case 'D':

            vprint(-300, 360, GLUT_BITMAP_9_BY_15, "DOWN");
            break;
        case 'L':

            vprint(-300, 360, GLUT_BITMAP_9_BY_15, "LEFT");
            break;
        case 'R':

            vprint(-300, 360, GLUT_BITMAP_9_BY_15, "RIGHT");
            break;
        default:
            vprint(-300, 360, GLUT_BITMAP_9_BY_15, "NOT SET");
            break;
        }
        vprint(-200, -390, GLUT_BITMAP_8_BY_13, "first use spacebar then arrow keys to switch modes!");
    
}

void displaySun()
{
    glColor3d(1, 1, 102 / 255.);//sun
    circle(250, 300, 50);
    glColor3d(1, 153 / 255., 0);
    circle(250, 300, 30+xx);
}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    int xx, yy;
    glClearColor(153/255., 204/255., 255/255., 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(102/255., 204 / 255.,0);//grass area
    glRectf(400, -400, -400, 150);
    
    displaySun();
    
    for(xx=-350;xx<350;xx+=120)
        for(yy=-350;yy<150;yy+=200)
            displayFlowers(xx,yy);
    if (butterfly == 0) {
        glColor3d(153 / 255., 0, 153 / 255.);
        vprint(-150, 0, GLUT_BITMAP_8_BY_13, "click window to see the butterfly!");
    }
    else if(butterfly == 1)
        displayButterfly();
    displayInfo();

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
        mode = !mode;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    if (mode)//for manuel movement
    {
        switch (key) {
        case GLUT_KEY_UP: up = true;
            if (u < 250) {
                direction = 'U';
                u += 10; 
                break;
            }
        case GLUT_KEY_DOWN: down = true;
            if (u > -350) {
                direction = 'D';
                u -= 10; 
                break;
            }
        case GLUT_KEY_LEFT: left = true;
            if (r > -300) {
                direction = 'L';
                r -= 10;
                break;
            }
        case GLUT_KEY_RIGHT: right = true;
            if (r < 300) {
                direction = 'R';
                r += 10; 
                break;
            }
        }
    }
    if (!mode)//for autonomous movement 
    {
        switch (key)
        {
        case GLUT_KEY_UP: up = true;
            direction = 'U'; 
            break;
        case GLUT_KEY_DOWN: down = true;
            direction = 'D'; 
            break;
        case GLUT_KEY_LEFT: left = true;
            direction = 'L'; 
            break;
        case GLUT_KEY_RIGHT: right = true;
            direction = 'R';  
            break;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; 
        break;
    case GLUT_KEY_DOWN: down = false; 
        break;
    case GLUT_KEY_LEFT: left = false; 
        break;
    case GLUT_KEY_RIGHT: right = false; 
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        butterfly = 1;
    }
  
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {//for generating random colors for body part of the butterfly
        red = rand() % 256;
        green = rand() % 256;
        blue = rand() % 256;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();


}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
    
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    switch (mode)
    {
    case false: if (r < 500)
        r++;
              else
        while (r > -500)
            r--;

    }

    if (timer)//for the movement of background flowers
    {
        flow++;
        if (flow > 120)
            flow = 0;
    }
    switch (!mode)//autonomous movement
    {

    case true: if (direction == 'R')
        if (r < 300)
            r += 2;
        else
            direction = 'L';
             else if (direction == 'D')
        if (u > -300)
            u -= 2;
        else
            direction = 'U';
             else if (direction == 'L')
        if (r > -300)
            r -= 2;
        else
            direction = 'R';
             else if (direction == 'U')
        if (u < 300)
            u += 2;
        else
            direction = 'D';

        break;
    }
    if (timer)//for the movement of sun to make it like its shining
    {
        switch (sunmove)
        {
        case(false):if (xx + 30 < 48)
            xx++;
                   else if (xx + 30 >= 48)
            xx--;
        }
    }
    // to refresh the window it calls display() function
glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Drawing of a boy in primary school");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
