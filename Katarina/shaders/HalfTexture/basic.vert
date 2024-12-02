#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje

out vec2 chTex;

uniform vec2 uPos; // Pozicija tile-a u prostoru
uniform float uRotation; // Ugao rotacije u stepenima
uniform vec2 uTexOffset; // UV koordinate za teksture koje se dele

void main()
{

	 // Pretvaranje ugla iz stepeni u radijane
    float radians = radians(uRotation);

	// Rotaciona matrica
    mat2 rotationMatrix = mat2(
        cos(radians), -sin(radians),
        sin(radians),  cos(radians)
    );

     // Transformacija UV koordinata za rotaciju
    vec2 rotatedTex = rotationMatrix * (inTex - vec2(0.5)) + vec2(0.5); // Rotacija oko centra (0.5, 0.5)
	
	// Postavljanje pozicije vertiksa u svet (clip space)
    gl_Position = vec4(inPos + uPos, 0.0, 1.0);

    // Skaliranje i ofsetovanje UV koordinata za sprite sheet
    chTex = vec2(
        rotatedTex.x * (uTexOffset.y - uTexOffset.x) + uTexOffset.x, // X koordinata
        rotatedTex.y // Y koordinata ostaje ista
    );
    // chTex = inTex * (uTexOffset.y - uTexOffset.x) + uTexOffset.x; // za cetvrtinu
}