#version 330

uniform mat4 model_view_projection;
uniform vec3 light_position;
uniform vec3 camera_position;


in vec3 position;
in vec3 normal;

out vec3 ex_view;
out vec3 ex_light;
out vec3 ex_normal;


void main() {
	
	gl_Position = model_view_projection * vec4(position, 1.f);

	ex_view = normalize(camera_position - position); 
	ex_light = normalize(light_position - position);
	ex_normal = normalize(normal);
}
