#pragma once
#include "libs.h"
struct UiVertex { float x,y,u,v,r,g,b,a; };
class Ui {
 GLuint vao=0,vbo=0,texture=0,program=0;
 std::vector<UiVertex> vertices;
 void quad(float x,float y,float w,float h,float u0,float v0,float u1,float v1,const glm::vec4& color);
public:
 Ui();
 ~Ui();
 Ui(const Ui&)=delete;
 Ui& operator=(const Ui&)=delete;
 void begin();
 void rect(float x,float y,float w,float h,const glm::vec4& color);
 void text(float x,float y,const std::string& value,float size=16.0f,const glm::vec4& color={0.10f,0.12f,0.15f,1});
 bool button(float x,float y,float w,float h,const std::string& value,bool active,double mouseX,double mouseY,bool click);
 void render(int logicalWidth,int logicalHeight,int framebufferWidth,int framebufferHeight);
};
