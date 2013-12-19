void main(void)
{
		gl_TexCoord[0] = gl_MultiTexCoord0;
	    gl_Position = gl_Vertex;
		gl_Position.y *=-1.0f;
}

