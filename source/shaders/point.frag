#version 330 core
uniform vec3 color;

out vec4 frag_color;

void main()
{
	frag_color = vec4(color, 1.0);
//	frag_color = vec4(0.6, 0.6, 1.0, 1.0);
}
