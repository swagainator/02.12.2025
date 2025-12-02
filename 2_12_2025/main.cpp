#include <iostream>
#include <stdexcept>
namespace topit {
    struct p_t {
        int x, y;
    };
    struct f_t { 
        p_t aa, bb;
    };
    struct IDraw {
        virtual ~IDraw() = default;
        virtual p_t begin() const = 0;
        virtual p_t next(p_t prev) const = 0;
    };
    bool operator==(p_t a, p_t b);
    bool operator!=(p_t a, p_t b);
    struct Dot : IDraw {
        explicit Dot(p_t dd);
        p_t begin() const override;
        p_t next(p_t prev) const override;
        p_t d;
    };
    void append(const IDraw* sh, p_t** ppts, size_t& s);
    f_t frame(const p_t* pts, size_t s);
    char* canvas(f_t fr, char fill);
    void paint(p_t p, char* cnv, f_t fr, char fill);
    void flush(std::ostream& os, const char* cnv, f_t fr);
}
int main() {
    using namespace topit;
    int err = 0;
    IDraw* shp[3] = {};
    p_t* pts = nullptr;
    size_t s = 0;
    try {
        shp[0] = new Dot({ 0, 0 });
        shp[1] = new Dot({ 2,3 });
        shp[2] = new Dot({ -5,-2 });
        for (size_t i = 0; i < 3; ++i) {
            append(shp[i], &pts, s);
        }
        f_t fr = frame(pts, s);
        char* cnv = canvas(fr, '.');
        for (size_t i = 0; i < s; ++i) {
            paint(pts[i], cnv, fr, '#');
        }
        flush(std::cout, cnv, fr);
        delete[] cnv;
    } catch (...) {
        std::cerr << "ERR\n";
        err = 1;
    }
    
    delete shp[0];
    delete shp[1];
    delete shp[2];
    return err;
}
topit::f_t topit::frame(const p_t* pts, size_t s) {
    int minx = pts[0].x;
    int miny = pts[0].y;
    int maxx = pts[0].x;
    int maxy = pts[0].y;
    for (size_t i = 1; i < s; ++i) {
        minx = std::min(minx, pts[i].x);
        maxx = std::max(maxx, pts[i].x);
        miny = std::min(miny, pts[i].y);
        maxy = std::max(maxy, pts[i].y);
    }
    p_t a{ minx, miny };
    p_t b{ maxx, maxy };
    return f_t{ a, b };
}
topit::Dot::Dot(p_t dd) :
 IDraw(), 
 d{dd} 
{}
topit::p_t topit::Dot::begin() const {
    return d;
}
topit::p_t topit::Dot::next(p_t prev) const {
    if (prev != d) {
        throw std::logic_error("bad prev");
    }
    return d;
}
bool topit::operator==(p_t a, p_t b) {
    return a.x = b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b) {
    return !(a == b);
}
