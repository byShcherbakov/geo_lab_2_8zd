#include "Experiment.h"
#include <iomanip>
#include <set>

// Run on the CURRENT GPU. The result is a measurement, not a hard-coded constant.
// Core profile uses shader antialiasing; legacy GL_POINT_SMOOTH is not involved.
PointExperiment measurePoints() {
 PointExperiment result;
 GLint oldFbo=0,oldViewport[4],textureLimit=0,oldPack=0;
 glGetIntegerv(GL_FRAMEBUFFER_BINDING,&oldFbo);
 glGetIntegerv(GL_VIEWPORT,oldViewport);
 glGetIntegerv(GL_MAX_TEXTURE_SIZE,&textureLimit);
 glGetIntegerv(GL_PACK_ALIGNMENT,&oldPack);
 const bool hadMultisample=glIsEnabled(GL_MULTISAMPLE)==GL_TRUE;
 GLuint fbo=0,texture=0;
 auto restore=[&](){
  glBindFramebuffer(GL_FRAMEBUFFER,static_cast<GLuint>(oldFbo));
  glViewport(oldViewport[0],oldViewport[1],oldViewport[2],oldViewport[3]);
  glPixelStorei(GL_PACK_ALIGNMENT,oldPack);
  if(hadMultisample)glEnable(GL_MULTISAMPLE);
  glDeleteFramebuffers(1,&fbo);glDeleteTextures(1,&texture);
 };
 try {
  const int limit=std::min({static_cast<int>(std::floor(maxPointSize)),textureLimit-16,2048});
  if(limit<4)throw std::runtime_error("Point-size range is too small for this experiment");
  // Limit memory use; a truncated experiment is explicitly reported as a lower bound.
  const int side=limit+16;
  glGenFramebuffers(1,&fbo);glBindFramebuffer(GL_FRAMEBUFFER,fbo);
  glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,side,side,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture,0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)throw std::runtime_error("Experiment framebuffer is incomplete");
  glDrawBuffer(GL_COLOR_ATTACHMENT0);glReadBuffer(GL_COLOR_ATTACHMENT0);
  glViewport(0,0,side,side);glDisable(GL_MULTISAMPLE);glPixelStorei(GL_PACK_ALIGNMENT,1);
  Model point(nullptr);point.upload(GL_POINTS,{{{0,0,0},{0,0,0}}});
  point.load_shaders("vertex_shader.txt","fragment_shader.txt");
  std::ostringstream report;
  report << "Task 1. Variant 2. Point-size experiment\n"
         << "Method: GL_POINTS + fragment-shader AA (smoothstep/fwidth), RGBA8, no MSAA.\n"
         << "Maximum is for this implementation, not legacy GL_POINT_SMOOTH.\n"
         << "GPU: " << glGetString(GL_RENDERER) << "\nVendor: " << glGetString(GL_VENDOR)
         << "\nOpenGL: " << glGetString(GL_VERSION) << "\nDriver maximum: " << maxPointSize
         << " framebuffer pixels.\nTest limit: " << limit << " px.\n"
         << "Pass: measured center width differs by <=2 px, and partially covered edge pixels exist.\n"
         << "requested,AA,measured_width,gray_edge_pixels,GL_error,pass\n";
  std::vector<unsigned char> row(static_cast<std::size_t>(side)*4);
  auto test=[&](float size,bool aa) {
   while(glGetError()!=GL_NO_ERROR){}
   glClearColor(1,1,1,1);glClear(GL_COLOR_BUFFER_BIT);
   RenderOptions options;options.pointSize=size;options.antialias=aa;point.render(options);
   int left=side,right=-1,gray=0;
   glReadPixels(0,side/2,side,1,GL_RGBA,GL_UNSIGNED_BYTE,row.data());
   for(int x=0;x<side;++x){int red=row[4*x];if(red<250){left=std::min(left,x);right=x;}if(red>5&&red<250)++gray;}
   const int width=right>=left?right-left+1:0;
   glReadPixels(0,side/2+static_cast<int>(std::min(size,float(limit))/4),side,1,GL_RGBA,GL_UNSIGNED_BYTE,row.data());
   for(int x=0;x<side;++x)if(row[4*x]>5&&row[4*x]<250)++gray;
   GLenum error=glGetError();
   bool pass=aa && error==GL_NO_ERROR && std::abs(width-size)<=2.0f && gray>0;
   report << size << ',' << (aa?"on":"off") << ',' << width << ',' << gray << ',' << error << ',' << (pass?"yes":"no") << '\n';
   if(pass)result.maximum=std::max(result.maximum,size);
   return pass;
  };
  std::set<int> sizes={4,8,16,32,64,128,256,512,1024,limit};
  for(int size:sizes)if(size<=limit){test(float(size),false);test(float(size),true);}
  // If the upper bound fails, descend to the largest supported integer size.
  if(result.maximum<float(limit))for(int size=limit-1;size>=4;--size)if(test(float(size),true))break;
  bool upperMeasured=false;
  if(maxPointSize<=limit+1 && maxPointSize>limit)upperMeasured=test(maxPointSize,true);
  result.reachedDriverLimit=(result.maximum>=maxPointSize-0.01f) || upperMeasured;
  if(result.reachedDriverLimit && maxPointSize+4<side-2)test(maxPointSize+4,true); // saturation control
  result.completed=true;
  report << "\nLargest verified size: " << result.maximum << " px.\n"
         << (result.reachedDriverLimit?"The measured size reaches the driver upper bound.\n":"Upper bound was not fully verified: treat the result as the largest tested passing size.\n");
  std::ofstream file("points_experiment.txt");
  if(file){file<<report.str();file.close();result.saved=bool(file);}
  std::cout<<report.str();
  result.message=result.maximum>0?"Измерено: "+std::to_string(int(result.maximum))+" px":"Сглаживание не подтверждено";
 } catch(const std::exception& e) {result.message=std::string("Ошибка эксперимента: ")+e.what();}
 restore();return result;
}
