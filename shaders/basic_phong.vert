#version 130

uniform mat4 proj_mat;
uniform mat4 model_view_mat;
uniform mat3 normal_mat;
uniform vec3 light_position;

in  vec3 position;
in  vec3 normal;
out vec3 ex_Normal;
out vec3 ex_View;
out vec3 ex_Light;

void main() {
	vec4 pos = model_view_mat * vec4(position, 1.0);
	gl_Position = proj_mat * pos;
	ex_Normal = normal_mat * normal;
	ex_View =  -pos.xyz;
	ex_Light = light_position - pos.xyz;
}