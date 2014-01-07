#version 130
uniform sampler2D Texture;

//smooth in vec4 pos;
in vec2 pos;
void main(void)
{
		gl_FragColor = texture2D(Texture,pos);
		//gl_FragColor.xy = pos/10.0f;
//		gl_FragColor.z = 0.0f;
//		gl_FragColor.w = 1.0f;

//		gl_FragColor *= 0.5f;
}
