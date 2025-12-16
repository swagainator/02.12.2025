//
// Created by platon on 16.12.2025.
//

#ifndef GEOM_H
#define GEOM_H
#include <cstddef>
namespace topit {
    struct Layers;

    struct p_t { int x, y; };
    struct f_t { p_t aa, bb; };
    size_t rows(f_t fr);
    size_t cols(f_t fr);
    bool operator==(p_t a, p_t b);
    bool operator!=(p_t a, p_t b);
    f_t frame(const p_t * pts, size_t s);
    f_t frame(const Layers& ls);


};


#endif