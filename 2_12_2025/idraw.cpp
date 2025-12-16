#include "idraw.h"

#include "geom.h"

namespace topit {
    p_t* extend(const p_t* pts, size_t s, p_t fill);
    void extend(p_t** pts, size_t& s, p_t fill);
}
topit::p_t* topit::extend(const p_t* pts, size_t s, p_t fill) {
    p_t* r = new p_t[s + 1];
    for (size_t i = 0; i < s; ++i) {
        r[i] = pts[i];
    }
    r[s] = fill;
    return r;
}

void topit::extend(p_t** pts, size_t& s, p_t fill) {
    p_t* r = extend(*pts, s, fill);
    delete [] *pts;
    ++s;
    *pts = r;
}
void topit::append(const IDraw* sh, p_t** ppts, size_t& s) {
    extend(ppts, s, sh->begin());
    p_t b = sh->begin();
    while (sh->next(b) != sh->begin()) {
        b = sh->next(b);
        extend(ppts, s, b);
    }
}
