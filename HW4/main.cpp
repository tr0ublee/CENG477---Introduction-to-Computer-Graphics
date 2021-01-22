#include "helper.h"

#include <vector>
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
float heightFactor = 10.0f;

// Camera camera;
// Camera initialCamera;

glm::vec3 lightPos;
GLuint depthMapFBO;
GLuint depthCubemap;
bool lightPosFlag = false;

// Our own data starts from now on.
// place things that can change to global cuz it is ez
glm::mat4 MVP; 
glm::vec3 camPos;
GLfloat* normals;
GLuint vaoHandle;


typedef struct vertexData {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 textureCoords;
} Vertex;

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void createMapData() {
  for (int i = 0; i <= textureHeight; i++) {
    for (int j = 0; j <= textureWidth; j++) {
      Vertex pushed;
      pushed.pos = glm::vec3(j,0.0,i);
      pushed.normal = glm::vec3(0.0,1.0,0.0);
      pushed.textureCoords = glm::vec2(1.0 - j*(1.0/textureWidth), 1.0 - i * (1.0/textureHeight));
      vertices.push_back(pushed);
    }
  }
  /*  x    x+1
      v0---v2
       | / | 
      v1---v3
      x+w  x+w+1
  */
  for (int i = 0 ; i < textureHeight; i++) {
    for (int j  = 0 ; j < textureWidth; j++) {
      int x = textureWidth * i + j;
      int v0 = x;
      int v1 = x + textureWidth;
      int v2 = x + 1;
      int v3 = x + textureWidth + 1;
      indices.push_back(v0);
      indices.push_back(v1);
      indices.push_back(v2);
      indices.push_back(v2);
      indices.push_back(v1);
      indices.push_back(v3);
    }
  }
}

void initBuffers() {
  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);
  createMapData();
  glGenBuffers(1, &idVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  
  glGenBuffers(1, &idIndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*) (0 + sizeof(glm::vec3)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*) (0 + 2 * sizeof(glm::vec3)));
  glEnableVertexAttribArray(2);
}

void initCamLightMVP() {
  camPos = glm::vec3(textureWidth/2.0, textureWidth/10.0, -textureWidth/4.0); // given in the pdf
  glm::vec3 gaze = glm::vec3(0.0, 0.0, 1.0); // -w
  glm::vec3 up = glm::vec3(0.0, 1.0, 0.0); // up vector will be the y direction
  glm::mat4 viewMatrix = glm::lookAt(camPos, camPos+gaze, up);
  float angle = 45.0f;
  float aspectRatio = 1.0f;
  float near = 0.1f;
  float far = 1000.0f;
  glm::mat4 projectionMatrix = glm::perspective(angle, aspectRatio, near, far);
  MVP = projectionMatrix * viewMatrix;// No modeling transformation.
}

void initLight() {
  lightPos = glm::vec3(textureWidth/2.0, 100.0, textureHeight/2.0);
}

void accessUniformVars() {
  int wHandle = glGetUniformLocation(idProgramShader, "widthTexture");
  glUniform1f(wHandle, textureWidth);
  int hHandle = glGetUniformLocation(idProgramShader, "heightTexture");
  glUniform1f(hHandle, textureHeight);
  int mvpHandle = glGetUniformLocation(idProgramShader, "MVP");
  glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, glm::value_ptr(MVP));
  int camPosHandle = glGetUniformLocation(idProgramShader, "cameraPosition");
  glUniform3fv(camPosHandle, 1, glm::value_ptr(camPos));
  int lightPosHandle = glGetUniformLocation(idProgramShader, "lightPos");
  glUniform3fv(lightPosHandle, 1, glm::value_ptr(lightPos));
  int heightFactorHandle = glGetUniformLocation(idProgramShader, "heightFactor");
  glUniform1f(heightFactorHandle, heightFactor);
  int rgbHandle = glGetUniformLocation(idProgramShader, "rgbTexture");
  glUniform1i(rgbHandle, 0);

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
  // glViewport(0,0, widthWindow, heightWindow);

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
  accessUniformVars();

  glEnable(GL_DEPTH_TEST);

  while(!glfwWindowShouldClose(win)) {
    glClearColor(0,0,0,1);
	  glClearDepth(1.0);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }


  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
