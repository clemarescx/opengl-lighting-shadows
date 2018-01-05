#version 330

out vec4 fragmentDepth;

void main(){
	fragmentDepth = vec4(vec3(1.0 - (1.0f - gl_FragCoord.z)*15.0), 1.f);
}