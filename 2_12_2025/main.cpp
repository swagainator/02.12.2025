#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "geom.h"
#include "idraw.h"
#include "dot.h"
#include "rect.h"
#include "frect.h"
#include "canvas.h"

int main() {
  using namespace topit;
  int err = 0;
  IDraw* shp[3] = {};
  Layers layers;
  try {
    shp[2] = new Rect({-3, -2}, {4, 5});
    shp[0] = new FRect({3, 4}, {10, 11});
    shp[1] = new FRect({-10, -4}, {7, 7});
    for (size_t i = 0; i < 3; ++i) {
      layers.append(*(shp[i]));
    }
    f_t fr = layers.frame();
    char * cnv = canvas(fr, '.');
    const char * brush = "#*%";
    for (size_t k = 0; k < layers.layers(); ++k) {
      size_t start = layers.start(k);
      size_t end = layers.end(k);
      for (size_t i = start; i < end; ++i) {
        paint(layers.point(i), cnv, fr, brush[k]);
      }
    }
    flush(std::cout, cnv, fr);
    delete [] cnv;
  } catch (...) {
    std::cerr << "Error!\n";
    err = 1;
  }
  delete shp[2];
  delete shp[1];
  delete shp[0];
  return err;
}







