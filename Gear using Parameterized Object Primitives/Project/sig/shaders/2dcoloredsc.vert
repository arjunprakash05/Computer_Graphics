# version 400

layout (location = 0) in vec2 vPos;

uniform mat4 vProj;
uniform mat4 vLocf;
uniform float zCoord;
uniform vec4 vColor;

flat out vec4 Color;

void main ()
{
	Color = vColor;
	gl_Position = vProj * ( vLocf * vec4 ( vPos.x, vPos.y, zCoord, 1.0 ) );
}
