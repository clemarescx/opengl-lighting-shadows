#version 330

uniform vec3 colour;

in vec3 ex_view;
in vec3 ex_light;
in vec3 ex_normal;

void main() {
	vec3 l = normalize(ex_light);
	vec3 h = normalize(normalize(ex_view) + l);
	vec3 n = normalize(ex_normal);

	float spec = pow(max(0.f, dot(h, n)), 128.f);
	vec3 diffuse = vec3( max( 0.f, dot(l, n) ) );

	gl_FragColor = vec4( vec3(diffuse * colour) + vec3(spec), 1.f);
	
}
