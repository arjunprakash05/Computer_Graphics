# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec4 vColor;

uniform mat4 vProj;
uniform mat4 vLocf;

out vec3 Pos;
out vec4 Color;
out vec3 Norm;

void main ()
{
	vec4 p4 = vLocf * vec4(vPos,1.0f); // vertex pos in eye coords
	Pos = p4.xyz / p4.w;
	Color = vColor / 255.0;
	Norm = normalize ( transpose(inverse(mat3(vLocf)))*vNorm );
	gl_Position = vProj * vec4(Pos,1.0);
}
