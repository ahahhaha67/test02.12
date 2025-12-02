#include <iostream>
#include <stdexcept> 

namespace top {
    struct p_t {
        int x, y;
    };

    struct f_t {
        p_t aa, bb;  
    };

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
} 

void append(const IDraw* sh, p_t** ppts, size_t& s);

int main() {
    using namespace top;
    int err=0;
    IDraw* shp[3] = {};
    p_t * pts = nullptr;
    size_t s = 0;
    
    try {
        shp[0] = new Dot({0, 0});
        shp[1] = new Dot({2, 4});
        for (size_t i = 0; i < 3; ++i){
            append (shp[i], &pts, &s);


        }
    } catch (...) {
        err = 1;
    }
    
    return 0;
}

top::Dot::Dot(p_t dd) : d{dd} {}

top::p_t top::Dot::begin() const {
    return d;
}

top::p_t top::Dot::next(p_t prev) const {
    if (prev != d) {
        throw std::logic_error("bad prev");
    }
    return d;
}

bool top::operator==(p_t a, p_t b) {
    return a.x == b.x && a.y == b.y;
}

bool top::operator!=(p_t a, p_t b) {
    return !(a == b);
}