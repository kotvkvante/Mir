#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 texture_coordinates;

uniform float u_rect_width;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 _vertex_color;
out float _rect_width;
void main()
{
	_rect_width = u_rect_width;
	_vertex_color = texture_coordinates; 
	
	
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
