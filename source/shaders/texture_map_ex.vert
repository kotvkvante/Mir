#version 330 core

layout (location = 0) in vec3 vertex_position;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texture_coord;

void main()
{
	// _t.x = texture_position.x;
	// _t.y = texture_position.y;
	//
	// int id = gl_VertexID;

	texture_coord.x = float(( (3 - gl_VertexID) * gl_VertexID / 2) );
	texture_coord.y = float(int( !(bool( (gl_VertexID & 2) >> 1 )) ) );


	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
