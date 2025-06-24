#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

GLuint loadTGA_Custom(const char* imagepath) {
    FILE* file;
    errno_t err = fopen_s(&file, imagepath, "rb");
    if (err != 0) {
        printf("Gambar %s tidak dapat dibuka\n", imagepath);
        return 0;
    }

    unsigned char header[18];
    if (fread(header, 1, 18, file) != 18) {
        printf("%s bukan file TGA yang valid\n", imagepath);
        fclose(file);
        return 0;
    }

    unsigned int width = header[12] | (header[13] << 8);
    unsigned int height = header[14] | (header[15] << 8);
    unsigned char bpp = header[16];
    if (bpp != 24) {
        printf("%s bukan TGA 24-bit\n", imagepath);
        fclose(file);
        return 0;
    }

    unsigned int imageSize = width * height * 3;
    unsigned char* data = new unsigned char[imageSize];
    if (fread(data, 1, imageSize, file) != imageSize) {
        printf("Gagal membaca data gambar TGA\n");
        delete[] data;
        fclose(file);
        return 0;
    }
    fclose(file);

    for (unsigned int i = 0; i < imageSize; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;

    return textureID;
}