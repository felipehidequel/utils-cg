/**
 * \file model.c
 *
 * \brief Implementação das funções de manipulação do modelo 3D.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"
#include "model.h"

int load_obj(const char *filename, Vertex *vertices, int *vcount, Face *faces, int *fcount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char line[512];
    *vcount = 0;
    *fcount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            if (sscanf(line + 2, "%f %f %f", &vertices[*vcount].x, &vertices[*vcount].y, &vertices[*vcount].z) == 3) {
                (*vcount)++;
            }
        } else if (strncmp(line, "f ", 2) == 0) {
            Face face = {.n = 0};
            char *token = strtok(line + 2, " \n");
            while (token && face.n < MAX_FACE_VERTS) {
                int index;
                if (sscanf(token, "%d", &index) == 1) {
                    face.verts[face.n++] = index;
                }
                token = strtok(NULL, " \n");
            }
            faces[(*fcount)++] = face;
        }
    }

    fclose(file);
    return 1;
}

void render_faces(Vertex *vertices, Face *faces, int vcount, int fcount) {
    for (int i = 0; i < fcount; i++) {
        Face face = faces[i];
        for (int j = 0; j < face.n; j++) {
            Vertex v0 = vertices[face.verts[j] - 1];
            Vertex v1 = vertices[face.verts[(j + 1) % face.n] - 1];
            Vertex nv0 = perspective(v0);
            Vertex nv1 = perspective(v1);
            resizing(nv0.x, nv0.y, nv1.x, nv1.y);
        }
    }
}

Vertex perspective(Vertex v) {
    float p = 2.0;
    Vertex vaux = {0, 0, 0};
    if (v.z > 0) {
        vaux.x = (v.x * p) / v.z;
        vaux.y = (v.y * p) / v.z;
        vaux.z = v.z;
    }
    return vaux;
}

static Vertex vecSub(Vertex a, Vertex b) {
    return (Vertex){a.x - b.x, a.y - b.y, a.z - b.z};
}

static Vertex vec_cross(Vertex a, Vertex b) {
    return (Vertex){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static float vec_dot(Vertex a, Vertex b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vertex vec_normalize(Vertex v) {
    float tamanho = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (tamanho == 0.0f) return (Vertex){0, 0, 0};
    return (Vertex){v.x / tamanho, v.y / tamanho, v.z / tamanho};
}

void apply_camera(Vertex *vertices, int vcount, Camera cam) {
    Vertex n = vec_normalize(vecSub(cam.target, cam.position));
    Vertex u = vec_normalize(vec_cross(cam.up, n));
    Vertex v = vec_cross(n, u);

    for (int i = 0; i < vcount; i++) {
        Vertex p = {
            vertices[i].x - cam.position.x,
            vertices[i].y - cam.position.y,
            vertices[i].z - cam.position.z
        };

        vertices[i].x = vec_dot(p, u);
        vertices[i].y = vec_dot(p, v);
        vertices[i].z = vec_dot(p, n);
    }
}

Vertex hermite(Vertex p1, Vertex p2, Vertex t1, Vertex t2, float t) {
    float h1 = 2 * t * t * t - 3 * t * t + 1;
    float h2 = -2 * t * t * t + 3 * t * t;
    float h3 = t * t * t - 2 * t + t;
    float h4 = t * t * t - t * t;

    Vertex result;
    result.x = h1 * p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x;
    result.y = h1 * p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y;
    result.z = h1 * p1.z + h2 * p2.z + h3 * t1.z + h4 * t2.z;
    return result;
}

Vertex bezier(Vertex p0, Vertex p1, Vertex p2, Vertex p3, float t){
    Vertex result;
    result.x = ((((1-t)*(1-t)*(1-t))*p0.x) + ((3*t)*((1-t)*(1-t))*p1.x) + (3*(t*t)*(1-t)*p2.x) + (t*t*t)*p3.x);
    result.y = ((((1-t)*(1-t)*(1-t))*p0.y) + ((3*t)*((1-t)*(1-t))*p1.y) + (3*(t*t)*(1-t)*p2.y) + (t*t*t)*p3.y);
    result.z = ((((1-t)*(1-t)*(1-t))*p0.z) + ((3*t)*((1-t)*(1-t))*p1.z) + (3*(t*t)*(1-t)*p2.z) + (t*t*t)*p3.z);
    return result;
}

Vertex bilinear(Vertex p0, Vertex p1, Vertex p2, Vertex p3, float u, float v){
    float ex = (1-v)*p0.x + (v*p3.x);
    float ey = (1-v)*p0.y + (v*p3.y);
    float fx = (1-v)*p1.x + (v*p2.x);
    float fy = (1-v)*p1.y + (v*p2.y);

    float px = (1-u)*ex + (u*fx);
    float py = (1-u)*ey + (u*fy);

    Vertex result;
    result.x = px;
    result.y = py;

    return result;
}

