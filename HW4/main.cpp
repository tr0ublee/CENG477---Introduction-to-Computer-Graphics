#include "helper.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

#define HEIGHT_DELTA 0.5 // +R -F
#define TEXTURE_DELTA 1 // -Q +E
#define LIGHT_POS_DELTA 5 // +T -G 
#define PITCH_DELTA 0.05 //W S
#define YAW_DELTA  0.05 // A D 
#define CAM_SPEED_DELTA  0.01 // Y H
/*
  I = Reset
  p = full screen
*/


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
int textureDelta = 0;
float angle = 45.0f;
float speed = 0.0f;
float pitch = 0.0f;
float yaw = 90.0f;
glm::vec3 gaze;
glm::vec3 up;
glm::mat4 viewMatrix;

typedef struct vertexData {
  glm::vec3 pos;
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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
      // DONE
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_R && action == GLFW_REPEAT) {
      if (action == GLFW_REPEAT)
      // increase height factor
      // DONE
      heightFactor += HEIGHT_DELTA;
    }
    if (key == GLFW_KEY_F && action == GLFW_REPEAT) {
      // decrease height factor
      // DONE
      heightFactor -= HEIGHT_DELTA;
    }
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
      // move texture left
      // DONE
      textureDelta -= TEXTURE_DELTA;
    }
    if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
      // move texture to right
      // DONE
      textureDelta += TEXTURE_DELTA;
    }
    if (key == GLFW_KEY_T && action == GLFW_REPEAT) {
      // increase light height
      // DONE
      lightPos.y += LIGHT_POS_DELTA;
    }
    if (key == GLFW_KEY_G && action == GLFW_REPEAT) {
      // decreate light height
      // DONE
      lightPos.y -= LIGHT_POS_DELTA;
    }
    if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
      // change pitch
      pitch += PITCH_DELTA;
      if(pitch > 89.0f){
        pitch =  89.0f;
      }
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
      // change pitch
      pitch -= PITCH_DELTA;
      if(pitch < -89.0f) {
        pitch = -89.0f;
      }
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
      // change yaw
      yaw -= YAW_DELTA;
    }
    if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
      // change yaw
      yaw += YAW_DELTA;
    }
    if (key == GLFW_KEY_Y && action == GLFW_REPEAT) {
      // increase camera speed
      speed += CAM_SPEED_DELTA;
    }
    if (key == GLFW_KEY_H && action == GLFW_REPEAT) {
      // decrease camera speed
      speed -= CAM_SPEED_DELTA;
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
      // stop camera
      speed = 0.0f;
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
      // reset cemarea
      speed = 0.0f;
      pitch = 0.0f;
      yaw = 0.0f;
      camPos = glm::vec3(textureWidth/2.0, textureWidth/10.0, -textureWidth/4.0);
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      // IMPLEMENT FULL SCREEN 
    }
}

void createMapData() {
  for (int i = 0; i <= textureHeight; i++) {
    for (int j = 0; j <= textureWidth; j++) {
      Vertex pushed;
      pushed.pos = glm::vec3(j, 0.0, i);
      pushed.textureCoords = glm::vec2(1.0 - (((float)j)/textureWidth), 1.0 - (((float)i)/textureHeight));
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
      int width = textureWidth + 1;
      int x = width * i + j;
      int v0 = x;
      int v1 = x + width;
      int v2 = x + 1;
      int v3 = x + width + 1;
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*) (0 + sizeof(glm::vec3)));
  glEnableVertexAttribArray(1);
}

void initCamMVP() {
  camPos = glm::vec3(textureWidth/2.0, textureWidth/10.0, -textureWidth/4.0); // given in the pdf
  gaze = glm::vec3(0.0, 0.0, 1.0); // -w
  up = glm::vec3(0.0, 1.0, 0.0); // up vector will be the y direction
  viewMatrix = glm::lookAt(camPos, camPos+gaze, up);
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
  glUniform1i(wHandle, textureWidth);
  int hHandle = glGetUniformLocation(idProgramShader, "heightTexture");
  glUniform1i(hHandle, textureHeight);
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
  int heightHandle = glGetUniformLocation(idProgramShader, "heightTextureData");
  glUniform1i(heightHandle, 1);
  int textureDeltaHandle = glGetUniformLocation(idProgramShader, "textureDelta");
  glUniform1i(textureDeltaHandle, textureDelta);
}

void recalcCamMVP(){
    gaze.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    gaze.y = sin(glm::radians(pitch));
    gaze.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    gaze = glm::normalize((gaze));
    viewMatrix = glm::lookAt(camPos, camPos+gaze, up);
    camPos += speed * gaze;
    float aspectRatio = 1.0f;
    float near = 0.1f;
    float far = 1000.0f;
    glm::mat4 projectionMatrix = glm::perspective(angle, aspectRatio, near, far);
    MVP = projectionMatrix * viewMatrix;// No modeling transformation.
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
  initCamMVP();
  initLight();
  initBuffers();
  accessUniformVars();

  glEnable(GL_DEPTH_TEST);

  while(!glfwWindowShouldClose(win)) {
    recalcCamMVP();
    accessUniformVars();
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
