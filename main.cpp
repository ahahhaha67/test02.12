#include <iostream>
namespace top{
struct p_t {
    int x,y;
};

bool operator == (p_t a, p_t b);
bool operator != (p_t a, p_t b);

int main(){
    using namespace top;
    p_t a{1, 1}, b{1, 1};
    std::cout << (a==b) << "\n";
}

bool top::operator == (p_t a, p_t b){
    return a.x == b.x && a.y == b.y;
}
bool top::operator != (p_t a, p_t b){
    return !(a==b);
}

}