#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

GLuint loadBMP_Custom(const char* imagepath) {
    unsigned char header[54];
    unsigned int dataPos, width, height, imageSize;
    unsigned char* data;

    FILE* file;
    fopen_s(&file, imagepath, "rb");
    if (!file) {
        printf("Gambar %s tidak dapat dibuka\n", imagepath);
        return 0;
    }

    if (fread(header, 1, 54, file) != 54) {
        printf("%s bukan file BMP yang valid\n", imagepath);
        fclose(file);
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("%s bukan file BMP yang valid\n", imagepath);
        fclose(file);
        return 0;
    }

    if (*(int*)&header[0x1E] != 0) {
        printf("%s menggunakan kompresi, tidak didukung\n", imagepath);
        fclose(file);
        return 0;
    }

    if (*(int*)&header[0x1C] != 24) {
        printf("%s bukan BMP 24-bit\n", imagepath);
        fclose(file);
        return 0;
    }

    dataPos = *(int*)&header[0x0A];
    imageSize = *(int*)&header[0x22];
    width = *(int*)&header[0x12];
    height = *(int*)&header[0x16];

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;

    return textureID;
}