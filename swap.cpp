#include <iostream>
#include <cmath>

static const int MAX_REG = 31;
static const int MANT_MAX = 22;

union Convert {
    int a;
    float b;
};

auto float_sign(uint64_t a) -> double
{
    double sign;
    if ((a & 0x80000000) != 0){
        sign = -1.0;
    } else {
        sign = 1.0;
    }
    return sign;
}

auto exponent(uint64_t a) -> double
{
    uint64_t a_e = a & 0x7f800000;
    a_e = a_e >> 23;
    double val = ((double) a_e - 127.0)*log10(2.0);
    return val;
}

auto mantissa(uint64_t a) -> double
{
    double mant = 0.0;
    for(int i = 0; i < MAX_REG-8; ++i){
        if ((a & (1 << (MANT_MAX - i))) != 0){
            mant += pow(0.5,i+1);
        } else {
            continue;
        }
    }
    mant += 1.0;
    return mant;
}

auto f_point(uint64_t a) -> double
{
    double mant = mantissa(a);
    double expon = exponent(a);
    double sign = float_sign(a);

    double float_val = sign*mant*pow(10,expon);
    return float_val;
}

template<typename T>
auto get_bits(T x) -> uint32_t
{
    union {
        T t;
        uint32_t u;
    } type;
    type.t = x;
    return type.u;
}

template<typename T>
auto swap(T &x, T &y) -> void
{
    x ^= y;
    y ^= x;
    x ^= y;
}

auto main() -> int
{
    Convert cx, cy;
    cx.a = 42;
    cy.b = 6.022e+23;
    std::cout << "x = " << cx.a << std::endl;
    std::cout << "y = " << cy.b << std::endl;
    uint32_t ux = get_bits(cx.a);
    uint32_t uy = get_bits(cy.b);
    swap(ux,uy);
    double f = f_point(ux);
    cx.b = (float) f;
    cy.a = (int) uy;
    std::cout << "x = " << cx.b << std::endl;
    std::cout << "y = " << cy.a << std::endl;
}
