#version 330 core
//uniform sampler2D texture;

//in vec2 texture_coord;
in vec3 _vertex_color;
out vec4 frag_color;

void main()
{
//	vec2 t = vec2(texture_coord.x , texture_coord.y);
//	frag_color = texture2D(texture, t);
//	frag_color = vec4(0.1, 0.2, 0.5, 1.0);
	
//	if()
	
	
	frag_color = vec4(_vertex_color.r / 500, 
					  _vertex_color.g / 500, 
					  _vertex_color.b / 500, 
					  1.0);
}
