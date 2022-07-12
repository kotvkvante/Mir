#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 _vertex_color;

void main()
{
	_vertex_color = vertex_color; 
	
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
