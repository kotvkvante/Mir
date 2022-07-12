#version 330 core
uniform sampler2D texture;
uniform bool is_solid_color;
uniform vec3 color;

in vec2 texture_coord;

out vec4 frag_color;

void main()
{
	vec2 t = vec2(texture_coord.x, texture_coord.y);
	
	if(is_solid_color)
	{
		frag_color = vec4(color, 1.0);
		return;
	}
		
	frag_color = texture2D(texture, t);
}
