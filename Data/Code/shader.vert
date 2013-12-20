//const float ratio = 0.5f;
const float ratio = 1.0f;
//const float ratio = 2.0f;
//const float ratio = 0.01f;
void main(void)
{
		gl_TexCoord[0] = gl_MultiTexCoord0;
	    gl_Position = gl_Vertex;
		gl_Position.y *=-1.0f;

//		gl_Position.y += 1.0f;

	gl_Position.x*=ratio;
//	gl_Position.y*=0.5f;
	gl_Position.y*=ratio;
//gl_Position.x-=1.0f;
}

