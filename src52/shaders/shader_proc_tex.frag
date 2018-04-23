#version 430 core

uniform vec3 objectColor1;
uniform vec3 objectColor2;
uniform vec3 lightDir;
uniform sampler2D sampler;

in vec3 interpNormal;
in vec2 vertexCoord;
in vec3 vertexPos;

void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	
	vec4 textureColor = texture2D(sampler, vertexCoord);
	float sine = sin(vertexPos.y);
	if (sine > 0) {
		gl_FragColor = vec4(objectColor1, 1.0);
	}
	else {
		gl_FragColor = vec4(objectColor2, 1.0);
	}
	
	
	
}