uniform sampler2D texture01;

uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform vec4 materialEmissive;
uniform float specularPower;

uniform vec3 lightDirection;
uniform vec3 cameraPosition;

varying vec2 outUv;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
	// Normalize interpolated normal vectors because they may be shorter than one
	vec3 normal = normalize(eyespaceNormal);
	// Calculate the relation of directions normal and light are pointing towards.
	// 1 = to each other
	// -1 = opposite directions
	float diffuseFactor = dot(normal, -lightDirection);
	vec4 diffuseColor = texture2D(texture01, outUv) * materialDiffuse * diffuseFactor;

	if (specularPower > 0.9)
	{
		// Calculate how light reflects from the camera's point of view
		vec3 surfaceToCamera = normalize(cameraPosition - eyespacePosition);
		// Calculate multiplier of how much specular color is received
		float specularFactor = dot(surfaceToCamera, reflect(lightDirection, normal));
		specularFactor = pow(max(0.0f, specularFactor), specularPower);
		vec4 specularColor = materialSpecular * specularFactor * diffuseFactor;

		gl_FragColor = materialAmbient + diffuseColor + materialEmissive + specularColor;
	}
	else
	{
		gl_FragColor = materialAmbient + diffuseColor + materialEmissive;	
	}
}