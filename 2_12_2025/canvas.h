//
// Created by platon on 16.12.2025.
//

#ifndef CANVAS_H
#define CANVAS_H
#include <iosfwd>

#include "geom.h"
#include "idraw.h"

namespace topit {
    char * canvas(f_t fr, char fill);
    void paint(p_t p, char* cnv, f_t fr, char fill);
    void flush(std::ostream& os, const char* cnv, f_t fr);

    struct Layers;
    f_t frame(const Layers& ls);

    struct Layers {
        Layers();
        ~Layers();

        Layers(const Layers&);
        Layers& operator=(const Layers&);
        Layers(Layers&&) noexcept;
        Layers& operator=(Layers&&) noexcept;

        void append(const IDraw & dr);

        f_t frame() const {
            return topit::frame(pts_, points_);
        }
        size_t points() const {
            return points_;
        }
        size_t layers() const {
            return layers_;
        }
        size_t start(size_t i) const {
            return !i ? 0 : sizes_[i - 1];
        }
        size_t end(size_t i) const {
            return sizes_[i];
        }
        p_t point(size_t i) const {
            return pts_[i];
        }
    private:
        size_t points_;
        p_t * pts_;
        size_t layers_;
        size_t * sizes_;
    };
}


#endif