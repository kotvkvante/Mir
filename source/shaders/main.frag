#version 330 core
uniform sampler2D texture;


in vec2 texture_coord;

out vec4 frag_color;

void main()
{
	vec2 t = vec2(texture_coord.x , texture_coord.y);
	frag_color = texture2D(texture, t);
	frag_color.a = 1.0;
	
//	frag_color.rgb = vec3(texture_coord.x, 0.2, 0.4);
	


	//frag_color = vec4(frag_color.r - frag_color.a, 
	//				  frag_color.g - frag_color.a,
	//				  frag_color.b - frag_color.a,
	//				  1.0); 
}
