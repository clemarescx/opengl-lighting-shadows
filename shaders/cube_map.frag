#version 330

uniform samplerCube cubemap;
uniform sampler2DShadow shadow_map;

uniform vec3 colour;

in vec3 cube_map_coord;
in vec3 view;
in vec3 light;
in vec3 normal;
in vec4 lightSpaceCoords;

void main() {
	vec3 l = normalize(light);
	vec3 h = normalize(normalize(view) + l);
	vec3 n = normalize(normal);

	float spec = pow(max(0.f, dot(h, n)), 128.f);
	vec4 diffuse = vec4( vec3( max( 0.f, dot(l, n) ) ), 1.0f);

	diffuse = texture(cubemap, cube_map_coord) * diffuse;
	
	float visibility = 1.0f; // visibility factor
	float depth = textureProj(shadow_map, lightSpaceCoords);

	if(depth < ((lightSpaceCoords.z)  / lightSpaceCoords.w)){
		visibility = 0.5f;
	}

	gl_FragColor = vec4( visibility * vec3(diffuse * colour + spec), 1.f);
	
}
