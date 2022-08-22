#version 330 core
uniform sampler2D texture;

uniform int field;
uniform int unit;
    uniform int unit_healt;
uniform int landscape;
uniform int building;

in vec2 texture_coord;
out vec4 frag_color;

void main()
{
    vec2 tc;
    vec4 field_color = vec4(0);
    vec4 unit_color = vec4(0);
    vec4 landscape_color = vec4(0);
    vec4 building_color = vec4(0);
    vec4 unit_healt_color = vec4(0);

    tc.x = texture_coord.x + field % 8;
    tc.y = texture_coord.y + field / 8;
    tc = vec2(tc.x / 8.0 , tc.y / 8.0);
    field_color = texture2D(texture, tc);

    if(unit > 0)
    {
        tc.x = texture_coord.x + unit % 8;
        tc.y = texture_coord.y + unit / 8;
        tc = vec2(tc.x / 8.0 , tc.y / 8.0);
        unit_color = texture2D(texture, tc);

        tc.x = texture_coord.x + unit_healt % 8;
        tc.y = texture_coord.y + unit_healt / 8;
        tc = vec2(tc.x / 8.0 , tc.y / 8.0);
        unit_healt_color = texture2D(texture, tc);
        unit_color = mix(unit_color, unit_healt_color, unit_healt_color.a);
    }

    if(landscape > 0)
    {
        tc.x = texture_coord.x + landscape % 8;
        tc.y = texture_coord.y + landscape / 8;
        tc = vec2(tc.x / 8.0 , tc.y / 8.0);
        landscape_color = texture2D(texture, tc);
    }

    if(building > 0)
    {
        tc.x = texture_coord.x + building % 8;
        tc.y = texture_coord.y + building / 8;
        tc = vec2(tc.x / 8.0 , tc.y / 8.0);
        building_color = texture2D(texture, tc);
    }

    frag_color = mix(field_color, landscape_color, landscape_color.a);
    frag_color = mix(frag_color, building_color, building_color.a);
    frag_color = mix(frag_color, unit_color, unit_color.a);
    // frag_color =  mix(frag_color, unit_color, unit_color.a);
}
