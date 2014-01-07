#version 130

//in vec4 gl_Vertex;
//in vec4 gl_MultiTexCoord0;

in vec3 vertex;
//in vec2 multiTexCoord0;
in vec2 tex;
//multiTexCoord0;
//uniform float sy;
//uniform float sx;
//smooth out vec4 pos;
out vec2 pos;
void main(void)
{
	    pos = tex;
	//	pos.zw= vec2(0.0f,1.0f);
//		gl_Position = pos;
//pos.z = vertex.z+0.5f;
//*
		gl_Position.xyz = vertex;
		gl_Position.w = 1.0f;
//		gl_Position.x *= sx;
//		gl_Position.y *= sy;
//*/
}

