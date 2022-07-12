#version 330 core
//uniform sampler2D texture;

in vec2 texture_coord;

out vec4 frag_color;

#define MAP_INDEX 1
#define PICK_COLOR_MAP_INDEX_F MAP_INDEX / 255.0  
#define PCMI PICK_COLOR_MAP_INDEX_F

void main()
{
	//if(bool(solid_color))
	//{
	//	return;
	//}
	
	
	//vec2 t = vec2(float(texture_coord.x) / 8.0 , float(texture_coord.y) / 8.0);
	//frag_color = texture2D(texture, t);
	//frag_color = vec4(texture_coord.x / 8.0f, texture_coord.y / 8.0f, PCMI, 1.0);
	frag_color = vec4(texture_coord.x / 255.0f, texture_coord.y / 255.0f, PCMI, 1.0);
	//frag_color = vec4(texture_coord.x / 8.0f, texture_coord.y / 255.0f, PCMI, 1.0);
	
	
	//frag_color = vec4(0.3, 0.5, 1.0, 0.5);
	
}

