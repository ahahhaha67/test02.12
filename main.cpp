#include <iostream>
#include <stdexcept>

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

    struct HorizontalLine : IDraw {
        HorizontalLine(p_t start, p_t end);
        p_t begin() const override;
        p_t next(p_t prev) const override;
    private:
        p_t a;
        p_t b;
    };

    struct Rect : IDraw{
        Rect(p_t pos, int w, int h);
        Rect (p_t a, p_t b);
        p_t begin () const override;
        p_t next (p_t prev) const override;
        f_t rect;
    }

    struct Square : IDraw {
        Square(p_t lt, int sd) : lt{lt}, sd{sd} {
            if (sd <= 0) throw std::logic_error("Square side must be positive");
        }

        p_t begin() const override {
            return lt;
        }

        p_t next(p_t pr) const override {
            p_t nx = pr;

            p_t rt = { lt.x + sd - 1, lt.y };
            p_t rb = { lt.x + sd - 1, lt.y - sd + 1 };
            p_t lb = { lt.x, lt.y - sd + 1 };

            if (pr == lt && pr != rt) nx.x++;
            else if (pr.x < rt.x && pr.y == rt.y) nx.x++;
            else if (pr == rt && pr != rb) nx.y--;
            else if (pr.x == rb.x && pr.y > rb.y) nx.y--;
            else if (pr == rb && pr != lb) nx.x--;
            else if (pr.x > lb.x && pr.y == lb.y) nx.x--;
            else if (pr == lb && pr != lt) nx.y++;
            else if (pr.x == lt.x && pr.y < lt.y) nx.y++;
            else return lt;

            return nx;
        }

    private:
        p_t lt;
        int sd;
    };

    p_t* extend(const p_t* pts, size_t s, p_t fill);
    void extend(p_t** pts, size_t& s, p_t fill);
    void append(const IDraw* sh, p_t** ppts, size_t& s);
    f_t frame (const p_t * pts, size_t s);
    char * canvas(f_t fr, char fill);
    void paint(p_t p, char * cnv, f_t fr, char fill);
    void flush (std::ostream & os, const char* cnv, f_t fr);
}

int main() {
    using namespace top;
    int err = 0;
    IDraw* shp[4] = {};
    p_t* pts = nullptr;
    size_t s = 0;

    try {
        shp[0] = new HorizontalLine({-5, 2}, {2, 2});
        shp[1] = new Dot({2, 4});
        shp[2] = new Dot ({-5, -2});
        shp[3] = new Square({0, 0}, 5);

        for (size_t i = 0; i < 4; ++i){
            append(shp[i], &pts, s);
        }

        f_t fr = frame(pts, s);
        char* cnv = canvas(fr, '.');

        for (size_t i = 0; i < s; ++i){
            paint(pts[i], cnv, fr, '#');
        }

        flush(std::cout, cnv, fr);
        delete[] cnv;

    } catch (const std::exception& e) {
        std::cerr << "Error\n";
        err = 1;
    }

    for (int i = 3; i >= 0; --i) delete shp[i];
    delete[] pts;
    return err;
}

namespace top {

HorizontalLine::HorizontalLine(p_t start, p_t end) : a{start}, b{end} {
    if (start.y != end.y) throw std::logic_error("HorizontalLine must have same y");
}

p_t HorizontalLine::begin() const { return a; }

p_t HorizontalLine::next(p_t prev) const {
    if (prev == b) return a;
    p_t nx = prev;
    if (prev.x < b.x) nx.x = prev.x + 1;
    else if (prev.x > b.x) nx.x = prev.x - 1;
    nx.y = a.y;
    return nx;
}

void extend(p_t** pts, size_t& s, p_t fill){
    p_t* r = extend(*pts, s, fill);
    delete [] *pts;
    ++s;
    *pts = r;
}

p_t* extend(const p_t* pts, size_t s, p_t fill){
    p_t* r = new p_t[s + 1];
    for (size_t i = 0; i < s; ++i) r[i] = pts[i];
    r[s] = fill;
    return r;
}

void append(const IDraw* sh, p_t** ppts, size_t& s){
    extend(ppts, s, sh->begin());
    p_t b = sh->begin();
    while (sh->next(b) != sh->begin()) {
        b = sh->next(b);
        extend(ppts, s, b);
    }
}

void paint(p_t p, char* cnv, f_t fr, char fill){
    size_t dx = p.x - fr.aa.x;
    size_t dy = fr.bb.y - p.y;
    cnv[dy * cols(fr) + dx] = fill;
}

void flush(std::ostream& os, const char* cnv, f_t fr){
    for (size_t i = 0; i < rows(fr); ++i){
        for (size_t j = 0; j < cols(fr); ++j){
            os << cnv[i * cols(fr) + j];
        }
        os << "\n";
    }
}

char* canvas(f_t fr, char fill){
    size_t s = rows(fr) * cols(fr);
    char* c = new char[s];
    for (size_t i = 0; i < s; ++i) c[i] = fill;
    return c;
}

f_t frame(const p_t* pts, size_t s){
    int minx = pts[0].x, miny = pts[0].y;
    int maxx = minx, maxy = miny;
    for (size_t i = 1; i < s; ++i){
        minx = std::min(minx, pts[i].x);
        miny = std::min(miny, pts[i].y);
        maxx = std::max(maxx, pts[i].x);
        maxy = std::max(maxy, pts[i].y);
    }
    return f_t{ {minx, miny}, {maxx, maxy} };
}

top::Rect::Rect(p_t pos, int w, int h):
    IDraw(),
    rect{pos, {pos.x + w, pos.y + h}}
    {
        if (!(w > 0 && h > 0)){
            throw std::logic_error("bad rect");
        }
    }

top::Rect::Rect(p_t a, p_t b):
    Rect(a, b.x - a.x, b.y - a.y);
    {}


Dot::Dot(p_t dd) : d{dd} {}
p_t Dot::begin() const { return d; }

p_t Dot::next(p_t prev) const {
    if (prev != d) throw std::logic_error("bad prev");
    return d;
}

size_t rows(f_t fr){ return fr.bb.y - fr.aa.y + 1; }
size_t cols(f_t fr){ return fr.bb.x - fr.aa.x + 1; }

bool operator==(p_t a, p_t b){ return a.x == b.x && a.y == b.y; }
bool operator!=(p_t a, p_t b){ return !(a == b); }

} 