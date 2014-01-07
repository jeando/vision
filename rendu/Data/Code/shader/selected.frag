#version 130
uniform sampler2D Texture;

//smooth in vec4 pos;
in vec2 pos;
void main(void)
{

		vec4 color = texture2D(Texture,pos);
//		color.xy = pos.xy;
		gl_FragColor = color;
}
