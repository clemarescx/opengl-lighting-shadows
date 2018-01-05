#version 330
uniform mat4 model_view_projection;
in vec3 position;

void main(){
	gl_Position = model_view_projection * vec4(position, 1.0f);
}