#include <iostream>
#include <stdexcept> 
#include <algorithm>

namespace topit {
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

    struct VerticalLine : IDraw {
        VerticalLine(p_t start, p_t end);
        p_t begin() const override;
        p_t next(p_t prev) const override;
    private:
        p_t start_;
        p_t end_;
        bool is_valid_;
    };

    struct Square : IDraw {
        Square(p_t left_bottom, int side);
        p_t begin() const override;
        p_t next(p_t prev) const override;
    private:
        p_t lb; 
        int side;
    };

    struct Rect: IDraw {
        Rect(p_t pos, int w, int h);
        Rect(p_t a, p_t b);
        p_t begin() const override;
        p_t next(p_t prev) const override;
        f_t rect;
    };
    struct FRect: IDraw{
        FRect(p_t pos, int w, int h);
        FRect(p_t a, p_t b);
        p_t begin() const override;
        p_t next(p_t prev) const override;
        f_t rect;
    };

    p_t* extend(const p_t* pts, size_t s, p_t fill);
    void extend(p_t** pts, size_t& s, p_t fill);
    void append(const IDraw* sh, p_t** ppts, size_t& s);
    f_t frame(const p_t * pts, size_t s);
    char * canvas(f_t fr, char fill);
    void paint(p_t p, char * cnv, f_t fr, char fill);
    void flush (std::ostream & os, const char* cnv, f_t fr);
} 

int main() {
    using namespace topit;
    int err=0;
    size_t shp_size = 2;
    IDraw* shp[shp_size] = {};
    p_t * pts = nullptr;
    size_t s = 0;
    
    try {
        shp[0] = new FRect({10, 5}, {20, 10});
        shp[1] = new Dot({2, 2});
        
        for (size_t i = 0; i < shp_size; ++i){
            append(shp[i], &pts, s);
        }
        f_t fr = frame(pts, s);
        char * cnv = canvas(fr, '.');
        for (size_t i = 0; i < s; ++i){
            paint(pts[i], cnv, fr, '#');
        }
        flush(std::cout, cnv, fr);
        delete[] cnv;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        err = 1;
    }

    for (size_t i = 0; i < shp_size; ++i) {
        delete shp[i];
    }
    
    return err;
}

void topit::extend(p_t** pts, size_t& s, p_t fill){
    p_t* r = extend(*pts, s, fill);
    delete[] *pts;
    ++s;
    *pts = r;
}

topit::p_t* topit::extend(const p_t* pts, size_t s, p_t fill){
    p_t* r = new p_t[s + 1];
    for (size_t i = 0; i < s; ++i){
        r[i] = pts[i];
    }
    r[s] = fill;
    return r;
}

void topit::append(const IDraw* sh, p_t** ppts, size_t& s){
    extend(ppts, s, sh->begin());
    p_t b = sh->begin();

    while(sh->next(b) != sh->begin()){
        b = sh->next(b);
        extend(ppts, s, b);
    }
}

void topit::paint(p_t p, char * cnv, f_t fr, char fill){
    size_t dx = p.x - fr.aa.x;
    size_t dy = fr.bb.y - p.y;
    cnv[dy * cols(fr) + dx] = fill;
}

void topit::flush(std::ostream& os, const char* cnv, f_t fr){
    for (size_t i = 0; i < rows(fr); ++i){
        for (size_t j = 0; j < cols(fr); ++j){
            os << cnv[i * cols(fr) + j];
        }
        os << "\n";
    }
}

char * topit::canvas(f_t fr, char fill){
    size_t s = rows(fr) * cols(fr);
    char * c = new char[rows(fr) * cols (fr)];
    for (size_t i = 0; i < s; ++i){
        c[i] = fill;
    }
    return c;
}

topit::f_t topit::frame(const p_t* pts, size_t s){
    int minx = pts[0].x, miny = pts[0].y;
    int maxx = minx, maxy = miny;
    for (size_t i = 1; i < s; ++i){
        minx = std::min(minx, pts[i].x);
        miny = std::min(miny, pts[i].y);
        maxx = std::max(maxx, pts[i].x);
        maxy = std::max(maxy, pts[i].y);
    }
    p_t a{minx, miny};
    p_t b{maxx, maxy};
    return f_t{a,b};
}


topit::Dot::Dot(p_t dd) : d{dd} {}

topit::p_t topit::Dot::begin() const {
    return d;
}

topit::p_t topit::Dot::next(p_t prev) const {
    if (prev != d) {
        throw std::logic_error("bad prev");
    }
    return d;
}

topit::VerticalLine::VerticalLine(p_t start, p_t end) 
    : start_(start), end_(end), is_valid_(start.x == end.x) {
}

topit::p_t topit::VerticalLine::begin() const {
    if (!is_valid_) {
        throw std::logic_error("Invalid vertical line: x coordinates differ");
    }
    
    if (start_.y <= end_.y) {
        return start_;
    } else {
        return end_;
    }
}

topit::p_t topit::VerticalLine::next(p_t prev) const {
    if (!is_valid_) {
        throw std::logic_error("Invalid vertical line");
    }
    p_t min_point = start_;
    p_t max_point = end_;
    if (start_.y > end_.y) {
        min_point = end_;
        max_point = start_;
    }
    
    if (prev == max_point) {
        return min_point;
    }
    
    p_t next_point = prev;
    next_point.y += 1;
    
    return next_point;
}

topit::Square::Square(p_t left_bottom, int side)
    : lb(left_bottom), side(side)
{
    if (side <= 0) {
        throw std::logic_error("Square side must be positive");
    }
}

topit::p_t topit::Square::begin() const {
    return lb;
}

topit::p_t topit::Square::next(p_t prev) const {

    p_t rb = { lb.x + side, lb.y };        
    p_t lt = { lb.x,         lb.y + side };  
    p_t rt = { lb.x + side, lb.y + side };  

    if (prev.y == lb.y && prev.x < rb.x) {
        return { prev.x + 1, prev.y };
    }

    if (prev.x == rb.x && prev.y < rt.y) {
        return { prev.x, prev.y + 1 };
    }

    if (prev.y == rt.y && prev.x > lt.x) {
        return { prev.x - 1, prev.y };
    }

    if (prev.x == lt.x && prev.y > lb.y) {
        return { prev.x, prev.y - 1 };
    }

    return lb;
}
topit::FRect::FRect(p_t pos, int w, int h) :
    IDraw(),
    rect{pos, {pos.x + w, pos.y + h}}
{
    if (!(w > 0 && h > 0)) {
        throw std::logic_error("bad request");
    }
}

topit::p_t topit::FRect::begin() const {
    return rect.aa;
}

topit::p_t topit::FRect::next(p_t prev) const {
    if (prev.x < rect.bb.x){
        return {prev.x + 1, prev.y};
    } else if (prev.x == rect.bb.x && prev.y < rect.bb.y){
        return {rect.aa.x, prev.y + 1};
    } else if (prev == rect.bb){
        return rect.aa;
    }
    throw std::logic_error("bad impl");
}

topit::FRect::FRect(p_t a, p_t b): FRect(a, b.x - a.x, b.y - a.y) {}

topit::Rect::Rect(p_t pos, int w, int h) :
    IDraw(),
    rect{pos, {pos.x + w, pos.y + h}}
{
    if (!(w > 0 && h > 0)) {
        throw std::logic_error("bad request");
    }
}

topit::Rect::Rect(p_t a, p_t b): Rect(a, b.x - a.x, b.y - a.y) {}

topit::p_t topit::Rect::begin() const {
    return rect.aa;
}

topit::p_t topit::Rect::next(p_t prev) const {
    if (prev.x == rect.aa.x && prev.y < rect.bb.y){
        return {prev.x, prev.y + 1};
    } else if (prev.y == rect.bb.y && prev.x < rect.bb.x){
        return  {prev.x + 1, prev.y};
    } else if (prev.x == rect.bb.x && prev.y > rect.aa.y){
        return {prev.x, prev.y - 1};
    } else if (prev.y == rect.aa.y && prev.x > rect.aa.x){
        return {prev.x - 1, prev.y};
    }
    throw std::logic_error("bad impl");
}






size_t topit::rows (f_t fr){
    return (fr.bb.y - fr.aa.y + 1);
}

size_t topit::cols (f_t fr){
    return (fr.bb.x - fr.aa.x + 1);
}

bool topit::operator==(p_t a, p_t b) {
    return a.x == b.x && a.y == b.y;
}

bool topit::operator!=(p_t a, p_t b) {
    return !(a == b);
}
