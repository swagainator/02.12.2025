#include <iostream>
#include <stdexcept> 
#include <algorithm>

namespace top {
    struct p_t {
        int x, y;
    };

    struct f_t {
        p_t aa, bb;
    };

    size_t rows(f_t fr);
    size_t cols(f_t fr);

    bool operator==(p_t a, p_t b);
    bool operator!=(p_t a, p_t b);

    struct IDraw {
        virtual ~IDraw() = default;
        virtual p_t begin() const = 0;
        virtual p_t next(p_t prev) const = 0;
    };

    struct Dot : IDraw {
        explicit Dot(p_t dd);
        p_t begin() const override;
        p_t next(p_t prev) const override;

    private:
        p_t d;
    };

    struct HSeg : IDraw {
        explicit HSeg(p_t start, int length);
        p_t begin() const override;
        p_t next(p_t prev) const override;

    private:
        p_t startp;
        int seglen;
    };

    struct Square : IDraw {
        explicit Square(p_t top_left, int size);
        p_t begin() const override;
        p_t next(p_t prev) const override;

    private:
        p_t topleft;
        int size;
        mutable bool instart;
        mutable int nowid;
    };

    p_t* extend(const p_t* pts, size_t s, p_t fill);
    void extend(p_t** pts, size_t& s, p_t fill);
    void append(const IDraw* sh, p_t** ppts, size_t& s);
    f_t frame(const p_t* pts, size_t s);
    char* canvas(f_t fr, char fill);
    void paint(p_t p, char* cnv, f_t fr, char fill);
    void flush(std::ostream& os, const char* cnv, f_t fr);
}

int main() {
    using namespace top;
    int err = 0;
    IDraw* shp[5] = {};
    p_t* pts = nullptr;
    size_t s = 0;

    try {
        shp[0] = new Dot({ -10, -10 });
        shp[1] = new Dot({ 10, -10 });
        shp[2] = new Dot({ -10, 10 });
        shp[3] = new HSeg({ -8, 0 }, 16);
        shp[4] = new Square({ 2, 2 }, 5);

        for (size_t i = 0; i < 5; ++i) {
            append(shp[i], &pts, s);
        }
        f_t fr = frame(pts, s);
        char* cnv = canvas(fr, '.');
        for (size_t i = 0; i < s; ++i) {
            paint(pts[i], cnv, fr, '#');
        }
        flush(std::cout, cnv, fr);
        delete[] cnv;

    }
    catch (...) {
        std::cerr << "Error\n";
        err = 1;
    }

    for (int i = 4; i >= 0; --i) {
        delete shp[i];
    }
    delete[] pts;

    return err;
}

void top::extend(p_t** pts, size_t& s, p_t fill) {
    p_t* r = extend(*pts, s, fill);
    delete[] * pts;
    ++s;
    *pts = r;
}

top::p_t* top::extend(const p_t* pts, size_t s, p_t fill) {
    p_t* r = new p_t[s + 1];
    for (size_t i = 0; i < s; ++i) {
        r[i] = pts[i];
    }
    r[s] = fill;
    return r;
}

void top::append(const IDraw* sh, p_t** ppts, size_t& s) {
    p_t first = sh->begin();
    extend(ppts, s, first);
    
    p_t current = first;
    p_t next_pt = sh->next(current);
    
    while (next_pt != first) {
        extend(ppts, s, next_pt);
        current = next_pt;
        next_pt = sh->next(current);
    }
}

void top::paint(p_t p, char* cnv, f_t fr, char fill) {
    int dx = p.x - fr.aa.x;
    int dy = fr.bb.y - p.y;
    cnv[dy * cols(fr) + dx] = fill;
}

void top::flush(std::ostream& os, const char* cnv, f_t fr) {
    for (size_t i = 0; i < rows(fr); ++i) {
        for (size_t j = 0; j < cols(fr); ++j) {
            os << cnv[i * cols(fr) + j];
        }
        os << "\n";
    }
}

char* top::canvas(f_t fr, char fill) {
    size_t s = rows(fr) * cols(fr);
    char* c = new char[s];
    for (size_t i = 0; i < s; ++i) {
        c[i] = fill;
    }
    return c;
}

top::f_t top::frame(const p_t* pts, size_t s) {
    int minx = pts[0].x, miny = pts[0].y;
    int maxx = minx, maxy = miny;
    for (size_t i = 1; i < s; ++i) {
        if (pts[i].x < minx) minx = pts[i].x;
        if (pts[i].y < miny) miny = pts[i].y;
        if (pts[i].x > maxx) maxx = pts[i].x;
        if (pts[i].y > maxy) maxy = pts[i].y;
    }
    p_t a{ minx, miny };
    p_t b{ maxx, maxy };
    return f_t{ a,b };
}

top::Dot::Dot(p_t dd) : d{ dd } {}

top::p_t top::Dot::begin() const {
    return d;
}

top::p_t top::Dot::next(p_t prev) const {
    if (prev != d) {
        throw std::logic_error("bad prev");
    }
    return d;
}

size_t top::rows(f_t fr) {
    return (fr.bb.y - fr.aa.y + 1);
}

size_t top::cols(f_t fr) {
    return (fr.bb.x - fr.aa.x + 1);
}

bool top::operator==(p_t a, p_t b) {
    return a.x == b.x && a.y == b.y;
}

bool top::operator!=(p_t a, p_t b) {
    return !(a == b);
}

top::HSeg::HSeg(p_t start, int length)
    : startp{ start }, seglen{ length } {
    if (length <= 0) {
        throw std::invalid_argument("len < 0");
    }
}

top::p_t top::HSeg::begin() const {
    return startp;
}

top::p_t top::HSeg::next(p_t prev) const {
    if (prev == startp && seglen > 1) {
        return { startp.x + 1, startp.y };
    }
    if (prev.x < startp.x + seglen - 1 && prev.x >= startp.x) {
        return { prev.x + 1, prev.y };
    }
    return startp;
}

top::Square::Square(p_t topleft, int size)
    : topleft{topleft}, size{size}, instart{false}, nowid{0} {
    if (size <= 0) {
        throw std::invalid_argument("size <= 0");
    }
}

top::p_t top::Square::begin() const {
    instart = false;
    nowid = 0;
    return topleft;
}

top::p_t top::Square::next(p_t prev) const {
    if (instart) {
        return topleft;
    }
    
    if (size == 1) {
        instart = true;
        return topleft;
    }
    int points = 4 * (size - 1);
    
    if (nowid >= points - 1) {
        instart = true;
        return topleft;
    }
    
    nowid++;

    if (nowid < size) {
        return {topleft.x + nowid, topleft.y};
    }
    else if (nowid < size + (size - 1)) {
        int idx = nowid - size;
        return {topleft.x + size - 1, topleft.y + idx + 1};
    }
    else if (nowid < size + 2 * (size - 1)) {
        int idx = nowid - (size + size - 1);
        return {topleft.x + size - 2 - idx, topleft.y + size - 1};
    }
    else {
        int idx = nowid - (size + 2 * (size - 1));
        return {topleft.x, topleft.y + size - 2 - idx};
    }
}
