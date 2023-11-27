
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "config.h"
#include "board.h"
#include "list.h"

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
                glVertex2d(dx * i, dy * (values[3] - j - 1));
                glVertex2d(dx * (i + 1), dy * (values[3] - j - 1));
                glVertex2d(dx * (i + 1), dy * (values[3] - j + 1 - 1));
                glVertex2d(dx * i, dy * (values[3] - j + 1 - 1));
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
    
    setPoint(board, values[2], values[3], x, y);
    
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

void count_turn(char* board, int x, int y) {
    clock_t start_time = clock();
    turn(board, values[2], values[3]);
    clock_t delta = clock() - start_time;
    clock_t temp = delta;
    int length = 0, number;
    // while (temp != 0) {
    //     temp %= 10;
    //     length += 1;
    // }
    // char string[++length];
    // string[length--] = '\0';
    // while (delta != 0) {
    //     number = delta % 10;
    //     delta %= 10;
    //     string[length--] = number + 48;
    // }

    // char string[] = "Hello!";
    // int bitmapLength = glutBitmapLength(GLUT_BITMAP_8_BY_13, string);
    // glRasterPos2f(100., 100.);
    // glColor3f(1., 0., 0.);
    // for (int i = 0; i < strlen(string); i++)
    //     glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);

    return;
}

void key(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
            exit(0);
            break;
        case 't':
            count_turn(board, values[2], values[3]);
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
        count_turn(board, values[2], values[3]);
        display();
    }
    glutTimerFunc(tickTime, tick, 0);
}

void updatePos(int x, int y) {
    curX = x / (values[1] / values[2]);
    curY = values[3] - y / (values[0] / values[3]) - 1;
}

int main(int argc, char *argv[]) {
    tickTime = 1000 / 65;
    curX = 0;
    curY = 0;
    paused = 1;

    values = readConfig("./data/config.conf");

    ASSERT(values[0] % values[3] == 0 && values[1] % values[2] == 0);

    cells = createList();


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
