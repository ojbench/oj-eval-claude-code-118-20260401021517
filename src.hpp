#include <iostream>
#include <stdexcept>

struct divided_by_zero final : std::exception {
public:
    divided_by_zero()   = default;
    ~divided_by_zero()  = default;
    const char *what() const noexcept override { return "Divided by zero!"; };
};

// Helper function for GCD
template <typename _Tp>
inline _Tp gcd(_Tp x, _Tp y) {
    while (y) {
        x %= y;
        std::swap(x, y);
    }
    return x;
}

// Helper function for absolute value
template <typename _Tp>
inline _Tp abs(_Tp x) {
    return x < _Tp(0) ? -x : x;
}

template <typename _Tp>
struct fraction {
private:
    _Tp num;  // numerator
    _Tp den;  // denominator

    // Reduce fraction to simplest form
    void reduce() {
        if (den == _Tp(0)) {
            throw divided_by_zero();
        }

        // Handle sign: denominator should be positive
        if (den < _Tp(0)) {
            num = -num;
            den = -den;
        }

        // Reduce by GCD
        _Tp g = gcd(abs(num), abs(den));
        if (g != _Tp(0)) {
            num /= g;
            den /= g;
        }
    }

public:
    // Default constructor, constructs to 0/1
    fraction() : num(_Tp(0)), den(_Tp(1)) {}

    // Constructor from single value, constructs to x/1
    fraction(_Tp x) : num(x), den(_Tp(1)) {}

    // Constructor from two values, constructs to x/y
    fraction(_Tp x, _Tp y) : num(x), den(y) {
        reduce();
    }

    // Addition
    fraction operator + (const fraction &other) const {
        _Tp new_num = num * other.den + other.num * den;
        _Tp new_den = den * other.den;
        return fraction(new_num, new_den);
    }

    // Subtraction
    fraction operator - (const fraction &other) const {
        _Tp new_num = num * other.den - other.num * den;
        _Tp new_den = den * other.den;
        return fraction(new_num, new_den);
    }

    // Multiplication
    fraction operator * (const fraction &other) const {
        _Tp new_num = num * other.num;
        _Tp new_den = den * other.den;
        return fraction(new_num, new_den);
    }

    // Division
    fraction operator / (const fraction &other) const {
        if (other.num == _Tp(0)) {
            throw divided_by_zero();
        }
        _Tp new_num = num * other.den;
        _Tp new_den = den * other.num;
        return fraction(new_num, new_den);
    }

    // Power
    fraction operator ^ (long long exp) const {
        if (exp == 0) {
            return fraction(_Tp(1), _Tp(1));
        }

        if (exp < 0) {
            if (num == _Tp(0)) {
                throw divided_by_zero();
            }
            // For negative exponent, flip and negate exponent
            fraction inv(den, num);
            return inv ^ (-exp);
        }

        // Fast power for positive exponent
        fraction result(_Tp(1), _Tp(1));
        fraction base = *this;

        while (exp > 0) {
            if (exp & 1) {
                result *= base;
            }
            base *= base;
            exp >>= 1;
        }

        return result;
    }

    // Compound assignment operators
    fraction &operator += (const fraction &other) {
        *this = *this + other;
        return *this;
    }

    fraction &operator -= (const fraction &other) {
        *this = *this - other;
        return *this;
    }

    fraction &operator *= (const fraction &other) {
        *this = *this * other;
        return *this;
    }

    fraction &operator /= (const fraction &other) {
        *this = *this / other;
        return *this;
    }

    fraction &operator ^= (long long exp) {
        *this = *this ^ exp;
        return *this;
    }

    // Type conversion to double
    explicit operator double() const {
        return double(num) / double(den);
    }

    // Type conversion to bool
    explicit operator bool() const {
        return num != _Tp(0);
    }

    // Equality comparison
    friend bool operator == (const fraction &a, const fraction &b) {
        return a.num == b.num && a.den == b.den;
    }

    // Less than comparison
    friend bool operator < (const fraction &a, const fraction &b) {
        return a.num * b.den < b.num * a.den;
    }

    // Output operator
    friend std::ostream & operator << (std::ostream &os, const fraction &f) {
        os << f.num << "/" << f.den;
        return os;
    }
};
