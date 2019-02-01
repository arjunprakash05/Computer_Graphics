# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

uniform mat4 vProj;
uniform mat4 vView;

out vec3 Pos;
out vec3 Norm;

void main ()
{
	vec4 p4 = vView * vec4(vPos,1.0f); // vertex pos in eye coords
	Pos = p4.xyz / p4.w;
	Norm = normalize ( transpose(inverse(mat3(vView)))*vNorm );
	gl_Position = vProj * vec4(Pos,1.0);
}
