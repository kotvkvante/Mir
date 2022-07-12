#version 330 core
uniform sampler2D texture;

//in vec2 texture_coord;
in float _rect_width;
in vec2 _vertex_color;

out vec4 frag_color;

#define _H 0.125

float map_x(float x, float ma)
{
	return x * 0.125 / ma;	
}

float map_x2(float x, float H)
{
	return (x-0.125) / (0.375 - 2*H)*0.125 + H;
}

float map(float x, float in_min, float in_max, float out_min, float out_max) 
{
  	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void main()
{
//	frag_color = vec4(_vertex_color.r, 
//					  _vertex_color.g, 
//					  _vertex_color.b, 
//					  1.0);
	
	float H = _H *  (3 * 64) / _rect_width ;
	
	frag_color = vec4(0.0, 
					  0.0, 
					  0.0, 
					  1.0);
	
	vec2 tmp = _vertex_color.xy;
	
	
	frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	
	
	if( H >= _vertex_color.x )
	{
		tmp.x = map(tmp.x, 0, H, 0, 0.125);
	}
	if(( H <= _vertex_color.x) && 
	   (_vertex_color.x <= 3 * 0.125 - H) )
	{
		tmp.x = map(tmp.x, H, 0.375- H, 0.125, 0.25);
	} 
	if(_vertex_color.x >= 3 * 0.125 - H ) 
	{
		tmp.x = map(tmp.x, 3 * 0.125 - H, 0.375, 0.25, 0.375);
	}	
	
	tmp+= vec2(1.0 / 8.0, 7.0 / 8.0);
	frag_color = texture2D(texture, tmp);
}
















