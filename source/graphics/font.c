#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stdarg.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/stb/stb_image_write.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../kernel/error_handler.h"
#include "../utils/utils.h"
#include "../math/point.h"
#include "graphics.h"
#include "textures.h"
#include "font.h"

#define MAX_STR_LENGTH 256
#define MAX_WSTR_LENGTH 256

FT_Library ft2_library;
FT_Face    face;

//char* text_ = "Tell dog my dot me\nAAAaaa aAAafASfasfasf";
char* text_ = "Mir! \nby kotvkvahte";
wchar_t* wtext = L"Россия";

text_t hello;
wtext_t russia;

font_t font;
glyph_info_t info[NUM_GLYPHS];
//glyph_info_t cyrillic[NUM_GLYPHS];


static const char* getErrorMessage(FT_Error err)
{
    #undef FTERRORS_H_
    #define FT_ERRORDEF( e, v, s )  case e: return s;
    #define FT_ERROR_START_LIST     switch (err) {
    #define FT_ERROR_END_LIST       }
    #include FT_ERRORS_H
    return "(Unknown error)";
}

#define FT2_CHECK_ERROR(error) \
if(error != 0) \
{\
    error_msg(DEFAULT_C, (char*)getErrorMessage(error));\
}


//static int _calc_str_width(char* str, int length);
static void _wtext_push_char(wtext_t* text, wchar_t c);

static point2i_t _calc_text_rect(text_t* text);
static point2i_t _calc_wtext_rect(wtext_t* text);

void texts_init()
{
//    text_init(&hello, text_);
    wtext_init(&russia, wtext);
}

void font_init_atlas()
{

    FT_Error error;

    error = FT_Init_FreeType(&ft2_library);
    FT2_CHECK_ERROR(error);

    error = FT_New_Face(ft2_library, "assets/fonts/consolas.ttf", 0, &face);
	FT2_CHECK_ERROR(error);

    // 16 = 1 >> 4
	error = FT_Set_Char_Size(face, 0, 16 * 64, 32*3, 32*3);
    FT2_CHECK_ERROR(error);

	// quick and dirty max texture size estimate
    // 2^6 = 64;
	int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(NUM_GLYPHS)) / 2;
	int tex_width = 1;
	while (tex_width < max_dim) tex_width <<= 1;
	int tex_height = tex_width;
    int max_off = 0;
	// render glyphs to atlas



	unsigned char* pixels = (unsigned char*)calloc(tex_width * tex_height, 1);
	int pen_x = 0, pen_y = 0;
    int i = 0;
	for( i = 0; i < NUM_GLYPHS; ++i){
		FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;


		if(pen_x + bmp->width >= tex_width){
			pen_x = 0;
			pen_y += ((face->size->metrics.height >> 6) + 1);
		}

		for(int row = 0; row < bmp->rows; ++row){
			for(int col = 0; col < bmp->width; ++col){
				int x = pen_x + col;
				int y = pen_y + row;
				pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		// this is stuff you'd need when rendering individual glyphs out of the atlas

		info[i].x0 = pen_x;
		info[i].y0 = pen_y;
		info[i].x1 = pen_x + bmp->width;
		info[i].y1 = pen_y + bmp->rows;

		info[i].x_off   = face->glyph->bitmap_left;
		info[i].y_off   = face->glyph->bitmap_top;

		if(info[i].y_off >= max_off)
		{
            max_off = info[i].y_off;
		}


		info[i].advance = face->glyph->advance.x >> 6;

		pen_x += bmp->width + 1;
	}

//    for(int i = 0; i < NUM_GLYPHS; ++i)
//    i++;
//    {
//		FT_Load_Char(face, 1099, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
//		FT_Bitmap* bmp = &face->glyph->bitmap;
//
//
//		if(pen_x + bmp->width >= tex_width){
//			pen_x = 0;
//			pen_y += ((face->size->metrics.height >> 6) + 1);
//		}
//
//		for(int row = 0; row < bmp->rows; ++row){
//			for(int col = 0; col < bmp->width; ++col){
//				int x = pen_x + col;
//				int y = pen_y + row;
//				pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
//			}
//		}
//
//		// this is stuff you'd need when rendering individual glyphs out of the atlas
//
////		info[i].x0 = pen_x;
////		info[i].y0 = pen_y;
////		info[i].x1 = pen_x + bmp->width;
////		info[i].y1 = pen_y + bmp->rows;
////
////		info[i].x_off   = face->glyph->bitmap_left;
////		info[i].y_off   = face->glyph->bitmap_top;
////		info[i].advance = face->glyph->advance.x >> 6;
//
//		pen_x += bmp->width + 1;
//	}
//    i--;

	// write png
	unsigned char* png_data = (unsigned char*)calloc(tex_width * tex_height * 4, 1);
	for(int i = 0; i < (tex_width * tex_height); ++i){
		png_data[i * 4 + 0] |= pixels[i];
		png_data[i * 4 + 1] |= pixels[i];
		png_data[i * 4 + 2] |= pixels[i];
        png_data[i * 4 + 3] |= pixels[i];
	}

    texture_load_font_map(tex_width, tex_height, (unsigned char*)png_data);
	stbi_write_png("font_output.png", tex_width, tex_height, 4, png_data, tex_width * 4);

	free(png_data);
	free(pixels);


//    font.height = face->height / 64;
//    print_i(face->max_advance_width / 64);

//    font.height = face->size->metrics.height / 64;
    FT_Pos a =  (face->size->metrics.ascender - face->size->metrics.descender) >> 6;
//    print_i(face->size->metrics.height >> 6);
//    print_i((face->size->metrics.ascender - face->size->metrics.descender) >> 6);
//    print_i(face->size);
//    print_i(font.height);
    font.height = a;

    font.y_offset = max_off - font.height ;

}

int font_get_height()
{
    return font.height;
}


//void font_get_char(unsigned long c)
//{
//    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
//
//    int ft_get_char_index = FT_Get_Char_Index(face, c);
//    printf("Code %d -> Glyph %d\n", c, ft_get_char_index);
//}




// String width in pixels
int _calc_first_line_y_offset(char* str, int length)
{
    int max_offset = 0;
    int k;
    for(int i = 0; i < length; i++)
    {
        if(str[i] == '\n') return max_offset;

        if(info[(int)str[i]].y_off > max_offset)
        {
            k = i;
            max_offset = info[(int)str[i]].y_off;
        }

    }
    return info[(int)str[k]].y1 - info[(int)str[k]].y0 - max_offset;
}

//static int _calc_str_width(char* str, int length)
//{
////    printf("%.*s\n", length, str);
//
//    int w = 0;
//    int i;
//
//    w -= info[(int)str[0]].x_off;
//    for(i = 0; i < length-1; i++)
//    {
////        printf("%c", str[i]);
//        int w_ = info[(int)str[i]].advance;
//        w += w_;
//    }
//
//    w += info[(int)str[i+1]].x1 - info[(int)str[i+1]].x0;
//    return w;
//}

static int _text_calc_wstr_width(wchar_t* str, int start, int length)
{
    int res = 0;
    int end = start + length;
    for(int i = start; i < end; i++)
    {
        res += info[(int)str[i]].advance;
    }
    return res;
}

static int _text_calc_str_width(text_t* text, int start, int length)
{
    int res = 0;
    int end = start + length;
    for(int i = start; i < end; i++)
    {
        res += info[(int)text->text[i]].advance;
    }

    return res;
}

static point2i_t _calc_text_rect(text_t* text)
{
    int line_count = 1;
    int char_count = 0;
    int char_count_max = 0;
    int char_index = 0;
    int i = 0;

    for(i = 0; i < text->length; i++)
    {
        char_count++;
        if(text->text[i] == '\n')
        {
            if(char_count > char_count_max)
            {
                char_count_max = char_count - 1;
                char_index = i - char_count + 1;
            }
            line_count++;
            char_count = 0;
        }

    }
    if(char_count > char_count_max)
    {
        char_count_max = char_count;
        char_index = i - char_count + 1;
    }

    int width = _text_calc_str_width(text, char_index, char_count_max);


    return (point2i_t){width, line_count * font.height};
}

static point2i_t _calc_wtext_rect(wtext_t* text)
{
    int line_count = 1;
    int char_count = 0;
    int char_count_max = 0;
    int char_index = 0;

    int i = 0;
//    if(text->text == NULL) error_msg(DEFAULT_C, "Text NULL");
//    wprintf(text->text[i]);
    while(text->text[i])
    {
        char_count++;
        if(text->text[i] == L'\t')
        {
            char_count+=3; // need to inc char_count by 4 (tab size)
                           // +=3 because of char_count++ already;
        }
        if(text->text[i] == L'\n')
        {
            if(char_count > char_count_max)
            {
                char_count_max = char_count - 1;
                char_index = i - char_count + 1;
            }
            line_count++;
            char_count = 0;
        }
        i++;
    }
    if(char_count > char_count_max)
    {
        char_count_max = char_count;
        char_index = i - char_count + 1;
    }

    int width = _text_calc_wstr_width(text->text, char_index, char_count_max);
    return (point2i_t){width, line_count * font.height};
}


//static point2i_t _calc_text_rect1(text_t* text)
//{
//    int max_str_len = 0, str_len = 0;
//    char* max_str = text->text;
//    int new_line_count = 0;
//    int i;
//    for(i = 0; i < text->length; i++)
//    {
//        if(text->text[i] == '\n')
//        {
//            if(str_len > max_str_len)
//            {
//                max_str_len = str_len - 1;
//                max_str     = &text->text[i - str_len + 1];
//            }
//            str_len = 0;
//            new_line_count++;
//        }
//        str_len++;
//    }
//
//    if(str_len > max_str_len)
//    {
//        max_str_len = str_len - 1;
//        max_str     = &text->text[i - str_len + 1];
//    }
//
////    printf("%c %d\n", *(max_str), max_str_len);
//
////    int p = _calc_str_width(max_str, max_str_len);
//    int t = _calc_first_line_y_offset(text->text, text->length);
////    print_i(font.height);
////    print_i(t);
//
//    text->fl_h = t;
////    new_line_count = new_line_count > 1 ? new_line_count - 1: new_line_count;
////    return (point2i_t){p, (new_line_count) * font.height + t};
//}

void wtext_init(wtext_t* text, wchar_t* str)
{
    text->length = wcslen(str);
//    print_i(text->length);
    text->text   = malloc((text->length + 1) * sizeof(wchar_t));
    wcscpy(text->text, str);

    if(text->text[text->length] != L'\0') error_msg(DEFAULT_C, "wtext_init");

    text->rect       = _calc_wtext_rect(text);
    text->background = true;
}


void wtext_set_text(wtext_t* text, const wchar_t* str)
{
    if(text->text != NULL) free(text->text);
//    wprintf(L"%d -> %ls\n",(text->length), str);

    text->length = wcslen(str);
    if(text->length > MAX_WSTR_LENGTH) error_msg(DEFAULT_C, "Text string length > MAX_WSTR_LENGTH");
    text->text = malloc( (text->length) * sizeof(wchar_t));

    wcsncpy(text->text, str, text->length );

//    if(text->text[text->length + 1] != L'\0')
//    {
//        wprintf(L">%ls<\n", text->text + text->length);
//        error_msg(DEFAULT_C, "Text last symbol is not L'\\0'");
//    }
//    else wprintf(L"Hi!\n");
//    wprintf(L"%ls\n", text->text);

    text->rect = _calc_wtext_rect(text);
}



void wtext_set_text_va(wtext_t* text, const wchar_t *format, ...)
{
    if(text->text != NULL) free(text->text);

    text->text = malloc(sizeof(wchar_t) * MAX_STR_LENGTH);

    va_list args;
    va_start(args, format);
    text->length = vswprintf(text->text, MAX_STR_LENGTH, format, args);
    va_end(args);

    text->rect = _calc_wtext_rect(text);
}

void wtext_set_text_ex(wtext_t* text, const wchar_t* format, const wchar_t* str)
{
    if(text->text != NULL) free(text->text);

    text->text = malloc(sizeof(wchar_t) * MAX_STR_LENGTH);
    text->length = swprintf(text->text, MAX_STR_LENGTH, format, str);

    text->rect = _calc_wtext_rect(text);
}

void wtext_set_text_ex_d(wtext_t* text, const wchar_t* format, int d)
{
    if(text->text != NULL) free(text->text);

    text->text = malloc(sizeof(wchar_t) * MAX_STR_LENGTH);
    text->length = swprintf(text->text, MAX_STR_LENGTH, format, d);

    text->rect = _calc_wtext_rect(text);
}

void text_init(text_t* text, char* str)
{
    text->text = malloc( (strlen(str) + 1) * sizeof(char));
    text->length = strlen(str);
    strcpy(text->text, str);
    text->text[text->length] = '\0';
    text->rect = _calc_text_rect(text);
    text->background = true;
}

static void _text_push_new_line(text_t* text)
{
    text_push_char(text, '\n');
}

void text_push_new_line(text_t* text)
{
    _text_push_new_line(text);
    text->rect = _calc_text_rect(text);
}


static void _text_pop_char(text_t* text)
{
    if(!text->length) return;
    text->length -= 1;
    text->text = realloc( text->text , (text->length + 1) * sizeof(char));
//    text->text[text->length - 1] = c;
    text->text[text->length ] = '\0';

}

void text_pop_char(text_t* text)
{
    _text_pop_char(text);
    text->rect = _calc_text_rect(text);
}


static void _text_push_char(text_t* text, char c)
{
//    printf("%p\n", text);
//    printf("%s\n", text);

//    if(text->length < 64)
//    {
//        text->text = (char*)realloc((void*)text->text, 64 * sizeof(char));
//        text->text[text->length] = c;
//        text->length = 64;
//    }

//    print_i(text->length);

//    text->text = (char*)realloc( text->text, (text->length + 1) * sizeof(char));
//    text->text[text->length] = c;
//    text->text[text->length + 1] ='\0';
//    strcpy(text->text, str);
    text->length += 1;
    text->text = realloc( text->text , (text->length + 1) * sizeof(char));
    text->text[text->length - 1] = c;
    text->text[text->length ] = '\0';


}

static void _wtext_push_char(wtext_t* text, wchar_t c)
{
    text->length += 1;
    text->text = realloc( text->text , (text->length + 1) * sizeof(wchar_t));
    text->text[text->length - 1] = c;
    text->text[text->length ] = '\0';
}



void text_push_char(text_t* text, unsigned char c)
{
    _text_push_char(text, c);
    text->rect = _calc_text_rect(text);
}

void wtext_push_char(wtext_t* text, unsigned int c)
{
//    printf("%ls", (wchar_t)c);

    _wtext_push_char(text, (wchar_t)c);
    text->rect = _calc_wtext_rect(text);
}



