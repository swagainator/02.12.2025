#ifndef IDRAW_H
#define IDRAW_H
#include "geom.h"


namespace topit {
    struct IDraw {
        virtual ~IDraw() = default;
        virtual p_t begin() const = 0;
        virtual p_t next(p_t prev) const = 0;
    };
}


#endif