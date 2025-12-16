#include "canvas.h"

#include <algorithm>
#include <iostream>
#include "frect.h"
#include "geom.h"

void topit::paint(p_t p, char* cnv, f_t fr, char fill) {
    size_t dx = static_cast<size_t>(p.x - fr.aa.x);
    size_t dy = static_cast<size_t>(fr.bb.y - p.y);
    cnv[dy * cols(fr) + dx] = fill;
}

void topit::flush(std::ostream& os, const char* cnv, f_t fr) {
    for (size_t i = 0; i < rows(fr); ++i) {
        for (size_t j = 0; j < cols(fr); ++j) {
            os << cnv[i * cols(fr) + j];
        }
        os << "\n";
    }
}

char * topit::canvas(f_t fr, char fill) {
    size_t s = rows(fr) * cols(fr);
    char * c = new char[s];
    for (size_t i = 0; i < s; ++i) {
        c[i] = fill;
    }
    return c;
}

void topit::Layers::append(const IDraw& dr) {
  size_t* ext_sizes = new size_t[layers_ + 1];
  try {
    topit::append(&dr, &pts_, points_);
  } catch (...) {
    delete [] ext_sizes;
    throw;
  }
  for (size_t i = 0; i < layers_; ++i) {
    ext_sizes[i] = sizes_[i];
  }
  ext_sizes[layers_] = points_;
  delete [] sizes_;
  sizes_ = ext_sizes;
  ++layers_;
}

topit::Layers::Layers():
  points_{0},
  pts_{nullptr},
  layers_{0},
  sizes_{nullptr}
{}

topit::Layers::~Layers()
{
  delete [] pts_;
  delete [] sizes_;
}


topit::Layers::Layers(const Layers& other):
  points_{other.points_},
  pts_{nullptr},
  layers_{other.layers_},
  sizes_{nullptr}
{
  if (points_ != 0) {
    pts_ = new p_t[points_];
    try {
      for (size_t i = 0; i < points_; ++i) {
        pts_[i] = other.pts_[i];
      }
    } catch (...) {
      delete [] pts_;
      pts_ = nullptr;
      throw;
    }
  }

  if (layers_ != 0) {
    sizes_ = new size_t[layers_];
    try {
      for (size_t i = 0; i < layers_; ++i) {
        sizes_[i] = other.sizes_[i];
      }
    } catch (...) {
      delete [] pts_;
      delete [] sizes_;
      pts_ = nullptr;
      sizes_ = nullptr;
      throw;
    }
  }
}

topit::Layers& topit::Layers::operator=(const Layers& other)
{
  if (this == &other) {
    return *this;
  }

  Layers tmp(other);
  std::swap(points_, tmp.points_);
  std::swap(pts_, tmp.pts_);
  std::swap(layers_, tmp.layers_);
  std::swap(sizes_, tmp.sizes_);
  return *this;
}

topit::Layers::Layers(Layers&& other) noexcept:
  points_{other.points_},
  pts_{other.pts_},
  layers_{other.layers_},
  sizes_{other.sizes_}
{
  other.points_ = 0;
  other.pts_ = nullptr;
  other.layers_ = 0;
  other.sizes_ = nullptr;
}

topit::Layers& topit::Layers::operator=(Layers&& other) noexcept
{
  if (this == &other) {
    return *this;
  }

  delete [] pts_;
  delete [] sizes_;

  points_ = other.points_;
  pts_ = other.pts_;
  layers_ = other.layers_;
  sizes_ = other.sizes_;

  other.points_ = 0;
  other.pts_ = nullptr;
  other.layers_ = 0;
  other.sizes_ = nullptr;

  return *this;
}

topit::f_t topit::frame(const p_t* pts, size_t s) {
  int minx = pts[0].x, miny = pts[0].y;
  int maxx = minx, maxy = miny;
  for (size_t i = 1; i < s; ++i) {
    minx = std::min(minx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxx = std::max(maxx, pts[i].x);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t a{minx, miny};
  p_t b{maxx, maxy};
  return f_t{a, b};
}