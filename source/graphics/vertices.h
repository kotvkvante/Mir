#ifndef VERTICES_H_INCLUDED
#define VERTICES_H_INCLUDED

static const GLfloat point[] =
{
    0.0f, 0.0f, 0.0f,
};

static const GLfloat vert[] =
{
    0.0f, 0.0f, 0.0f, // 1
    1.0f, 0.0f, 0.0f, // 2
    1.0f, 1.0f, 0.0f, // 3
    0.0f, 1.0f, 0.0f, // 4
};

static const GLfloat vert1[] =
{
    0.5f, 0.5f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

static const GLfloat vert2[] =
{
    0.0f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
};

static const GLfloat vert3[] =
{
    0.0f, 0.0f, 0.0f,
};

static const GLfloat vert4[] =
{
    0.0f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.0f, -0.5f,
    0.0f, 0.0f, -0.5f,
};

static const GLfloat vert5[] =
{
    0.0f, 0.0f, -0.5f,
    0.0f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.0f, -0.5f,
};

static const GLfloat vert6[] =
{
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f,
};

static const GLfloat vert7[] =
{
    0.0f,  10.0f, 0.0f,
    10.0f, 10.0f, 0.0f,
    10.0f, 0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,
};

static const GLfloat vert8[] =
{
    -10.0f,  10.0f, 0.0f,
    10.0f, 10.0f, 0.0f,
    10.0f, -10.0f,  0.0f,
    -10.0f,  -10.0f,  0.0f,
};

static const GLfloat text_coords[] =
{
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};

static const GLfloat __text_coords[] =
{
    0.0f, 1.0f / 2,
    1.0f / 2, 1.0f / 2,
    1.0f / 2, 0.0f,
    0.0f, 0.0f,
};


static const GLfloat text_coordsx3[] =
{
    0.0f,        1.0f / 8.0f,
    3.0f / 8.0f, 1.0f / 8.0f,
    3.0f / 8.0f, 0.0f,
    0.0f, 0.0f,
};

static const GLfloat text_coords_8[] =
{
    0.0f,        1.0f / 8.0f,
    1.0f / 8.0f, 1.0f / 8.0f,
    1.0f / 8.0f, 0.0f,
    0.0f,        0.0f,
};



static const GLfloat text_coords1[] =
{
    0.0f,   512.0f,
    512.0f, 512.0f,
    512.0f, 0.0f,
    0.0f,   0.0f,
};

static const GLfloat vertices[] =
{
    -0.5f, -0.5f, 0.0f, // Left
     0.5f, -0.5f, 0.0f, // Right
     0.0f,  0.5f, 0.0f,  // Top
};


#endif // VERTICES_H_INCLUDED
