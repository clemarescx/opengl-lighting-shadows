#version 130

uniform bool lighting;
uniform vec3 colour;

in vec3 ex_Normal;
in vec3 ex_View;
in vec3 ex_Light;
out vec4 res_Color;

void main() {
    if(lighting) {
		vec3 v = normalize(ex_View);
		vec3 l = normalize(ex_Light);
		vec3 n = normalize(ex_Normal);
		vec3 h = normalize(v+l);
		float diff = max(0.f, dot(l, n));
		float spec = pow(max(0.f, dot(h, n)), 128.f);
	
		res_Color = diff * vec4(colour, 1.0f) + vec4(spec);
	} else {
		res_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}