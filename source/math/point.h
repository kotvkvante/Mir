#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

typedef struct point2f_t
{
	float x, y;
} point2f_t;

typedef struct point2i_t
{
	int x, y;
} point2i_t;

typedef struct point2c_t
{
    char x, y;
} point2c_t;

typedef struct point3uc_t
{
    unsigned char x, y, z;
} point3uc_t;

typedef struct point3i_t
{
    int x, y, z;
} point3i_t;

typedef struct colori_rgb_t
{
    int r, g, b;
} colori_rgb_t;

typedef struct colorf_rgb_t
{
    float r, g, b;
} colorf_rgb_t;

#endif // POINT_H_INCLUDED
