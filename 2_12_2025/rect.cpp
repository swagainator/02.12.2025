#include "rect.h"

#include <stdexcept>

#include "geom.h"

topit::Rect::Rect(p_t pos, int w, int h):
  IDraw(),
  rect{pos, {pos.x + w, pos.y + h}}
{
    if (!(w > 0 && h > 0)) {
        throw std::logic_error("bad rect");
    }
}

topit::Rect::Rect(p_t a, p_t b):
  Rect(a, b.x - a.x, b.y - a.y)
{}

topit::p_t topit::Rect::begin() const {
    return rect.aa;
}

topit::p_t topit::Rect::next(p_t prev) const {
    if (prev.x == rect.aa.x && prev.y < rect.bb.y) {
        return {prev.x, prev.y + 1};
    } else if (prev.y == rect.bb.y && prev.x < rect.bb.x) {
        return {prev.x + 1, prev.y};
    } else if (prev.x == rect.bb.x && prev.y > rect.aa.y) {
        return {prev.x, prev.y - 1};
    } else if (prev.y == rect.aa.y && prev.x > rect.aa.x) {
        return {prev.x - 1, prev.y};
    }
    throw std::logic_error("bad impl");
}