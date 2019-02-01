# version 400

in  vec4 Color;
in  vec2 Texc;
out vec4 fColor;

uniform sampler2D TexId; // diffuse color texture

void main()
{
	vec4 kd = texture2D ( TexId, Texc );
	fColor = Color*0.1 + 0.9*kd;
} 
