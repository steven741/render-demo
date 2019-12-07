#include <stdio.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>


typedef struct __attribute__((packed)) {
  float x,y,z;
} XYZ;

typedef struct __attribute__((packed)) {
  float s,t;
} UV;

typedef struct __attribute__((packed)) {
  XYZ pos; UV tex;
} Vector;

Vector vertices[] = {
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


int validate_shader(GLuint shader) {
  int stat;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &stat);

  if (!stat) {
    char info[512];
    glGetShaderInfoLog(shader, 512, NULL, info);
    fprintf(stderr, "Shader Compile Log:\n%s", info);
  }
  return stat;
}


int validate_program(GLuint program) {
  int stat;
  glGetProgramiv(program, GL_LINK_STATUS, &stat);

  if (!stat) {
    char info[512];
    glGetProgramInfoLog(program, 512, NULL, info);
    fprintf(stderr, "Program Linker Log:\n%s", info);
  }
  return stat;
}


// Not for general use. Only used to load the specific bmp
// used in this project. Assumes little endian.
GLuint load_bmp(const char* filename) {
  typedef struct __attribute__((packed)) {
    char b,m;
    uint32_t size;
    uint16_t pad1,pad2;
    uint32_t offset;
  } BMPFILEHEADER;

  typedef struct __attribute__((packed)) {
    uint32_t        bV5Size;
    int32_t         bV5Width;
    int32_t         bV5Height;
    uint16_t        bV5Planes;
    uint16_t        bV5BitCount;
    uint32_t        bV5Compression;
    uint32_t        bV5SizeImage;
    int32_t         bV5XPelsPerMeter;
    int32_t         bV5YPelsPerMeter;
    uint32_t        bV5ClrUsed;
    uint32_t        bV5ClrImportant;
    uint32_t        bV5RedMask;
    uint32_t        bV5GreenMask;
    uint32_t        bV5BlueMask;
    uint32_t        bV5AlphaMask;
    uint32_t        bV5CSType;
    uint32_t        padding[9]; //not going to use
    uint32_t        bV5GammaRed;
    uint32_t        bV5GammaGreen;
    uint32_t        bV5GammaBlue;
    uint32_t        bV5Intent;
    uint32_t        bV5ProfileData;
    uint32_t        bV5ProfileSize;
    uint32_t        bV5Reserved;
  } BITMAPV5HEADER;

  typedef struct __attribute__((packed)) {
    uint8_t b,g,r;
  } BGR;

  BMPFILEHEADER bmp_header;
  BITMAPV5HEADER dib_header;
  BGR pixels[512*512];

  FILE* bmp_file = fopen(filename, "rb");
  fread(&bmp_header, sizeof(BMPFILEHEADER), 1, bmp_file);
  fread(&dib_header, sizeof(BITMAPV5HEADER), 1, bmp_file);
  fread(pixels, sizeof(BGR), 512*512, bmp_file);
  fclose(bmp_file);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	       dib_header.bV5Width, dib_header.bV5Height, 0,
	       GL_BGR, GL_UNSIGNED_BYTE, &pixels);
  return texture;
}



int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    fprintf(stderr, "Failed to start SDL2.\n SDL2: %s\n", SDL_GetError());

  SDL_Window* window = SDL_CreateWindow("opengl",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					640, 480,
					SDL_WINDOW_ALLOW_HIGHDPI |
					SDL_WINDOW_OPENGL);
  if (window == NULL)
    fprintf(stderr, "Failed to create window.\n SDL2: %s\n", SDL_GetError());

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		      SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  if (glcontext == NULL)
    fprintf(stderr, "Failed to get OpenGL context.\n SDL2: %s\n", SDL_GetError());

  printf("Vendor: %s\nRenderer: %s\nVersion: %s\nGLSL: %s\n",
	 glGetString(GL_VENDOR),
	 glGetString(GL_RENDERER),
	 glGetString(GL_VERSION),
	 glGetString(GL_SHADING_LANGUAGE_VERSION));

  glewInit();


  // Compile and use shader program.
  FILE* vertex_file = fopen("src/main.vert", "rb");
  FILE* fragment_file = fopen("src/main.frag", "rb");

  char vertex_source[2048];
  char fragment_source[2028];

  char* vertex_sources[1] = { vertex_source };
  char* fragment_sources[1] = { fragment_source };

  for (int i=0, c='\0'; c != EOF && i < 2048; ++i)
    if ((c=fgetc(vertex_file)) != EOF)
      vertex_source[i] = c;
    else
      vertex_source[i] = '\0';

  for (int i=0, c='\0'; c != EOF && i < 2048; ++i)
    if ((c=fgetc(fragment_file)) != EOF)
      fragment_source[i] = c;
    else
      fragment_source[i] = '\0';

  fclose(vertex_file);
  fclose(fragment_file);

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertex_shader, 1, (const GLchar **)vertex_sources, NULL);
  glShaderSource(fragment_shader, 1, (const GLchar **)fragment_sources, NULL);
  glCompileShader(vertex_shader);
  glCompileShader(fragment_shader);

  validate_shader(vertex_shader);
  validate_shader(fragment_shader);

  GLuint program_shader = glCreateProgram();
  glAttachShader(program_shader, vertex_shader);
  glAttachShader(program_shader, fragment_shader);
  glLinkProgram(program_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  validate_program(program_shader);

  glUseProgram(program_shader);


  // Setup buffer objects and memory for geometry
  GLuint vao;
  GLuint vbo;
  GLint shader_pos_loc = glGetAttribLocation(program_shader, "inPos");
  GLint shader_tex_loc = glGetAttribLocation(program_shader, "inTex");

  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(shader_pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector), NULL);
  glEnableVertexAttribArray(shader_pos_loc);
  glVertexAttribPointer(shader_tex_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vector), (void*)sizeof(XYZ));
  glEnableVertexAttribArray(shader_tex_loc);


  //
  GLint shader_mPos_loc      = glGetAttribLocation(program_shader, "modelPos");
  GLint shader_mSca_loc      = glGetAttribLocation(program_shader, "modelScale");
  GLint shader_mRotAxis_loc  = glGetAttribLocation(program_shader, "modelRotAxis");
  GLint shader_mRotAngle_loc = glGetAttribLocation(program_shader, "modelRotAngle");

  glVertexAttrib3f(shader_mPos_loc, 0.0f, 0.0f, 0.0f);
  glVertexAttrib3f(shader_mSca_loc, 1.0f, 1.0f, 1.0f);
  glVertexAttrib3f(shader_mRotAxis_loc, 0.0f, 0.0f, 0.0f);
  glVertexAttrib1f(shader_mRotAngle_loc, 0.0f);


  // Load an image
  GLuint tex = load_bmp("src/container.bmp");


  // Setup OpenGL state
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, 640, 480);
  glClearColor(0.13f, 0.10f, 0.10f, 1.0f);


  SDL_Event event;

  float t = 0.0f;

  for (;;) {
    Uint64 t0 = SDL_GetPerformanceCounter();

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	glDeleteTextures(1, &tex);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program_shader);

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
      }
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glVertexAttrib3f(shader_mRotAxis_loc, 0.0f, 1.0f, 0.0f);
    glVertexAttrib1f(shader_mRotAngle_loc, t);
    glVertexAttrib3f(shader_mPos_loc, 0.0f, 0.0f, 0.0f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mRotAxis_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 20.0f);
    glVertexAttrib3f(shader_mPos_loc, 2.0f, 5.0f, -15.0f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 20.0f);
    glVertexAttrib3f(shader_mPos_loc, 2.0f, 5.0f, -15.0f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 40.0f);
    glVertexAttrib3f(shader_mPos_loc, -1.5f, -2.2f, -2.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 60.0f);
    glVertexAttrib3f(shader_mPos_loc, -3.8f, -2.0f, -12.3f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 80.0f);
    glVertexAttrib3f(shader_mPos_loc, 2.4f, -0.4f, -3.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 100.0f);
    glVertexAttrib3f(shader_mPos_loc, -1.7f,  3.0f, -7.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 120.0f);
    glVertexAttrib3f(shader_mPos_loc, 1.3f, -2.0f, -2.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 140.0f);
    glVertexAttrib3f(shader_mPos_loc, 1.5f,  2.0f, -2.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 160.0f);
    glVertexAttrib3f(shader_mPos_loc, 1.5f,  0.2f, -1.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glVertexAttrib3f(shader_mPos_loc, 1.0f, 0.3f, 0.5f);
    glVertexAttrib1f(shader_mRotAngle_loc, t + 180.0f);
    glVertexAttrib3f(shader_mPos_loc, -1.3f,  1.0f, -1.5f);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    SDL_GL_SwapWindow(window);

    // Calculate dt
    Uint64 t1 = SDL_GetPerformanceCounter();
    float dt = (float)(t1-t0)/SDL_GetPerformanceFrequency();
    t += dt;
  }
}
