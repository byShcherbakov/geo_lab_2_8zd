#include "Model.h"
#include "func.h"
Model::Model(GLFWwindow*) { glGenVertexArrays(1,&vao); glGenBuffers(1,&vbo); }
Model::Model(Model&& o) noexcept : vao(std::exchange(o.vao,0)), vbo(std::exchange(o.vbo,0)),
 program(std::exchange(o.program,0)), mode(o.mode), ranges(std::move(o.ranges)), faceMode(o.faceMode) {}
Model::~Model() { glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao); if(program) glDeleteProgram(program); }
void Model::upload(GLenum primitive,const std::vector<Vertex>& vertices,const std::vector<DrawRange>& parts) {
 mode=primitive; ranges=parts;
 if(ranges.empty()) ranges.push_back({0,static_cast<GLsizei>(vertices.size())});
 glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
 glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);
 glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,position)));
 glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,color)));
 glEnableVertexAttribArray(0); glEnableVertexAttribArray(1);
}
void Model::load_shaders(const char* vertexFile,const char* fragmentFile) {
 auto compile=[](GLenum type,const std::string& source) {
  GLuint id=glCreateShader(type);const char* text=source.c_str();glShaderSource(id,1,&text,nullptr);glCompileShader(id);
  GLint ok;glGetShaderiv(id,GL_COMPILE_STATUS,&ok);
  if(!ok) {char log[4096]{};glGetShaderInfoLog(id,sizeof(log),nullptr,log);glDeleteShader(id);throw std::runtime_error(log);}return id;
 };
 std::vector<GLuint> shaders;
 try {
  shaders.push_back(compile(GL_VERTEX_SHADER,LoadShader(vertexFile)));
  if(mode==GL_LINES || mode==GL_LINE_STRIP || mode==GL_LINE_LOOP)
   shaders.push_back(compile(GL_GEOMETRY_SHADER,LoadShader("line_geometry.txt")));
  shaders.push_back(compile(GL_FRAGMENT_SHADER,LoadShader(fragmentFile)));
  program=glCreateProgram();for(auto s:shaders)glAttachShader(program,s);
  glLinkProgram(program);GLint ok;glGetProgramiv(program,GL_LINK_STATUS,&ok);
  if(!ok) {char log[4096]{};glGetProgramInfoLog(program,sizeof(log),nullptr,log);throw std::runtime_error(log);}
 } catch(...) {for(auto s:shaders)glDeleteShader(s);throw;}
 for(auto s:shaders)glDeleteShader(s);
}
void Model::render(const RenderOptions& options) {
 glUseProgram(program);glBindVertexArray(vao);
 auto uniform=[&](const char* name,int value){glUniform1i(glGetUniformLocation(program,name),value);};
 uniform("smooth_shading",options.smooth); uniform("round_points",mode==GL_POINTS && options.antialias);
 glUniform1f(glGetUniformLocation(program,"point_size"),options.pointSize);
 glUniform1f(glGetUniformLocation(program,"line_width"),options.lineWidth);
 GLint viewport[4];glGetIntegerv(GL_VIEWPORT,viewport);
 glUniform2f(glGetUniformLocation(program,"viewport_size"),float(viewport[2]),float(viewport[3]));
 uniform("mirror_x",options.mirror);
 glFrontFace(GL_CCW);
 auto draw=[&](){for(auto r:ranges)glDrawArrays(mode,r.first,r.count);};
 glDisable(GL_CULL_FACE);glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 if(faceMode==1 || faceMode==2) {
  glEnable(GL_CULL_FACE);glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT_AND_BACK,faceMode==1?GL_POINT:GL_FILL);draw();
  glCullFace(GL_FRONT);glPolygonMode(GL_FRONT_AND_BACK,faceMode==1?GL_FILL:GL_LINE);draw();
 } else {if(faceMode==3)glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);draw();}
 glDisable(GL_CULL_FACE);glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);glFrontFace(GL_CCW);
}
