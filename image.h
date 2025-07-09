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

#ifndef IMAGE_H
#define IMAGE_H

#define WIDTH 800
#define HEIGHT 800

//unsigned char image[HEIGHT][WIDTH][3];

void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);

void clr();

void save();

void draw_point( int x0, int y0, float r, float g, float b);

// adicionado função converteCores
void converteCores(int x0, int y0, float r, float g, float b);

void resizing( float v0x, float v0y, float v1x, float v1y );

#endif
