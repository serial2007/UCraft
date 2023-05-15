#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float a_light;
//layout(location = 2) in vec4 u_Color;

//uniform mat4 model;     //局部-->世界
//uniform mat4 view;      //世界-->相机观察的空间

//out vec4 v_Color;
out vec2 v_TexCoord;
out float v_light;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;


//uniform sampler2D u_Texture;

void main()
{
		//gl_Position = projection * view * model * vec4(aPos, 1.0f);
	if (position.z >= -0.05f)
	{
		gl_Position = proj * view * model * vec4(position, 1.0f);
		v_TexCoord = texCoord;
		v_light = a_light;
	}
	else
	{
		gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
		v_TexCoord = texCoord;
		v_light = a_light;
	}
	//v_Color = u_Color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
//in vec4 v_Color;
in float v_light;

uniform sampler2D v_Texture;

void main()
{
	vec4 texColor = texture(v_Texture, v_TexCoord);
	color = vec4(texColor.r * v_light, texColor.g * v_light, texColor.b * v_light, texColor.a);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}