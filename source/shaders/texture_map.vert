#version 330 core

layout (location = 0) in vec3 vertex_position;
//layout (location = 1) in vec2 texture_coordinates;

uniform ivec2 texture_position;
uniform int is_solid_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texture_coord;
//flat out int solid_color;

//#define SOLID_COLOR -1

void main()
{
	//if(texture_position.x == SOLID_COLOR)
	//{
	//	texture_coord.x = texture_position.x;
	//	texture_coord.y = texture_position.y;
	//	solid_color = 1;
	//} 
	//else
	//{
	texture_coord.x = (texture_position.x % 8) + ( (3 - gl_VertexID) * gl_VertexID / 2);
	texture_coord.y = (texture_position.x / 8) + int( !(bool( (gl_VertexID & 2) >> 1 ))  );	
	
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
