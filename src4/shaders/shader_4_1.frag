#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDirection;
uniform vec3 cameraPos;


in vec3 interpNormal;
in vec3 positions;

void main()
{
	float intensiveness = dot(-lightDirection, interpNormal);
	intensiveness = max(intensiveness, 0.0);
	
	//gl_FragColor = vec4(objectColor*intensiveness, 1.0);
	
	vec3 v = normalize(cameraPos-positions);
	vec3 r = normalize(reflect(lightDirection, interpNormal));
	
	float intense = dot(v, r);
	intense = max(intense, 0.0);
	intense = pow(intense, 1000);
	gl_FragColor = vec4(objectColor*intensiveness+vec3(1.0)*intense, 1.0);
}





