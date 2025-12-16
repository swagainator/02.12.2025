//
// Created by platon on 16.12.2025.
//

#include "geom.h"
size_t topit::rows(f_t fr) {
    return static_cast<size_t>(fr.bb.y - fr.aa.y + 1);
}

size_t topit::cols(f_t fr) {
    return static_cast<size_t>(fr.bb.x - fr.aa.x + 1);
}

bool topit::operator==(p_t a, p_t b) {
    return a.x == b.x && a.y == b.y;
}

bool topit::operator!=(p_t a, p_t b) {
    return !(a == b);
}