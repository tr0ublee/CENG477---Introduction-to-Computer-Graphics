#include "helper.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

static GLFWwindow* win = NULL;
int widthWindow = 1000, heightWindow = 1000;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;

// Buffers
GLuint idVertexBuffer;
GLuint idIndexBuffer;

int textureWidth, textureHeight;
float heightFactor;

// Camera camera;
// Camera initialCamera;

glm::vec3 lightPos;
GLuint depthMapFBO;
GLuint depthCubemap;
bool lightPosFlag = false;

// Our own data starts from now on.
// place things that can change to global cuz it is ez
glm::mat4 MVP; 

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void createMapData(int height, int width, GLfloat* vertexPos, GLfloat* indices) {
  for (int j=0; j <= height; j++) {
    for (int i=0; i <= width; i++) {
      int x = i * j;
      vertexPos[x] = i;
      vertexPos[x+1] = 0;
      vertexPos[x+2] = j;
    }
  }
  for (int j=0; j < height; j++) {
    for (int i=0; i < width; i++) {
      int x = i * j;
      indices[x] = i;
      indices[x+1] = i + width;
      indices[x+2] = i + 1;
      indices[x+3] = i + 1;
      indices[x+4] = i + width;
      indices[x+5] = i + width + 1;
    }
  }
}

void initBuffers() {
  int vertexSize = (textureHeight+1) * (textureWidth+1);
  int indiceSize = textureHeight * textureWidth * 2 * 3;
  GLfloat* vertices = new GLfloat[vertexSize];
  GLfloat* indices = new GLfloat[indiceSize];
  createMapData(textureHeight, textureWidth, vertices, indices);
  glGenBuffers(1, &idVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &idIndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize * sizeof(GLfloat), indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);

}

void initCamLightMVP() {
  glm::vec3 pos = glm::vec3(textureWidth/2, textureWidth/10, -textureWidth/4); // given in the pdf
  glm::vec3 gaze = glm::vec3(0,0,1); // -w
  glm::vec3 up = glm::vec3(0,1,0); // up vector will be the y direction
  glm::mat4 viewMatrix = glm::lookAt(pos, pos+gaze, up);
  float angle = 45.0f;
  float aspectRatio = 1.0f;
  float near = 0.1f;
  float far = 1000.0f;
  glm::mat4 projectionMatrix = glm::perspective(angle, aspectRatio, near, far);
  MVP = projectionMatrix * viewMatrix;// No modeling transformation.
}

void initLight() {
  lightPos = glm::vec3(textureWidth/2, 100, textureWidth/2);

}


int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Please provide height and texture image files!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This might be used for local

  win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);

  if (!win) {
      glfwTerminate();
      exit(-1);
  }
  glfwMakeContextCurrent(win);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

      glfwTerminate();
      exit(-1);
  }

  std::string vertFile = "./shader.vert";
  std::string fragFile = "./shader.frag";
  initShaders(idProgramShader, vertFile, fragFile);
  glUseProgram(idProgramShader);

  glfwSetKeyCallback(win, keyCallback);

  initTexture(argv[1], argv[2], &textureWidth, &textureHeight);
  initCamLightMVP();
  initLight();
  initBuffers();

  while(!glfwWindowShouldClose(win)) {
    glfwSwapBuffers(win);
    glfwPollEvents();
  }


  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
