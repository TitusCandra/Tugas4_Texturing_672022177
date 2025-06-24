#include <GL/freeglut.h>
#include <jpeglib.h>
#include <stdio.h>
#include <stdlib.h>

GLuint loadJPG_Custom(const char* imagepath) {
    FILE* file;
    errno_t err = fopen_s(&file, imagepath, "rb");
    if (!file) {
        printf("Gambar %s tidak dapat dibuka\n", imagepath);
        return 0;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);

    if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK) {
        printf("%s bukan file JPG yang valid\n", imagepath);
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return 0;
    }

    jpeg_start_decompress(&cinfo);
    unsigned int width = cinfo.output_width;
    unsigned int height = cinfo.output_height;
    int channels = cinfo.output_components;

    if (width != 256 || height != 256) {
        printf("%s bukan gambar berukuran 256x256 piksel\n", imagepath);
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return 0;
    }

    if (channels != 3) {
        printf("%s bukan JPG RGB 24-bit\n", imagepath);
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return 0;
    }

    unsigned char* data = new unsigned char[width * height * channels];
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, width * channels, 1);

    unsigned char* p = data;
    while (cinfo.output_scanline < height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(p, buffer[0], width * channels);
        p += width * channels;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
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