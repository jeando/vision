#version 130
uniform sampler2D Texture;

//smooth in vec4 pos;
in vec2 pos;
uniform float selected;
void main(void)
{

		if(selected==0.0f)
		{
			gl_FragColor = texture2D(Texture,pos)/3.0f;
		}
		else
		{
			gl_FragColor = texture2D(Texture,pos);
		}
}
