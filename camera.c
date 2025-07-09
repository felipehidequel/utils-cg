/**
 * \file camera.c
 *
 * \brief Implementação da função para manipulação de camera.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Árido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date July 2025
 */

#include "image.h"
#include "camera.h"
#include <math.h>
#include <stdio.h>

Vertex sub(Vertex a, Vertex b) {
  /* Implemente essa funcao */
  return (Vertex) {a.x-b.x, a.y-b.y, a.z - b.z};
}

Vertex cross(Vertex a, Vertex b) {
  /* Implemente essa funcao */
  return (Vertex){
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
};
}

float dot(Vertex a, Vertex b) {
  /* Implemente essa funcao */
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vertex normalize(Vertex v) {
  /* Implemente essa funcao */
  float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
  if (len == 0.0f) return (Vertex){0, 0, 0};
  return (Vertex){v.x / len, v.y / len, v.z / len};

}

Vertex apply_camera( Vertex p, Camera cam ){
  Vertex n = normalize(sub(cam.target, cam.position)); /* Modifique esse vetor */
  Vertex u = normalize(cross(cam.up, n)); /* Modifique esse vetor */
  Vertex v = cross(n,u); /* Modifique esse vetor */
  
  Vertex aux = sub(  p, cam.position );
  return (Vertex) { dot(aux, u),
		    dot(aux, v),
		    dot(aux, n)};
}

Vertex interpolacao(Vertex a, Vertex b, double t) {
  /* Implemente essa funcao */
  float x = (1-t)*a.x + t*b.x;
  float y = (1-t)*a.y + t*b.y;
  float z = (1-t)*a.z + t*b.z;
  return (Vertex) {x, y, z};
}

Vertex bilinear_surface(Vertex A, Vertex B, Vertex C, Vertex D, double u, double v) {
  Vertex ad = interpolacao(A, D, u);
  Vertex bc = interpolacao(B, C, u);
  return interpolacao(ad, bc, v);
}

Vertex perspective_project(Vertex v, float d) {
  Vertex vaux = {0.0, 0.0, 0.0};
  
  if ( v.z + d > 0 ){
    vaux.x = (v.x * d)/(v.z + d);
    vaux.y = (v.y * d)/(v.z + d);
    vaux.z = v.z;
  }
  
  return (Vertex){(int)((vaux.x + 1.0) * WIDTH / 2.0), (int)((1.0 - vaux.y) * HEIGHT / 2.0), 0};
}


void render_surface(Vertex A, Vertex B, Vertex C, Vertex D, Camera cam, int maxvalue){
  for (int iu = 0; iu < maxvalue; iu++) {
    for (int iv = 0; iv < maxvalue; iv++) {
      double u = (double)iu / (maxvalue - 1);
      double v = (double)iv / (maxvalue - 1);
      Vertex p = bilinear_surface(A, B, C, D, u, v);
      Vertex pc = apply_camera(p, cam);
      Vertex point = perspective_project(pc, 2.0);
      // Coloracao usando z                                                                                                          
      int g = (int)(128 + 127 * p.z / 2.0); // mapeia z de [-2,2] para [1,255]                                                                                   
      if (g < 0) g = 0; if (g > 255) g = 255;
      px((int)point.x, (int)point.y, g, 0, 0);
    }
  }
}

void render_video_surface(Vertex A, Vertex B, Vertex C, Vertex D, Camera cam, int maxvalue, int frames){
  for (int frame = 0; frame < frames; frame++) {
    clr();
    
    double angle = 2 * M_PI * frame / 20.0;
    cam.position = (Vertex){6 * cos(angle), 3, 6 * sin(angle)};
    
    render_surface(A, B, C, D, cam, 100);
    
    char filename[64];
    sprintf(filename, "frame_%03d.ppm", frame);
    write_ppm(filename);
    printf("Frame %d gerado: %s\n", frame, filename);
  }
}
