#version 420


//Lec5 (semester 1 of computer graphics)
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

//layout bindings for all the textures
layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D LUT1;
layout(binding = 2) uniform sampler2D LUT2;
layout(binding = 3) uniform sampler2D LUT3;
layout(binding = 4) uniform sampler2D table;
//light and camera positions
uniform vec3 lightPos;
uniform vec3 cameraPos;
//mode of operation variables
uniform int mode;
uniform int diffuseRamp;
uniform int specularRamp;
uniform int includeLUT1;
uniform int includeLUT2;
uniform int includeLUT3;

out vec4 frag_color;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);//use a pure white light

void main() { 


	// Lec5 -- Ambient
	
	float ambientStrength = 0.6;
	vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(lightPos - inPos);

	float d = max(dot(N, lightDir), 0.0); // we don't want negative diffuse
	if(mode == 5){//if in toon shading mode, clamp the light levels
	if(d < 0.3){
	d = 0.3;
	}
	else if(d >= 0.3 && d < 0.6){
	d = 0.6;
	}
	else if(d >= 0.6){
	d = 0.8;
	}
	}

	if(diffuseRamp == 1){//if using diffuse ramp, replace diffuse with value from 1D texture
	d = texture(table,vec2(d,0.5)).r;
	}
	vec3 diffuse = d * lightColor;

	//Attenuation
	float dist = length(lightPos - inPos);
	diffuse = diffuse / dist*dist;

	// Specular
	float specularStrength = 1.0;
	vec3 camDir = normalize(cameraPos - inPos);
	vec3 reflectedRay = reflect(-lightDir, N); // light direction to the point
	float spec = pow(max(dot(camDir, reflectedRay), 0.0), 128); // shininess coeficient

	if(specularRamp == 1){//if using specular ramp, replace specular with value from 1D texture
	spec = texture(table,vec2(spec,0.5)).r;
	}
	vec3 specular = specularStrength * spec * lightColor;
	vec4 lighting = vec4(ambient + diffuse + specular, 1.0);

	if(mode == 1){//if in no lighting mode, replace the calculated light with no light
	lighting = vec4(1,1,1,1);
	}
	if(mode == 2){//if in ambient lighting mode, replace the calculated light with the ambient only
		lighting = vec4(ambient,1);
	}
	if(mode == 3){//if in specular lighting mode, replace the calculated light with the specular only
		lighting = vec4(specular,1);
	}
	if(mode == 4){//if in ambient + specular lighting mode, replace the calculated light with ambient + specular
		lighting = vec4(ambient + specular,1);
	}
	


	//get texture colour
	vec4 texCol = texture(albedo, inUV);
	

	//get the amount of blue in the texture colour
	 float blueAmount = texCol.b * 63;

     vec2 quad1;//find the closest blue tile thats <= the blue value in the LUT
    quad1.y = floor(floor(blueAmount) / 8.0);
    quad1.x = floor(blueAmount) - (quad1.y * 8.0);

    vec2 quad2;//find the closest blue tile thats >= the blue value in the LUT
    quad2.y = floor(ceil(blueAmount) / 8.0);
    quad2.x = ceil(blueAmount) - (quad2.y * 8.0);

	//find the R and G values within the blue squares
     vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * texCol.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * texCol.g);

   

     vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * texCol.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * texCol.g);
   
   

   //make variables for the 2 colours to mix together
   //defaulted to the texture colour in case no LUT is applied
   vec4 newColour1 = texCol;
   vec4 newColour2 = texCol;
   float str = fract(blueAmount);//strength of mixing
   
   if(includeLUT1 == 1){//if using first LUT, sample using LUT1
    newColour1 = texture2D(LUT1, texPos1);
     newColour2 = texture2D(LUT1, texPos2);
	 str = 0.3;
   }
   else if(includeLUT2 == 1){//if using second LUT, sample using LUT2
    newColour1 = texture2D(LUT2, texPos1);
     newColour2 = texture2D(LUT2, texPos2);
	 str = 0.3;
   }
    else if(includeLUT3 == 1){//if using third LUT, sample using LUT3
    newColour1 = texture2D(LUT3, texPos1);
     newColour2 = texture2D(LUT3, texPos2);
	 str = 1.0;
   }
    
	//mix the two colours
     vec4 LUTCol = mix(texCol, newColour2, str);

	 //set the fragment colour
	frag_color = LUTCol * lighting;
}