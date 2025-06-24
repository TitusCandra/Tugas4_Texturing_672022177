#include <GL/freeglut.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

GLuint loadPNG_Custom(const char* imagepath) {
    FILE* file;
    errno_t err = fopen_s(&file, imagepath, "rb");
    if (!file) {
        printf("Gambar %s tidak dapat dibuka\n", imagepath);
        return 0;
    }

    unsigned char header[8];
    if (fread(header, 1, 8, file) != 8) {
        printf("%s bukan file PNG yang valid\n", imagepath);
        fclose(file);
        return 0;
    }
    if (png_sig_cmp(header, 0, 8)) {
        printf("%s bukan file PNG yang valid\n", imagepath);
        fclose(file);
        return 0;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        printf("Gagal membuat struktur PNG\n");
        fclose(file);
        return 0;
    }
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(file);
        return 0;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file);
        printf("Error saat membaca PNG\n");
        return 0;
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 8);

    png_read_info(png, info);
    unsigned int width = png_get_image_width(png, info);
    unsigned int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (width != 256 || height != 256) {
        printf("%s harus beresolusi 256x256 piksel (ditemukan %ux%u)\n", imagepath, width, height);
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file);
        return 0;
    }

    unsigned int row_bytes = png_get_rowbytes(png, info);
    size_t imageSize = row_bytes * height;
    unsigned char* data = new unsigned char[imageSize];
    png_bytep* row_pointers = new png_bytep[height];
    for (unsigned int i = 0; i < height; i++) {
        row_pointers[i] = data + i * row_bytes;
    }

    png_read_image(png, row_pointers);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
    delete[] row_pointers;
    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);

    return textureID;
}