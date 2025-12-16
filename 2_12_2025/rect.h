//
// Created by platon on 16.12.2025.
//

#ifndef RECT_H
#define RECT_H
#include "idraw.h"

namespace topit {
    struct Rect: IDraw {
        Rect(p_t pos, int w, int h);
        Rect(p_t a, p_t b);
        p_t begin() const override;
        p_t next(p_t prev) const override;
        f_t rect;
    };
}


#endif