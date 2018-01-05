#version 330

uniform mat4 model_mat;

uniform mat4 view_proj_mat;
uniform vec3 light_position;
uniform vec3 camera_position;


in vec3 position;
in vec3 normal;

out vec3 v;
out vec3 l;
out vec3 n;
out vec3 cube_tex_coord;

uniform mat4 shadow_MVP;
out vec4 ex_lightSpaceCoords;

void main() {
	vec4 pos = model_mat * vec4(position, 1.f);
	gl_Position = view_proj_mat * pos;

	v = normalize(camera_position - position); 
	l = normalize(light_position - position);
	n = normalize(normal);

	cube_tex_coord = position;
	
	ex_lightSpaceCoords = shadow_MVP * pos;
}
