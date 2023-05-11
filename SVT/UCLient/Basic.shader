#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
//layout(location = 2) in vec4 u_Color;

//uniform mat4 model;     //局部-->世界
//uniform mat4 view;      //世界-->相机观察的空间

//out vec4 v_Color;
out vec2 v_TexCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

//uniform sampler2D u_Texture;

void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 1.0f);
	gl_Position = proj * view * model * vec4(position, 1.0f);
	v_TexCoord = texCoord;
	//v_Color = u_Color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
//in vec4 v_Color;

uniform sampler2D v_Texture;

void main()
{
	vec4 texColor = texture(v_Texture, v_TexCoord);
	color = texColor;
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}