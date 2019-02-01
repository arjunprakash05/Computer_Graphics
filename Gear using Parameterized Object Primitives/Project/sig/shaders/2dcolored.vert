# version 400

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec4 vColor;

uniform mat4 vProj;
uniform mat4 vLocf;
uniform float zCoord;

flat out vec4 Color;

void main ()
{
	Color = vColor / 255.0;
	gl_Position = vProj * ( vLocf * vec4 ( vPos.x, vPos.y, zCoord, 1.0 ) );
}
