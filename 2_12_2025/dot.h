#ifndef DOT_H
#define DOT_H
#include "geom.h"
#include "idraw.h"
namespace topit {
    struct Dot: IDraw {
        explicit Dot(p_t dd);
        p_t begin() const override;
        p_t next(p_t prev) const override;
        p_t d;
    };
}


#endif