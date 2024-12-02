#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;

uniform vec2 uPos;
uniform float uRotation; // Ugao rotacije u stepenima

void main()
{

	 // Pretvaranje ugla iz stepeni u radijane
    float radians = radians(uRotation);

	// Rotaciona matrica
    mat2 rotationMatrix = mat2(
        cos(radians), -sin(radians),
        sin(radians),  cos(radians)
    );

    // Transformacija vertiksa
    vec2 rotatedPos = rotationMatrix * inPos;
	
	gl_Position = vec4(rotatedPos + uPos, 0.0, 1.0);

	chTex = inTex;
}