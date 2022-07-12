#version 330 core

layout (location = 0) in vec3 vertex_position;
//layout (location = 1) in vec2 texture_coordinates;
//uniform vec2 texture_position;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


//out vec2 texture_coord;

void main()
{
//	texture_coord.x = texture_coordinates.x + texture_position.x;
//	texture_coord.y = texture_coordinates.y + texture_position.y;

	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
