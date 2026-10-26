#include "tasks.h"
#include <array>
#include <random>
namespace {
constexpr int N=7;
constexpr float PI=3.14159265358979323846f;
const std::vector<glm::vec3> first={{-0.80000000f, 0.34491018f, 0.0f},
 {-0.57005988f, -0.34491018f, 0.0f},
 {-0.34970060f, 0.00000000f, 0.0f},
 {0.11976048f, 0.00000000f, 0.0f},
 {-0.00479042f, 0.45988024f, 0.0f},
 {0.80000000f, 0.45988024f, 0.0f},
 {0.11976048f, -0.45988024f, 0.0f}};
const std::vector<glm::vec3> second={{-0.80000000f, 0.62337662f, 0.0f},
 {0.80000000f, 0.62337662f, 0.0f},
 {0.17662338f, 0.24935065f, 0.0f},
 {0.17662338f, -0.24935065f, 0.0f},
 {0.66493506f, -0.24935065f, 0.0f},
 {0.66493506f, -0.62337662f, 0.0f},
 {-0.57142857f, -0.62337662f, 0.0f},
 {-0.07272727f, 0.00000000f, 0.0f}};
const std::vector<glm::vec3> third={{-0.80000000f, -0.16000000f, 0.0f},
 {-0.26666667f, 0.58666667f, 0.0f},
 {0.69333333f, 0.48000000f, 0.0f},
 {0.80000000f, -0.16000000f, 0.0f},
 {0.26666667f, -0.58666667f, 0.0f},
 {-0.05333333f, 0.26666667f, 0.0f},
 {-0.48000000f, -0.26666667f, 0.0f},
 {0.05333333f, -0.37333333f, 0.0f},
 {-0.26666667f, -0.58666667f, 0.0f}};
const std::vector<std::array<int,3>> triSecond={{0,1,2},{7,0,2},{7,2,3},{3,4,5},{3,5,6},{3,6,7}};
const std::vector<std::array<int,3>> triThird={{1,2,3},{3,4,5},{1,3,5},{0,1,5},{0,5,6},{8,0,6},{6,7,8}};
std::vector<glm::vec3> regular() {
 std::vector<glm::vec3> points;
 for(int i=0;i<N;++i) {float a=PI/2+2*PI*i/N;points.push_back({0.72f*std::cos(a),0.72f*std::sin(a),0});}
 return points;
}
glm::vec3 randomColor(std::mt19937& rng) {
 std::uniform_real_distribution<float> dist(0.15f,0.9f);return {dist(rng),dist(rng),dist(rng)};
}
Model outline(GLFWwindow* w,GLenum mode,const std::vector<glm::vec3>& points) {
 Model m(w);std::vector<Vertex> vertices;
 for(auto v:points)vertices.push_back({v,{0,0,0}});
 m.upload(mode,vertices);return m;
}
Model triangles(GLFWwindow* w,const std::vector<glm::vec3>& points,const std::vector<std::array<int,3>>& indices) {
 Model m(w);std::vector<Vertex> vertices;std::mt19937 rng(colorSeed);
 for(auto t:indices) {
  // Normalize winding to CCW. Flat shading uses the last vertex color.
  auto a=points[t[0]],b=points[t[1]],c=points[t[2]];
  if((b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x)<0)std::swap(t[1],t[2]);
  for(auto i:t)vertices.push_back({points[i],randomColor(rng)});
 }
 m.upload(GL_TRIANGLES,vertices);return m;
}
}
Model task1(GLFWwindow* w) {return outline(w,GL_POINTS,regular());}
Model task2(GLFWwindow* w) {
 auto points=regular();std::vector<glm::vec3> lines;
 for(int i=0;i<N;++i){lines.push_back(points[i]);lines.push_back(points[(i+1)%N]);}
 return outline(w,GL_LINES,lines);
}
Model task3(GLFWwindow* w) {return outline(w,GL_LINE_STRIP,first);}
Model task4(GLFWwindow* w) {return outline(w,GL_LINE_LOOP,second);}
Model task5_1(GLFWwindow* w) {return triangles(w,second,triSecond);}
Model task5_2(GLFWwindow* w) {
 Model m(w);std::vector<Vertex> vertices;std::mt19937 rng(colorSeed+1);
 // One strip, eight vertices, six non-overlapping triangles.
 for(int i:{1,0,2,7,3,6,4,5})vertices.push_back({second[i],randomColor(rng)});
 m.upload(GL_TRIANGLE_STRIP,vertices);return m;
}
Model task5_3_1(GLFWwindow* w) {
 Model m(w);std::vector<Vertex> vertices;std::mt19937 rng(colorSeed+2);
 // A concave contour is split into two valid fans, three triangles each.
 for(int i:{2,1,0,7,3, 3,4,5,6,7})vertices.push_back({second[i],randomColor(rng)});
 m.upload(GL_TRIANGLE_FAN,vertices,{{0,5},{5,5}});return m;
}
Model task6(GLFWwindow* w) {
 Model m(w);std::vector<Vertex> vertices;std::mt19937 rng(colorSeed+3);auto points=regular();
 vertices.push_back({{0,0,0},randomColor(rng)});
 for(int i=0;i<=N;++i)vertices.push_back({points[i%N],randomColor(rng)});
 m.upload(GL_TRIANGLE_FAN,vertices);return m;
}
Model task7(GLFWwindow* w) {return triangles(w,third,triThird);}
Model task8a(GLFWwindow* w) {auto m=task7(w);m.setFaceMode(1);return m;}
Model task8b(GLFWwindow* w) {auto m=task7(w);m.setFaceMode(2);return m;}
Model task8c(GLFWwindow* w) {auto m=task7(w);m.setFaceMode(3);return m;}
