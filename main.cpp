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

    struct Dot: IDraw {
        p_t begin () const override;
        p_t next (p_t prev) const override;
        p_t d;
    };
} 

int main() {
    using namespace top;
    p_t a{1, 1}, b{1, 1};
    std::cout << (a == b) << "\n";
}

top::p_t top::Dot::begin() const {
    return d;
}

top::p_t top::Dot::next(p_t prev) const {
    if (prev != d){
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
