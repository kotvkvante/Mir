#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define M_PI 3.14159265358979323846

#define true 1
#define false 0

#define math_sqr(a) ((a) * (a))
#define math_dist(x1, y1, x2, y2) ( math_sqr( (x1) - (x2) ) + math_sqr( (y1) - (y2) ) )

#define here printf("Here!\n")

#define print_i(a) printf("%s = %d\n", #a, a)
#define print_f(a) printf("%s = %f\n", #a, a)

#define print_2i(a, b) printf("{\n\t%s = %d\n\t%s = %d\n}\n", #a, a, #b, b);
#define print_2f(a, b) printf("{\n\t%s = %f\n\t%s = %f\n}\n", #a, a, #b, b);

#define print_3i(a, b, c) printf("{\n"); print_i(a); print_i(b); print_i(c); printf("}\n")
#define print_3f(a, b, c) printf("{\n"); print_f(a); print_f(b); print_f(c); printf("}\n")

#define print_p2f(p2) printf("%s{%f, %f}\n", #p2, p2.x, p2.y)
#define print_p2i(p2) printf("%s{%d, %d}\n", #p2, p2.x, p2.y)

#define rgb_to_float(a, b, c) (a) / 255.0f, (b) / 255.0f, (c) / 255.0f

int rand_index(int a, int b);
int rand_index_zero(int b);
#endif // UTILS_H_INCLUDED
