#define MIR_RENDER
#define GRAPHICS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "../math/point.h"
#include "../math/matrix.h"
#include "graphics.h"

#include "vertices.h"
#include "../kernel/kernel.h"
#include "../utils/utils.h"
#include "../kernel/error_handler.h"
#include "../kernel/window_events.h"
#include "../kernel/kernel_t.h"
#include "../mir/mir.h"

#include "camera.h"
#include "font.h"
#include "textures.h"
#include "texture_map.h"
#include "../gui/gui.h"
#include "../mir/mir.h"
#include "../mir/tile.h"
#include "../mir/unit.h"
#include "../gui/label.h"
#include "../gui/button.h"


#define PROGRAM(prog, path) \
if(LoadProgram(&(prog.id), "source/shaders/" path ".frag", "source/shaders/" path ".vert") < 0) \
{\
    log_msg_s(DEFAULT_C, "Failed load %s shader.", path); \
} prog ## _init()


extern kernel_t kernel;
extern mouse_t mouse;

extern mir_t mir;
extern label_t test;

extern textures_t textures;
extern GLuint char_texture;
extern GLuint rendered_texture;

extern int field_textures[];
extern int landscape_textures[];

extern font_t font;
extern text_t hello;
extern wtext_t russia;

extern glyph_info_t info[NUM_GLYPHS];


extern char* text_;


void program_init(gl_program_t* program);
void glp_base_init();
void glp_text_init();
void glp_texture_map_init();
void glp_texture_map_s_init(); // solid
void glp_texture_map_ex_init();
void glp_3slices_init();
void glp_point_init();



#define MIR_RENDER
gl_program_t gl_program;
glp_base_t glp_base;
glp_text_t glp_text;
glp_texture_map_t glp_texture_map;
glp_texture_map_s_t glp_texture_map_s; // solid
glp_texture_map_ex_t glp_texture_map_ex; // extended
glp_9slices_t glp_9slices;
glp_3slices_t glp_3slices;
glp_point_t glp_point;


void vao_init();
void vao_base_init();
void vbo_point_init();


GLuint LoadFile(char* FileName, const GLchar** t);
GLuint LoadShader(char *FileName, GLuint type);
int LoadProgram(GLuint* ID, char* frag, char* vert);
void draw_square();
void draw_rectangle(float x, float y, float w, float h);

void opengl_init()
{
    if(!glfwInit())
	{
		error_msg(DEFAULT_C, "Failed init glfw");
	}

//    GLint maxTextureSize;
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
//    print_i(maxTextureSize);

//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	kernel.window_size.x = 1280.0f;
	kernel.window_size.y = 720.0f;
	kernel.window_title  = "Mir";
  	kernel.window = glfwCreateWindow(
		kernel.window_size.x,
  		kernel.window_size.y,
  		kernel.window_title,
  		NULL,
  		NULL
  	);
  	if (!kernel.window)
  	{
        error_msg(DEFAULT_C, "Failed create window");
  	}

  	glfwMakeContextCurrent(kernel.window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        error_msg(DEFAULT_C, "Failed load glad");
    }

	glfwSetKeyCallback(kernel.window, key_callback);
    glfwSetMouseButtonCallback(kernel.window, mouse_button_callback);
    glfwSetCursorPosCallback(kernel.window, cursor_position_callback);
    glfwSetCharCallback(kernel.window, character_callback);

//  	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}


void graphics_init()
{

    if(LoadProgram(&gl_program.id, "source/shaders/main.frag", "source/shaders/main.vert") < 0)
    {
        error_msg(DEFAULT_C, "Failed load main shader.");
    } program_init(&gl_program);

    PROGRAM(glp_base, "base");
    PROGRAM(glp_text, "text");
    PROGRAM(glp_texture_map, "texture_map");
    PROGRAM(glp_texture_map_s, "texture_map_solid");
    PROGRAM(glp_texture_map_ex, "texture_map_ex");
//    PROGRAM(glp_9slices, "9slices");
    PROGRAM(glp_3slices, "3slices");
    PROGRAM(glp_point, "point");

    vao_init();
    vao_base_init();
    vbo_point_init();
}

void graphics_reload_program()
{
//    PROGRAM(glp_base, "base");
//    PROGRAM(glp_text, "text");
//    PROGRAM(glp_texture_map, "texture_map");
//    PROGRAM(glp_texture_map_s, "texture_map_solid");
    PROGRAM(glp_texture_map_ex, "texture_map_ex");
//    PROGRAM(glp_9slices, "9slices");
//    PROGRAM(glp_3slices, "3slices");
//    PROGRAM(glp_point, "point");

    log_msg(GRAPHICS_C, "Reload program");
}

GLuint vao;
GLuint vbo;
GLuint vao_base;
GLuint vbo_base;
GLuint vbo_point;

GLint size_vert;

void vao_init()
{
    size_vert = sizeof(vert);
    GLint size_text_coords = sizeof(text_coords);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, size_vert + size_text_coords, vert, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, size_vert, size_text_coords, text_coords);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vert)) );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void vao_base_init()
{
#define prog glp_base
    GLint size_vert = sizeof(vert);
    GLint size_text_coords = sizeof(text_coords_8);
	glGenVertexArrays(1, &vao_base);
	glBindVertexArray(vao_base);

	glGenBuffers(1, &vbo_base);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_base);

	glBufferData(GL_ARRAY_BUFFER, size_vert + size_text_coords, vert, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, size_vert, size_text_coords, text_coords_8);

	glEnableVertexAttribArray(prog.vertex_position);
	glVertexAttribPointer(prog.vertex_position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(prog.texture_coordinates);
	glVertexAttribPointer(prog.texture_coordinates, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vert)) );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#undef prog
}

void vbo_point_init()
{
    glGenBuffers(1, &vbo_point);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_point);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point) * 3, point, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void render_frame()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.7f, 0.6f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

//    draw_texture(textures.font_map);

//    draw_texture_char(textures.font_map, a);
//    draw_tile(textures.tile_map, 1, 1);

//    char* text1 = "kotvkvahte";
//    char* text1 = "Tell dog my dot me not";
//    char* text = "facking matter is\nme why im not sleeping now\nbecause am stupid animal";

//    draw_texture_text(text, strlen(text), textures.font_map);
//    draw_texture_text_xy(text_, strlen(text_), textures.font_map, 10.0f , 10.0f);
//
//    draw_text_xy(&hello, textures.font_map, 10.0f, 250.0f);

//    mir_map_draw_pickmap();

//    draw_point(200.0f, 200.0f);

////    mir_map_draw_pickmap();

//    draw_tile_ex_xy(1, 1);
//    _draw_tile_ex(0, 0);
//
//    draw_tile_ex(mir_map_get_tile(2, 2));

    mir_draw_team();
    mir_draw_map();
//    mir_map_draw_active();

//
    buttons_draw();
    labels_draw();

//    draw_point(100.0f, 100.0f);
//    draw_point(-kernel_get_window_width()/ 2 , -kernel_get_window_height()/2 );


//    draw_rectangle_whxy3s(textures.tile_map, (point2i_t){100, 64}, (point2i_t){128, 84});
//    draw_label(&test);

    draw_text_xyrgb(&hello, textures.font_map, 50.0f, 250.0f,  (0.2f + sin(2.0f * glfwGetTime()))/2.0f,
                                                                 (0.2f + sin(M_PI + 2.0f * glfwGetTime()) / 2.0f), 0.6f);

//    draw_gui_to_texture();

//    draw_rectangle_t(0, 0, 64, 64, textures.tile_map);

//    draw_rectangle_whxy3s(textures.font_map, (point2i_t){140, 64}, (point2i_t){128, 164});
//    draw_rectangle_whxy3s(textures.font_map, (point2i_t){180, 64}, (point2i_t){108, 324});
//    draw_rectangle_whxy3s(textures.font_map, (point2i_t){180, 64}, (point2i_t){900-4, 500});
//    draw_rectangle_whxy3s(textures.font_map, (point2i_t){180, 64}, (point2i_t){0, 0});

    draw_wtext_xyrgb(&russia, mouse.x, mouse.y, (0.2f + sin(2.0f * glfwGetTime()))/2.0f,
                                                             (0.2f + sin(M_PI + 2.0f * glfwGetTime()) / 2.0f), 0.6f);



    graphics_check_error();
//    draw_rectangle_l(0, 0, 64, 64);
}

void render_frame_to_texture()
{
    if(texture_named_render_begin(&textures.mir_pick_map, 0, 0) < 0)
        return;

    mir_map_draw_pickmap();
    buttons_draw_pickmap();

    texture_named_render_end();
}



void graphics_check_error()
{

    static int i = 0;
    if(i == -1) return;

    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        printf("%s\n", error);
        i = -1;
    }
}

void draw_square()
{
    static float dx = 0.0f;
    dx+= 1.0f;
    if(dx > 800.0f)
        dx = 0.0f;

    glUseProgram(gl_program.id);

        float tmp[16];
        matrix_identity(tmp);
        matrix_translate(tmp, dx, 10.0f, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_identity());

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coords), text_coords);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void draw_texture(GLuint texture)
{
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, texture);

        draw_rectangle(10.0f, 10.0f, 512.0f, 512.0f);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void draw_rectangle(float x, float y, float w, float h)
{
    glUseProgram(gl_program.id);

        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, w, h, 0.0f);
        matrix_translate(tmp, x, y, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coords), text_coords);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void draw_rectangle_l(float x, float y, float w, float h)
{
    glUseProgram(gl_program.id);

        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, w, h, 0.0f);
        matrix_translate(tmp, x, y, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());

//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coords), text_coords);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);

    glLineWidth(4.0f);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, 4);
}


void draw_rectangle_t(float x, float y, float w, float h, GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(gl_program.id);

        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, w * 3, h, 0.0f);
        matrix_translate(tmp, x, y, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());

//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coordsx3), text_coordsx3);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coordsx3), text_coordsx3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_rectangle_whxy3s(GLuint texture, point2i_t wh, point2i_t xy)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(glp_3slices.id);

        float tmp[16];
        matrix_identity(tmp);
        // ??
        matrix_scale(tmp, wh.x, wh.y, 0.0f);
        matrix_translate(tmp, xy.x,  xy.y, 0.0f);

        glUniformMatrix4fv(glp_3slices.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(glp_3slices.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(glp_3slices.view, 1, GL_FALSE, camera_get_identity());



        glUniform1f(glp_3slices.u_rect_width, wh.x);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coordsx3), text_coordsx3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_rectangle_2p2i(point2i_t wh, point2i_t xy)
{
    glUseProgram(glp_base.id);

        float tmp[16];
        matrix_identity(tmp);
        // ??
        matrix_scale(tmp, wh.x, wh.y, 0.0f);
        matrix_translate(tmp, xy.x,  xy.y, 0.0f);

        glUniformMatrix4fv(glp_base.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(glp_base.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(glp_base.view, 1, GL_FALSE, camera_get_identity());

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(text_coords), text_coords);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void draw_rectangle_xy_wh_rgb(point2i_t xy, point2i_t wh, point3uc_t rgb)
{
//    here;
    glUseProgram(glp_text.id);

        float tmp[16];

        matrix_identity(tmp);
        matrix_scale(tmp, wh.x, wh.y, 0.0f);
        matrix_translate(tmp, xy.x, xy.y, 0.0f);

        glUniformMatrix4fv(glp_text.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(glp_text.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(glp_text.view, 1, GL_FALSE, camera_get_identity());
        glUniform1i(glp_text.is_solid_color, true);
    //    glUniform3f(glp_text.color, rgb.x / 255.0, rgb.y / 255.0, rgb.z / 255.0);
        glUniform3f(glp_text.color, rgb.x / 255.0, rgb.y / 255.0, rgb.z / 255.0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void draw_button(button_t* button)
{
    point2i_t tmp = button->position;
    tmp.y += -button->text.rect.y + font_get_height();

    draw_rectangle_2p2i(button->text.rect , tmp);
    draw_wtext_xyrgb(&button->text,
                     button->position.x, button->position.y,
                     button->text_color.x / 255, button->text_color.y / 255, button->text_color.z / 255);
}

void draw_button_pickmap(button_t* button)
{
    point2i_t tmp = button->position;
    tmp.y += -button->text.rect.y + font.height;

    draw_rectangle_xy_wh_rgb(tmp, button->text.rect, (point3uc_t){BUTTON_T, button->id, GUI_INDEX} );
}

void draw_label(label_t* label)
{
    point2i_t tmp = label->position;
    tmp.y += -label->text.rect.y + font.height;
    draw_rectangle_2p2i(label->text.rect, tmp);

    draw_wtext_xyrgb(&label->text, label->position.x, label->position.y, 1.0f, 0.4f, 0.2f);
}

void draw_label_pickmap(label_t* label)
{
    point2i_t tmp = label->position;
    tmp.y += -label->text.rect.y + font.height;
    draw_rectangle_xy_wh_rgb(tmp, label->text.rect, (point3uc_t){LABEL_T, label->id, GUI_INDEX} );
}

void draw_texture_part(GLuint texture)
{
    glUseProgram(gl_program.id);
    glBindTexture(GL_TEXTURE_2D, texture);
    static int x = 0; x++; x %= 3;
    static int y = 0; y++; y %= 2;
        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, 128.0f, 128.0f, 0.0f);
        matrix_translate(tmp, 512.0f+20.0f, 10.0f, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
//    glDisable(GL_BLEND);
}

void draw_texture_char(GLuint texture, unsigned char ch)
{
    glUseProgram(gl_program.id);
    glBindTexture(GL_TEXTURE_2D, texture);
    int x = info[ch].x0; int x1 = info[ch].x1;
    int y = info[ch].y0; int y1 = info[ch].y1;


        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, (x1 - x) * 2.0f, (y1 - y) * 2.0f, 0.0f);
        matrix_translate(tmp, 512.0f+20.0f, 10.0f, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());
            float t[] =
            {
                (float)x/512.0f,  (float)y1/512.0f,
                (float)x1/512.0f, (float)y1/512.0f,
                (float)x1/512.0f, (float)y/512.0f,
                (float)x/512.0f,  (float)y/512.0f,

            };
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_texture_text(char* text, int length, GLint texture)
{
    glUseProgram(gl_program.id);
    glBindTexture(GL_TEXTURE_2D, texture);

    float x = 128.0f, y = 256.0f;
    for(int i = 0; i < length; i++)
    {
        unsigned int ch = (unsigned int)text[i];
        if(ch == '\n')
        {
            y-= -24.0f;
            x = 128.0f;
            continue;
        }
        int x0 = info[ch].x0; int x1 = info[ch].x1;
        int y0 = info[ch].y0; int y1 = info[ch].y1;
        float xpos = x + 20.0f + info[ch].x_off;
        float ypos = y - ((y1-y0) - info[ch].y_off);

        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, (x1 - x0), (y1 - y0), 0.0f);
        matrix_translate(tmp, xpos, ypos, 0.0f);


        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());

            float t[] =
            {
                (float)x0  / 512.0f, (float)y1 / 512.0f,
                (float)x1 / 512.0f, (float)y1 / 512.0f,
                (float)x1 / 512.0f, (float)y0  / 512.0f,
                (float)x0  / 512.0f, (float)y0  / 512.0f,

            };
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        x += info[ch].advance;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

}

void draw_texture_text_xy(char* text, int length, GLint texture, float x_start, float y_start)
{
    glEnable(GL_BLEND);
    glUseProgram(gl_program.id);
    glBindTexture(GL_TEXTURE_2D, texture);

    float x = x_start;
    float y = y_start;

//        static float k = 1.0f;
//        k+=1.0f;
//        if(k > 10.0f)
//            k = 1.0f;


    for(int i = 0; i < length; i++)
    {
        unsigned int ch = (unsigned int)text[i];
        if(text[i] == '\n')
        {
            y-= font.height;
            x = x_start;
            continue;
        }
        int x0 = info[ch].x0; int x1 = info[ch].x1;
        int y0 = info[ch].y0; int y1 = info[ch].y1;

        float xpos = x + info[ch].x_off;
        float ypos = y - ((y1-y0) - info[ch].y_off);
//        print_f(xpos);
//        printf("%c\n", text[i]);

//        print_i(info[ch].x_off);


        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, (x1 - x0), (y1 - y0), 0.0f);

//        matrix_scale(tmp, k, k, 0.0f);
        matrix_translate(tmp, xpos, ypos, 0.0f);


        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_identity());

        float t[] =
        {
            (float)x0 / 512.0f, (float)y1 / 512.0f,
            (float)x1 / 512.0f, (float)y1 / 512.0f,
            (float)x1 / 512.0f, (float)y0 / 512.0f,
            (float)x0 / 512.0f, (float)y0 / 512.0f,

        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        x += info[ch].advance;

    }


//    printf("%f\n", xpos - x_start + info[text[length-1]].advance);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);

}

void draw_text_xy(text_t* text_s, GLint font_map, float xpos, float ypos)
{
    glEnable(GL_BLEND);
    glUseProgram(gl_program.id);
    glBindTexture(GL_TEXTURE_2D, font_map);

    float x = xpos;
    float y = ypos;
    char* text = text_s->text;
    int length = text_s->length;
    float tmp[16];

    if(text_s->background)
    {
        matrix_identity(tmp);
        matrix_scale(tmp, (float)text_s->rect.x, (float)text_s->rect.y, 0.0f);
        matrix_translate(tmp, xpos, ypos - text_s->rect.y + text_s->fl_h, 0.0f);

        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);


        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }



    for(int i = 0; i < length; i++)
    {
        unsigned int ch = (unsigned int)text[i];
        if(text[i] == '\n')
        {
            y-= font.height;
            x = xpos;
            continue;
        }
        int x0 = info[ch].x0; int x1 = info[ch].x1;
        int y0 = info[ch].y0; int y1 = info[ch].y1;

        float xpos = x + info[ch].x_off;
        float ypos = y - ((y1-y0) - info[ch].y_off);

        matrix_identity(tmp);
        matrix_scale(tmp, (x1 - x0), (y1 - y0), 0.0f);
        matrix_translate(tmp, xpos, ypos, 0.0f);


        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_identity());

        float t[] =
        {
            (float)x0 / 512.0f, (float)y1 / 512.0f,
            (float)x1 / 512.0f, (float)y1 / 512.0f,
            (float)x1 / 512.0f, (float)y0 / 512.0f,
            (float)x0 / 512.0f, (float)y0 / 512.0f,

        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        x += info[ch].advance;

    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);


}

void draw_text_xyrgb(text_t* text_s, GLint font_map, float xpos, float ypos, float r, float g, float b)
{
#define p glp_text
    glEnable(GL_BLEND);

    glUseProgram(glp_text.id);
    glBindTexture(GL_TEXTURE_2D, font_map);

    float x = xpos;
    float y = ypos;
    char* text = text_s->text;
    int length = text_s->length;
    float tmp[16];

    if(text_s->background)
    {
        matrix_identity(tmp);
        matrix_scale(tmp, (float)text_s->rect.x, (float)text_s->rect.y, 0.0f);
        matrix_translate(tmp, xpos, ypos - text_s->rect.y + font.height - FONT_Y_OFFSET, 0.0f);

        glUniformMatrix4fv(p.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_identity());
        glUniform1i(p.is_solid_color, true);
        glUniform3f(p.color, r, g, b);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glUniform1i(p.is_solid_color, false);

    for(int i = 0; i < length; i++)
    {
        unsigned int ch = (unsigned int)text[i];
        if(text[i] == '\n')
        {
            y-= font.height;
            x = xpos;
            continue;
        }
        int x0 = info[ch].x0; int x1 = info[ch].x1;
        int y0 = info[ch].y0; int y1 = info[ch].y1;

        float xpos = x + info[ch].x_off;
        float ypos = y - ((y1-y0) - info[ch].y_off);


//        if((y1-y0) - info[ch].y_off != 0)
//        {
//            printf("Char: %c\n", ch);
//            print_i();
//        }

        matrix_identity(tmp);
        matrix_scale(tmp, (x1 - x0), (y1 - y0), 0.0f);
        matrix_translate(tmp, xpos, ypos, 0.0f);


        glUniformMatrix4fv(p.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_identity());

        float t[] =
        {
            (float)x0 / ATLAS_TEXTURE_SIZE, (float)y1 / ATLAS_TEXTURE_SIZE,
            (float)x1 / ATLAS_TEXTURE_SIZE, (float)y1 / ATLAS_TEXTURE_SIZE,
            (float)x1 / ATLAS_TEXTURE_SIZE, (float)y0 / ATLAS_TEXTURE_SIZE,
            (float)x0 / ATLAS_TEXTURE_SIZE, (float)y0 / ATLAS_TEXTURE_SIZE,

        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        x += info[ch].advance;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
#undef p
}

void draw_wtext_xyrgb(wtext_t* text_s, float xpos, float ypos, float r, float g, float b)
{
#define p glp_text
    glBindVertexArray(0);
//    print_2f(xpos, ypos);
    draw_point(xpos, ypos);

    glBindVertexArray(vao);

    glEnable(GL_BLEND);

    glUseProgram(glp_text.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_font_map());

//    print_2i(xpos, ypos);

    float x = xpos;
    float y = ypos;
    wchar_t* text = text_s->text;
    int length = text_s->length;
    float tmp[16];



//    if(text_s->background)
//    {
//        matrix_identity(tmp);
//        matrix_scale(tmp, (float)text_s->rect.x, (float)text_s->rect.y, 0.0f);
//        matrix_translate(tmp, xpos, ypos - text_s->rect.y + font.height, 0.0f);
//
//        glUniformMatrix4fv(p.model, 1, GL_FALSE, tmp);
//        glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_projection());
//        glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_identity());
//        glUniform1i(p.is_solid_color, true);
//        glUniform3f(p.color, r, g, b);
//
//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    }

    glUniform1i(p.is_solid_color, false);

    for(int i = 0; i < length; i++)
    {
        unsigned int ch = (unsigned int)text[i];
        if(text[i] == '\n')
        {
            y-= font.height;
            x = xpos;
            continue;
        }
        if(text[i] == '\t')
        {
            x += (info[ch].x1 - info[ch].x0) * 4;
            continue;
        }

        int x0 = info[ch].x0; int x1 = info[ch].x1;
        int y0 = info[ch].y0; int y1 = info[ch].y1;

        float xpos = x + info[ch].x_off;
        float ypos = y - ((y1 - y0) - info[ch].y_off);

        matrix_identity(tmp);
        matrix_scale(tmp, (x1 - x0), (y1 - y0), 0.0f);
        matrix_translate(tmp, xpos, ypos + FONT_Y_OFFSET, 0.0f);


        glUniformMatrix4fv(p.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_identity());

//        float t[] =
//        {
//            (float)x0 / ATLAS_TEXTURE_SIZE, (float)y1 / ATLAS_TEXTURE_SIZE,
//            (float)x1 / ATLAS_TEXTURE_SIZE, (float)y1 / ATLAS_TEXTURE_SIZE,
//            (float)x1 / ATLAS_TEXTURE_SIZE, (float)y0 / ATLAS_TEXTURE_SIZE,
//            (float)x0 / ATLAS_TEXTURE_SIZE, (float)y0 / ATLAS_TEXTURE_SIZE,
//        };


        float t[] =
        {
            (float)x0 / ATLAS_TEXTURE_SIZE, (float)y1 / ATLAS_TEXTURE_SIZE, // 1
            (float)x1 / ATLAS_TEXTURE_SIZE, (float)y1 / ATLAS_TEXTURE_SIZE, // 2
            (float)x1 / ATLAS_TEXTURE_SIZE, (float)y0 / ATLAS_TEXTURE_SIZE, // 3
            (float)x0 / ATLAS_TEXTURE_SIZE, (float)y0 / ATLAS_TEXTURE_SIZE, // 4
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        x += info[ch].advance;

    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);

#undef p
}

void draw_tile(GLuint tile_map, int x0, int y0)
{
    glEnable(GL_BLEND);
    glUseProgram(gl_program.id);
    glBindTexture(GL_TEXTURE_2D, tile_map);

        float xpos = 64.0f;
        float ypos = 64.0f;
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float tmp[16];
        matrix_identity(tmp);
        matrix_scale(tmp, 64.0f, 64.0f, 0.0f);
        matrix_translate(tmp, xpos, ypos, 0.0f);


        glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
        glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
        glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());

        float t[] =
        {
            (float)x0 / 8.0f, (float)y1 / 8.0f,
            (float)x1 / 8.0f, (float)y1 / 8.0f,
            (float)x1 / 8.0f, (float)y0 / 8.0f,
            (float)x0 / 8.0f, (float)y0 / 8.0f,

        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void draw_screen_to_texture()
{
    texture_render_begin(kernel.window_size.x, kernel.window_size.y);

    mir_draw_map();

    texture_render_end();
}

void draw_map_to_texture()
{
    int size = mir_map_get_size() * 64;
    if(texture_named_render_begin(&textures.mir_pick_map, size, size) < 0)
    {
        log_msg_s(DEFAULT_C, "%s: failed.", __func__);
        return;
    }
    mir_map_draw_pickmap();
    texture_named_render_end();
}

void draw_gui_to_texture()
{
    if(texture_named_render_begin(&textures.mir_pick_map, 0, 0) < 0)
    {
        log_msg_s(DEFAULT_C, "%s: failed.", __func__);
        return;
    }
    gui_draw_pickmap();

    texture_named_render_end();
}

void draw_tile_to_texture()
{
    texture_render_begin(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
//    texture_render_begin(kernel.window_size.x, kernel.window_size.y);


//    mir_draw_tile(0, 0);
//        begin_draw_tiles_ex();
//        tile_t* t = mir_get_tile(0, 0);
//        mir_draw_tile_ex(0, 0, t);
//        end_draw_tiles_ex();

    texture_render_end();
}

void draw_point(float x, float y)
{
    glUseProgram(glp_point.id);

    float tmp[16];
    matrix_identity(tmp);
    matrix_translate(tmp, x, y, 0);

    glUniformMatrix4fv(glp_point.model, 1, GL_FALSE, tmp);
    glUniformMatrix4fv(glp_point.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_point.view, 1, GL_FALSE, camera_get_identity());


    glBindBuffer( GL_ARRAY_BUFFER, vbo_point );
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(0);

    glPointSize(6.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0 );
}

void draw_tile_ex_xy(int x, int y)
{
    static float _world_matrix[16], _map_matrix[16], _tile_matrix[16];
    // begin
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map_ex.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_tile_map());
    glBindVertexArray(vao);

    glUniformMatrix4fv(glp_texture_map_ex.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_texture_map_ex.view, 1, GL_FALSE, camera_get_view());

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);

    // prepare

    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(glp_texture_map_ex.model, 1, GL_FALSE, _world_matrix);

    // draw stuff

    glUniform1i(glp_texture_map_ex.field, 0);
    glUniform1i(glp_texture_map_ex.unit, 16);
    glUniform1i(glp_texture_map_ex.landscape, 40);
    glUniform1i(glp_texture_map_ex.building, -1);
//    glUniform1d(, 0, 0);
//    glUniform2i(glp_texture_map_ex.texture_position, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // end
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void draw_tile_ex(tile_t* tile)
{
    static float _world_matrix[16], _map_matrix[16], _tile_matrix[16];
    // begin
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map_ex.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_tile_map());
    glBindVertexArray(vao);

    glUniformMatrix4fv(glp_texture_map_ex.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_texture_map_ex.view, 1, GL_FALSE, camera_get_view());

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);

    // prepare

    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, tile->x, tile->y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(glp_texture_map_ex.model, 1, GL_FALSE, _world_matrix);

    // draw stuff

    glUniform1i(glp_texture_map_ex.field, field_textures[(int)tile->entities[FIELD]]);
    glUniform1i(glp_texture_map_ex.unit, tile->unit ? tile->unit->texture : -1);
    glUniform1i(glp_texture_map_ex.landscape, 40);
    glUniform1i(glp_texture_map_ex.building, -1);
//    glUniform1d(, 0, 0);
//    glUniform2i(glp_texture_map_ex.texture_position, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // end
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void _draw_tile_ex(tile_t* tile)
{
//    print_i((int)tile->entities[FIELD]);
//    print_i(field_textures[(int)tile->entities[FIELD]]);
    glUniform1i(glp_texture_map_ex.field, field_textures[(int)tile->entities[FIELD]]);
    glUniform1i(glp_texture_map_ex.unit, tile->entities[UNIT] ? tile->unit->texture : -1);
    glUniform1i(glp_texture_map_ex.unit_healt, tile->entities[UNIT] ? GET_TEXTURE(TM_HEALTH_POINT) + tile->unit->health : 0);
    glUniform1i(glp_texture_map_ex.landscape, tile->entities[LANDSCAPE] ? landscape_textures[(int)tile->entities[LANDSCAPE]] : -1);

    glUniform1i(glp_texture_map_ex.building, -1);
    if(tile->entities[UNIT])
        if(tile->unit->team == mir_get_turn())
            glUniform1i(glp_texture_map_ex.building, tile->unit->energy ? 21 : -1 );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void _draw_active_ex(int texture)
{
    glUniform1i(glp_texture_map_ex.field, texture);
    glUniform1i(glp_texture_map_ex.unit, -1);
    glUniform1i(glp_texture_map_ex.landscape, -1);
    glUniform1i(glp_texture_map_ex.building, -1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void _draw_tile_ex_xy(int x, int y)
{
    static float _world_matrix[16], _map_matrix[16], _tile_matrix[16];
    // begin
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_tile_map());
    glBindVertexArray(vao);

    glUniformMatrix4fv(glp_texture_map.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_texture_map.view, 1, GL_FALSE, camera_get_view());

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);

    // prepare

    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(glp_texture_map.model, 1, GL_FALSE, _world_matrix);

    // draw stuff

//    glUniform1d;
    glUniform2i(glp_texture_map.texture_position, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // end
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}



extern GLuint FramebufferName;
unsigned char gfx_get_pickmap_index(int x, int y, int* p0, int* p1)
{
    unsigned char t[4];
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, t);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(t[2]) {*p0 = (int)t[0]; *p1 = (int)t[1];}

    return t[2];
}



void graphics_get_pickmap_index(int x, int y)
{
    unsigned char pixel[4];
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    switch (pixel[2])
    {
        case NULL_INDEX:
        break;
//        case MAP_INDEX:
//
//            mir_map_handle_hovered(pixel[0], pixel[1]);
        break;

        case GUI_INDEX:
            gui_handle_hovered(pixel[0], pixel[1]);
        break;
    }
    return;

//    unhover: buttons_unhover();

//    float pixel[4];
//    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);


//    int index = 255 * pixel[2];
//
//    switch(index)
//    {
//        case MAP_INDEX:
//            mir_map_handle_hovered_f(pixel[0], pixel[1]);
//        break;
//
//        case GUI_INDEX:
//            gui_handle_hovered();
//        break;
//    }
}



GLuint LoadFile(char* FileName, const GLchar** t)
{
    int len = 8000;
    const GLchar* text = malloc(len);
    memset((char*)text, 0, len);

    FILE *f;
    f = fopen(FileName, "rb");
    if (!f)
    {
        printf("Error load %s\n", FileName);
        return 0;
    }
    fread( (char*)text, 1, 8000, f);
    *t = text;
    fclose(f);
    return 1;

}

GLuint LoadShader(char *file_name, GLuint type)
{
    const GLchar* text = NULL;

    if (LoadFile(file_name, &text) < 0)
        return -1;

    GLint shader = glCreateShader(type);
    glShaderSource(shader, 1, &text, NULL);
    glCompileShader(shader);
    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        printf("%s: %s\n", file_name, log);
        //printf("%s", text);
    }

    free((char*)text);
    return shader;
}


int LoadProgram(GLuint* ID, char* frag, char* vert)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader(vert, GL_VERTEX_SHADER);
    fragmentShader = LoadShader(frag, GL_FRAGMENT_SHADER);
    if( (vertexShader < 0)  || (fragmentShader < 0) )
    {
        return -1;
    }
    // Create the program object
    *ID = glCreateProgram();
    if(*ID == 0)
        return -1;
    glAttachShader(*ID, vertexShader);
    glAttachShader(*ID, fragmentShader);

    // Link the program
    glLinkProgram(*ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Check the link status
    glGetProgramiv(*ID, GL_LINK_STATUS, &linked);
    if(!linked)
	{
	    GLint infoLen = 0;
	    glGetProgramiv(*ID, GL_INFO_LOG_LENGTH, &infoLen);
	    if(infoLen > 1)
		{
		    char* infoLog = malloc(sizeof(char) * infoLen);
		    glGetProgramInfoLog(*ID, infoLen, NULL, infoLog);
		    fprintf(stderr, "Error linking program:\n%s\n", infoLog);
		    free(infoLog);

        }

	    glDeleteProgram(*ID);
	    return -1;
	}

	return 1;
}

static GLint _check_attr(GLint id, char* str)
{
    GLint t = glGetAttribLocation(id, str);
    if(t < 0)
    {
        log_msg_s(DEFAULT_C, "Can't find %s attr", str);
    }
    return t;
}

static GLint _check_uniform(GLint id, char* str)
{
    GLint t = glGetUniformLocation(id, str);
    if(t < 0)
    {
        log_msg_s(DEFAULT_C, "Can't find %s uniform", str);
    }
    return t;
}

#define CHECK_ATTR(a, b) _check_attr(a, b)
#define CHECK_UNIFORM(a, b) _check_uniform(a, b)

void program_init(gl_program_t* program)
{
    program->vertex_position = CHECK_ATTR(program->id, "vertex_position");
    program->texture_coordinates = CHECK_ATTR(program->id, "texture_coordinates");
    program->projection = CHECK_UNIFORM(program->id, "projection");
    program->model      = CHECK_UNIFORM(program->id, "model");
    program->view       = CHECK_UNIFORM(program->id, "view");

}

#define p glp_base
void glp_base_init()
{
    p.vertex_position = CHECK_ATTR(p.id, "vertex_position");
//    p.texture_coordinates = CHECK_ATTR(p.id, "texture_coordinates");
//    p.texture_position = CHECK_UNIFORM(p.id, "texture_position");

    p.projection = CHECK_UNIFORM(p.id, "projection");
    p.model      = CHECK_UNIFORM(p.id, "model");
    p.view       = CHECK_UNIFORM(p.id, "view");
//    glp_base.vertex_position = glGetAttribLocation(glp_base.id, "vertex_position");
//    print_i(glp_base.vertex_position);
}
#undef p

#define p glp_text
void glp_text_init()
{
    p.vertex_position = CHECK_ATTR(p.id, "vertex_position");
    p.texture_coordinates = CHECK_ATTR(p.id, "texture_coordinates");
    p.texture_position = CHECK_UNIFORM(p.id, "texture_position");

    p.projection = CHECK_UNIFORM(p.id, "projection");
    p.model      = CHECK_UNIFORM(p.id, "model");
    p.view       = CHECK_UNIFORM(p.id, "view");
    p.color      = CHECK_UNIFORM(p.id, "color");
    p.is_solid_color = CHECK_UNIFORM(p.id, "is_solid_color");
}
#undef p


#define ATTR(attr) p.attr = CHECK_ATTR(p.id, #attr)
#define UNIF(unif) p.unif = CHECK_UNIFORM(p.id, #unif)

void glp_texture_map_init()
{
#define p glp_texture_map
    ATTR(vertex_position);
    UNIF(texture_position);
    UNIF(projection);
    UNIF(model);
    UNIF(view);
#undef p
}

void glp_texture_map_s_init()
{
#define p glp_texture_map_s
    ATTR(vertex_position);
    UNIF(texture_position);
    UNIF(projection);
    UNIF(model);
    UNIF(view);
#undef p
}

void glp_texture_map_ex_init()
{
#define p glp_texture_map_ex
    ATTR(vertex_position);

    UNIF(field);
    UNIF(unit);
        UNIF(unit_healt);
    UNIF(landscape);
    UNIF(building);

    UNIF(projection);
    UNIF(model);
    UNIF(view);
#undef p
}

void glp_9slices_init()
{
#define p glp_9slices
//    ATTR(vertex_position);
//    ATTR(vertex_color);
//    UNIF(projection);
//    UNIF(view);
//    UNIF(model);
#undef p
}

void glp_3slices_init()
{
#define p glp_3slices
    ATTR(vertex_position);
//    ATTR(vertex_color);

    UNIF(u_rect_width);
    UNIF(projection);
    UNIF(view);
    UNIF(model);
#undef p
}

void glp_point_init()
{
#define p glp_point
    ATTR(vertex_position);

    UNIF(projection);
    UNIF(view);
    UNIF(model);
    UNIF(color);
#undef p
}

