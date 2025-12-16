//
// Created by platon on 16.12.2025.
//

#ifndef FRECT_H
#define FRECT_H
#include "geom.h"
#include "idraw.h"

namespace topit {
    struct FRect: IDraw {
        FRect(p_t pos, int w, int h);
        FRect(p_t a, p_t b);
        p_t begin() const override;
        p_t next(p_t prev) const override;
        f_t rect;
    };
    void append(const IDraw* sh, p_t** ppts, size_t& s);
}
#endif