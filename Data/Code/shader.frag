
uniform sampler2D Texture;
void main(void)
{
//		gl_FragColor = texture2D(Texture,gl_TexCoord[0].xy);
		vec4 color = texture2D(Texture,gl_TexCoord[0].st);
		gl_FragColor = color/2.0f;


		gl_FragColor.a = 1.0f;


//		gl_FragColor = texture2D(Texture,gl_FragCoord.xy);
//		gl_FragColor.r = 1.0;
//		gl_FragColor = vec4(1.,0.,0.,1.);
//		gl_FragColor.rg = gl_TexCoord[0].st;
}
