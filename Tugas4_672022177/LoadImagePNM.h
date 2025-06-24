#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

GLuint loadPNM_Custom(const char* imagepath) {
    FILE* file;
    errno_t err = fopen_s(&file, imagepath, "rb");
    if (!file) {
        printf("Gambar %s tidak dapat dibuka\n", imagepath);
        return 0;
    }

    char header[3];
    fscanf(file, "%2s\n", header);
    if (header[0] != 'P' || header[1] != '6') {
        printf("%s bukan file PNM P6\n", imagepath);
        fclose(file);
        return 0;
    }

    int c;
    while ((c = fgetc(file)) == '#') {
        while (fgetc(file) != '\n');
    }
    fseek(file, -1, SEEK_CUR);

    unsigned int width, height, maxval;
    fscanf(file, "%u %u\n%u\n", &width, &height, &maxval);
    if (maxval != 255) {
        printf("%s bukan PNM dengan maxval 255\n", imagepath);
        fclose(file);
        return 0;
    }

    if (width != 256 || height != 256) {
        printf("%s bukan gambar berukuran 256x256 piksel\n", imagepath);
        fclose(file);
        return 0;
    }

    unsigned int imageSize = width * height * 3;

    unsigned char* data = new unsigned char[imageSize];
    if (fread(data, 1, imageSize, file) != imageSize) {
        printf("Gagal membaca data gambar PNM\n");
        delete[] data;
        fclose(file);
        return 0;
    }
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