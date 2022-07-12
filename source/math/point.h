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


#endif // POINT_H_INCLUDED
