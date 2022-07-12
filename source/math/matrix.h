#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

void matrix_scale(float*, float, float, float );

void matrix_frustum(float* matrix, float left, float right, float bottom, float top, float zNear, float zFar);
void matrix_perspective(float* matrix, float fieldOfView, float aspectRatio, float zNear, float zFar);

//void matrix_look_at(float*, TVector3d*, TVector3d*, TVector3d* );
void matrix_ortho(float*, float, float, float, float, float, float);
void matrix_translate(float*, float, float, float );

void matrix_rotate_x(float*, float);
void matrix_rotate_y(float*, float);
void matrix_rotate_z(float*, float);

void matrix_identity(float* );
void matrix_multiply(float* destination, float* operand1, float* operand2);

float to_radians(float degrees);
int copyMatrix_fd(float* , double*);
int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate);
void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2);
void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector);
int glhInvertMatrixf2(float *m, float *out);


void matrix_print(float*);
#endif // MATRIX_H_INCLUDED
