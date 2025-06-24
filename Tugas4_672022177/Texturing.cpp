#define _CRT_SECURE_NO_WARNINGS 
#include "loadImageBMP.h"
#include "loadImageJPEG.h"
#include "loadImagePNG.h"
#include "loadImagePNM.h"
#include "loadImageJPG.h"
#include "loadImageTGA.h"
#include <GL/freeglut.h>
#include <iostream>

GLuint _textureIDs[6]; 
float rotationAngle = 0.0f;

void drawCube() {
    glEnable(GL_TEXTURE_2D);

    for (int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, _textureIDs[i]);

        glColor3f(1.0f, 1.0f, 1.0f);

        switch (i) {
        case 0:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glEnd();
            break;
        case 1:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glEnd();
            break;
        case 2:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glEnd();
            break;
        case 3:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glEnd();
            break;
        case 4:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
            glEnd();
            break;
        case 5:
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glEnd();
            break;
        }
    }

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef(rotationAngle, 1.0f, 1.0f, 0.0f);
    drawCube();
    glPopMatrix();

    glutSwapBuffers();
}

void myinit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    _textureIDs[0] = loadBMP_Custom("Gambar1-256x256.bmp");
    _textureIDs[1] = loadJPEG_Custom("Gambar2-256x256.jpeg");
    _textureIDs[2] = loadPNG_Custom("Gambar3-256x256.png");
    _textureIDs[3] = loadPNM_Custom("Gambar4-256x256.pnm");
    _textureIDs[4] = loadJPG_Custom("Gambar5-256x256.jpg");
    _textureIDs[5] = loadTGA_Custom("Gambar6-256x256.tga");

    for (int i = 0; i < 6; i++) {
        if (_textureIDs[i] == 0) {
            std::cerr << "Gagal memuat tekstur " << i << "! Periksa path, format, atau ukuran gambar." << std::endl;
            exit(1);
        }
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    rotationAngle += 1.0f;
    if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 60 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Texturing Cube Rotation");
    glutDisplayFunc(display);

    myinit();

    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    return 0;
}