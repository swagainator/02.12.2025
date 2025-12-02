#include <iostream>
namespace topit {
    struct p_t {
        int x, y;
    };
    struct f_t { 
        p_t aa, bb;
    };
    struct IDraw {
        virtual ~IDraw() = default;
        virtual p_t next() const = 0;
        virtual p_t next(p_t prev) const = 0;
    };
    bool operator==(p_t a, p_t b);
    bool operator!=(p_t a, p_t b);
}
int main() {
    using namespace topit;
    p_t a{ 1,1 }, b{ 0,1 };
    std::cout << (a == b) << "\n";
}

bool topit::operator==(p_t a, p_t b) {
    return a.x = b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b) {
    return !(a == b);
}
