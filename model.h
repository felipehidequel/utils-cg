/**
 * \file model.h
 *
 * \brief Header com protótipos de funções para manipulação de modelo 3D.
 *
 * \author
 * Petrucio Ricardo Tavares de Medeiros \n
 * Universidade Federal Rural do Semi-Árido \n
 * Departamento de Engenharias e Tecnologia \n
 * petrucio at ufersa (dot) edu (dot) br
 *
 * \version 1.0
 * \date May 2025
 */

#ifndef MODEL_H
#define MODEL_H

#define MAX_VERTICES 50000
#define MAX_FACES 50000
#define MAX_FACE_VERTS 32

typedef struct {
  float x, y, z;
} Vertex;

typedef struct {
  int verts[MAX_FACE_VERTS];
  int n;
} Face;

typedef struct {
  Vertex position;
  Vertex target;
  Vertex up;
} Camera;

int load_obj(const char *filename, Vertex *vertices, int *vcount, Face *faces, int *fcount);

void render_faces(Vertex *vertices, Face *faces, int vcount, int fcount);

void draw_line(int x0, int y0, int x1, int y1);

void apply_camera(Vertex *vertices, int vcount, Camera cam);

Vertex perspective(Vertex v);

Vertex hermite(Vertex p1, Vertex p2, Vertex t1, Vertex t2, float t);

Vertex bezier(Vertex p0, Vertex p1, Vertex p2, Vertex p3, float t);

Vertex bilinear(Vertex p0, Vertex p1, Vertex p2, Vertex p3, float u, float v);

#endif
