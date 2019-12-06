#version 140

in vec2 tex_st;

uniform sampler2D tex1;


void main() {
  gl_FragColor = texture(tex1, tex_st);
}