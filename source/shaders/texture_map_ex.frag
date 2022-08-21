#version 330 core
uniform sampler2D texture;

uniform int texture_position;
uniform int unit;
// flat in int id;
// flat in ivec2 _t;

in vec2 texture_coord;
out vec4 frag_color;


void main()
{
    vec2 _t;
    _t.x = texture_coord.x + texture_position % 8;
    _t.y = texture_coord.y + texture_position / 8;

    vec2 t = vec2(float(_t.x) / 8.0 , float(_t.y) / 8.0);


	frag_color = texture2D(texture, t);


    vec2 _unit;
    _unit.x = texture_coord.x + unit % 8;
    _unit.y = texture_coord.y + unit / 8;
    t = vec2(float(_unit.x) / 8.0 , float(_unit.y) / 8.0);

    vec4 unit_color = texture2D(texture, t);


    frag_color =  mix(frag_color, unit_color, unit_color.a);
}
