#include <stdlib.h> // abs

void drawDDA( int x0, int y0, int x1, int y1 ){
  // calcula a variacao
  int dx = x1 - x0;
  int dy = y1 - y0;

  // calcula o número de passos de incremento
  int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

  // define o incremento em cada eixo
  float x_inc = (float) dx / steps;
  float y_inc = (float) dy / steps;

  float x = x0;
  float y = y0;

  for (int i = 0; i <= steps; i++){
    putPixel((int)x, (int)y, 255, 0, 0);
    x += x_inc;
    y += y_inc;
  }
}