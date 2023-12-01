attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

varying vec2 outUv;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
	outUv = uv;
	vec4 vertexPosition = vec4(position, 1.0);
	eyespacePosition = (modelMatrix * vertexPosition).xyz;
	eyespaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
	gl_Position = modelViewProjectionMatrix * vertexPosition;
}