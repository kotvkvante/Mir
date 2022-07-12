#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb/stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/stb/stb_image_write.h"

#include "../math/point.h"
#include "../kernel/error_handler.h"
#include "../kernel/kernel.h"
#include "textures.h"
#include "texture_map.h"
#include "../utils/utils.h"
#include "graphics.h"

typedef struct texture_map_t
{
    char* path;
    int path_length;

    GLuint texture;

} texture_map_t;

GLuint char_texture;
GLuint rendered_texture;

textures_t textures;
texture_9slices_t slices9;
texture_3slices_t slices3;


void textures_init()
{
    textures.tile_map = texture_load("assets/textures/35.png");
}

GLuint texture_get_tile_map()
{
    return textures.tile_map;
}

GLuint texture_get_font_map()
{
    return textures.font_map;
}


GLuint texture_load(char* file_name)
{
    GLuint t;
    GLuint* texture = &t;
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//    glGenerateMipmap(GL_TEXTURE_2D);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;

    unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    if (nrChannels < 4)
    {
        error_msg(DEFAULT_C, "Loaded image have less than 4 color channels.\n");
    }

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        log_msg_s(DEFAULT_C, "Failed load texture: %s\n", (char*)file_name);
    }
    stbi_image_free(data);

    return t;
}


GLuint texture_from_bitmap(unsigned char* data, int width, int height, int channels)
{
    if(data == NULL)
    {
        error_msg(DEFAULT_C, "Texture from bitmap data null");
    }
    GLuint t;
    GLuint* texture = &t;
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return t;
}

void texture_load_char(unsigned char* data, int width, int height, int channels)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &char_texture);
    glBindTexture(GL_TEXTURE_2D, char_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void texture_load_font_map(int width, int height, unsigned char* data)
{
    glGenTextures(1, &textures.font_map);
    glBindTexture(GL_TEXTURE_2D, textures.font_map);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


GLuint FramebufferName = 0;
static int _texture_width;
static int _texture_height;
static int _texture_named_width;
static int _texture_named_height;
//static int _failed;

int texture_named_render_begin(GLuint* named_texture, int width, int height)
{
    point2f_t ws = kernel_get_window_size();
     _texture_named_width = ws.x;
    _texture_named_height = ws.y;

    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    if(glIsTexture(*named_texture))
    {
//        error_msg(GRAPHICS_C, "Named texture already exists!");
    }
    else
    {
        glGenTextures(1, named_texture);
    }

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, *named_texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texture_named_width, _texture_named_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *named_texture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        graphics_check_error();
        error_msg(DEFAULT_C, "Failed create framebuffer.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0, 0, _texture_named_width, _texture_named_height);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return 0;
}

void texture_named_render_end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    point2f_t scr = kernel_get_window_size();
    glViewport(0, 0, scr.x, scr.y);
}

void texture_named_save()
{
    unsigned char* imageData = (unsigned char*)malloc(sizeof(int) * (_texture_named_height * _texture_named_width * 3) );
	glReadPixels(0, 0, _texture_named_width, _texture_named_height,
                 GL_RGB, GL_UNSIGNED_BYTE, imageData);

	char file_path[32];
    sprintf(file_path, "test_named_%ld.png", time(NULL));

    stbi_write_png(file_path,
                   _texture_named_width,
                   _texture_named_height,
                   3, imageData, _texture_named_width * 3);
}

void texture_render_begin(int width, int height)
{
    _texture_width  = width;
    _texture_height = height;


    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glGenTextures(1, &rendered_texture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, rendered_texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rendered_texture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        error_msg(DEFAULT_C, "Failed create framebuffer.");

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0, 0, width, height);
}

void texture_render_end()
{

    unsigned char* imageData = (unsigned char*)malloc(sizeof(int) * (_texture_height * _texture_width * 3) );
	glReadPixels(0, 0, _texture_width, _texture_height, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	char file_path[32];
    sprintf(file_path, "test_%ld.png", time(NULL));

//    printf("%s\n", file_path);

    stbi_write_png(file_path, _texture_width, _texture_height, 3, imageData, _texture_width * 3);

//    	stbi_write_png("font_output.png", tex_width, tex_height, 4, png_data, tex_width * 4);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    point2f_t scr = kernel_get_window_size();
    glViewport(0, 0, scr.x, scr.y);
}

void texture_save_to_png(GLuint texture)
{
//    pass
}

void texture_9slices_init()
{
//    slices9
}

void texture_3slices_init()
{
//    slices3
}

//int awx_ScreenShot()
//{
//    unsigned char *pixels;
//    FILE *image;
//    GLint viewport[4];
//    glGetIntegerv(GL_VIEWPORT, viewport);
//    pixels = new unsigned char[viewport[2]*viewport[3]*3];
//    glReadPixels(0, 0, viewport[2], viewport[3], GL_BGR,
//                                       GL_UNSIGNED_BYTE, pixels);
//    char tempstring[50];
//    sprintf(tempstring,"Screenshots/screenshot_%i.tga",1);
//    if((image=fopen(tempstring, "wb"))==NULL)
//    return 1;
//    unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
//    unsigned char header[6]={((int)(viewport[2]%256)),
//                       ((int)(viewport[2]/256)),
//                        ((int)(viewport[3]%256)),
//                        ((int)(viewport[3]/256)),24,0};  // TGA header schreiben
//                        fwrite(TGAheader, sizeof(unsigned char), 12, image);  // Header schreiben
//                        fwrite(header, sizeof(unsigned char), 6, image);
//                        fwrite(pixels, sizeof(unsigned char),
//                        viewport[2]*viewport[3]*3, image);
//    fclose(image);
//    delete [] pixels;
//    return 0;
//}

