#version 330 core

layout (location = 0) in vec3 vertex_position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texture_coord;

#define D

void main()
{

#ifdef A
	const int a[8] = int[8](0, 1, 1, 1, 1, 0, 0, 0);
	texture_coord.x = a[2 * gl_VertexID + 0];
	texture_coord.y = a[2 * gl_VertexID + 1];
#elif defined(B)
	texture_coord.x = float(( (3 - gl_VertexID) * gl_VertexID / 2) );
	texture_coord.y = float(int( !(bool( (gl_VertexID & 2) >> 1 )) ) );
#elif defined(C)
	texture_coord.x = (3 - gl_VertexID) * gl_VertexID / 2;
	texture_coord.y = 2 + ~((gl_VertexID & 2) >> 1);
#elif defined(D)
	texture_coord.x = (gl_VertexID + 1) / 2 % 2;
	texture_coord.y = 1 - gl_VertexID / 2;	
#endif


	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
