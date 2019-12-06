#version 140

in vec3 inPos;
in vec2 inTex;

out vec2 tex_st;


const float PI   = 3.1415926535897932384626433832795;
const float PI_2 = 1.57079632679489661923;
const float PI_4 = 0.785398163397448309616;

const float fov = 45.0 * PI/180.0;   // 45 degs
const float a   = 640.0/480.0;       // aspect ratio
const float n   = 0.1;               // near
const float f   = 100.0;             // far
const float d   = 1.0 / tan(fov/2.0);

mat4 translate(vec3 p) {
  return mat4(
    1.0, 0.0,  0.0, 0.0,
    0.0, 1.0,  0.0, 0.0,
    0.0, 0.0,  1.0, 0.0,
    p.x, p.y, p.z, 1.0
  );
}

uniform mat4 model = mat4(1.0);
uniform mat4 view  = mat4(
  1.0, 0.0,  0.0, 0.0,
  0.0, 1.0,  0.0, 0.0,
  0.0, 0.0,  1.0, 0.0,
  0.0, 0.0, -3.0, 1.0);
uniform mat4 proj  = mat4(
  d/a, 0.0,             0.0,  0.0,
  0.0,   d,             0.0,  0.0,
  0.0, 0.0,     (n+f)/(n-f), -1.0,
  0.0, 0.0, (2.0*n*f)/(n-f),  0.0);


void main()
{
  gl_Position = proj * view * model * vec4(inPos, 1.0);

  tex_st = inTex;
}