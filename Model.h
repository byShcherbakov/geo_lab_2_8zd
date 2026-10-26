#pragma once
#include "libs.h"
struct Vertex { glm::vec3 position; glm::vec3 color; };
struct DrawRange { GLint first; GLsizei count; };
struct RenderOptions {
 bool smooth=false;
 bool antialias=true;
 bool mirror=false;
 float pointSize=24.0f;
 float lineWidth=4.0f;
};
class Model {
 GLuint vao=0, vbo=0, program=0;
 GLenum mode=GL_TRIANGLES;
 std::vector<DrawRange> ranges;
 int faceMode=0;
public:
 Model(GLFWwindow*);
 Model(const Model&)=delete;
 Model& operator=(const Model&)=delete;
 Model(Model&&) noexcept;
 ~Model();
 void upload(GLenum primitive, const std::vector<Vertex>& vertices,
             const std::vector<DrawRange>& parts={});
 void setFaceMode(int value) { faceMode=value; }
 void load_shaders(const char*, const char*);
 void render(const RenderOptions& options = {});
};
