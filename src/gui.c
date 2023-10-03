
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "board.h"

int tickTime;
int paused;
int *values;
char *board;
int curX, curY;

void syncBoard() {
    int dx = values[1] / values[2];
    int dy = values[0] / values[3];

    for (int i = 0; i < values[2]; i++) {
        for (int j = 0; j < values[3]; j++) {
            if (*(board + i * values[3] + j)) {
                glColor3f(0.0, 0.0, 0.0);
                glBegin(GL_POLYGON);
                glVertex2d(dx * i, dy * j);
                glVertex2d(dx * (i + 1), dy * j);
                glVertex2d(dx * (i + 1), dy * (j + 1));
                glVertex2d(dx * i, dy * (j + 1));
                glEnd();
            }
        }
    }
    glFlush();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);

    int dx = values[1] / values[2], dy = values[0] / values[3];
    for (int i = 0; i < values[2]; i++) {
        glVertex2d(dx * i, values[0]);
        glVertex2d(dx * i, 0);
    }

    for (int i = 0; i < values[3]; i++) {
        glVertex2d(values[1], dy * i);
        glVertex2d(0, dy * i);
    }
    glEnd();
    glFlush();

    syncBoard();
}

void onMouseButton(int button, int state, int x, int y) {
    if (state == 1 || button != GLUT_LEFT_BUTTON) {
        return;
    }

    x /= values[1] / values[2];
    y = values[3] - y / (values[0] / values[3]) - 1;
    
    *(board + x * values[3] + y) = (*(board + x * values[3] + y) + 1) % 2;
    
    display();
}

void specialKey(int key, int x, int y) {
    switch(key) {
        case 101:
            tickTime += 1000 / 65;
            break;
        case 103:
            tickTime -= 1000 / 65;
            if (tickTime < 0)
                tickTime = 0;
            break;
    }

    display();
}

void key(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
            exit(0);
            break;
        case 't':
            turn(board, values[2], values[3]);
            break;
        case 'g':
            setGlider(board, values[2], values[3], curX, curY);
            break;
        case 's':
            setShattle(board, values[2], values[3], curX, curY);
            break;
        case 'G':
            setGun(board, values[2], values[3], curX, curY);
            break;
        case 'p':
            paused = (paused + 1) % 2;
            break;
        case 'c':
            for (int i = 0; i < values[2]; i++)
                for (int j = 0; j < values[3]; j++)
                    *(board + i * values[3] + j) = 0;
            break;
    }

    display();
}

void tick() {
    if (!paused) {
        turn(board, values[2], values[3]);
        display();
    }
    glutTimerFunc(tickTime, tick, 0);
}

void updatePos(int x, int y) {
    curX = x / (values[1] / values[2]);
    curY = y / (values[0] / values[3]);
}

int main(int argc, char *argv[]) {
    tickTime = 1000 / 65;
    curX = 0;
    curY = 0;
    paused = 1;

    values = readConfig("./data/config.conf");

    board = (char *)malloc(sizeof(char) * values[2] * values[3]);
	for (int i = 0; i < values[2]; i++)
		for (int j = 0; j < values[3]; j++)
			*(board + i * values[3] + j) = 0;

    int window;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);

    glutInitWindowSize(values[1], values[0]);
    window = glutCreateWindow("Triangle");

    glClearColor(1.0,1.0,1.0,1.0);
    gluOrtho2D(0,values[1],0,values[0]);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    glutMouseFunc(onMouseButton);
    glutPassiveMotionFunc(updatePos);

    tick(0);

    glutMainLoop();

    return 0;
}
