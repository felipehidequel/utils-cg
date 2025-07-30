void drawBresenhamLinesIncomplete(int x0, int y0, int x1, int y1) {

  // calcula a variacao
  int dx = x1 - x0;
  int dy = y1 - y0;
  // calcula o erro
  int D = (2 * dy) - dx;

  int x = x0;
  int y = y0;

  putPixel(x, y, 255, 0, 0);

  while (x < x1) {
    x = x + 1;
    if (D < 0) {
      D = D + (2 * dy);
    } else {
      y = y + 1;
      D = D + (2 * (dy - dx));
    }
    putPixel(x, y, 255, 0, 0);
  }
}


void drawBresenhamLines( int x0, int y0, int x1, int y1 ){
  // calcula a variacao absoluta
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);

  // indica a direcao da reta
  int sx = (x0 < x1) ? 1 : -1; // direção de x
  int sy = (y0 < y1) ? 1 : -1; // direção de y

  // calcula o erro
  int erro = dx - dy;

  while (1) {
    putPixel(x0, y0, 250, 0, 0);

    // Quando a condicao for alcancada finaliza o loop
    if (x0 == x1 && y0 == y1) break;

    // atualiza o erro
    int erro2 = 2 * erro;

    if (erro2 > -dy) { // verifica se o erro esta acima
      erro -= dy;
      x0 += sx;
    }

    if (erro2 < dx) { // verifica se o erro esta abaixo
      erro += dx;
      y0 += sy;
    }
  }
}