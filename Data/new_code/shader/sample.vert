#version 130

//in vec4 gl_Vertex;
//in vec4 gl_MultiTexCoord0;

in vec3 vertex;
//in vec2 multiTexCoord0;
in vec2 tex;
//multiTexCoord0;
out vec2 pos;
//smooth out vec4 pos;

uniform float tx;
uniform float wr;

void main(void)
{
	    pos = tex;
//	    pos.xy = tex;
	//	pos.zw= vec2(0.0f,1.0f);
//		gl_Position = pos;
//pos.z = vertex.z+0.5f;
		gl_Position.xyz = vertex;
		gl_Position.x += tx;
		gl_Position.x /= wr;
		gl_Position.w = 1.0f;
}

