#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <iostream>
#include <array>
#include <string_view>
#include <stdexcept>
#include <cassert>
#include <cstdint>


/*  Typedefs for better semantics
    + type can easily be changed  */

using val_t = std::uint_fast8_t;
using deg_t = std::uint_fast8_t;



/*  𝔽₄ is the finite field 𝔽₂[𝑥]/(𝑃)
    with 𝑃 := 𝑥² + 𝑥 + 1  */
class 𝔽₄
{
public:
    constexpr 𝔽₄(val_t value = 0)
        : value_ {value}
    {
        assert(value < 4 && "F4 constructor argument must be < 4.");
        if (value > 3)
            throw std::out_of_range("F4 constructor argument must be < 4.");
    }

    /*  dictionary:
        0 <=> 0
        1 <=> 1
        2 <=> α := 𝑥 mod 𝑃
        3 <=> α + 1  */
        
    friend constexpr 𝔽₄ operator+(const 𝔽₄& elt1, const 𝔽₄& elt2);
    friend constexpr 𝔽₄ operator-(const 𝔽₄& elt1, const 𝔽₄& elt2);
    friend constexpr 𝔽₄ operator*(const 𝔽₄& elt1, const 𝔽₄& elt2);
    friend constexpr 𝔽₄ operator/(const 𝔽₄& elt1, const 𝔽₄& elt2);

    constexpr 𝔽₄ operator-() const { return *this; }

    // increment operator to generate all elements of the field
    constexpr 𝔽₄& operator++()
    {
        if (++value_ == 4)
            value_ = 0;
        return *this;
    }

    constexpr 𝔽₄& operator+=(const 𝔽₄& other) { return *this = *this + other; }
    constexpr 𝔽₄& operator-=(const 𝔽₄& other) { return *this = *this - other; }
    constexpr 𝔽₄& operator*=(const 𝔽₄& other) { return *this = *this * other; }
    constexpr 𝔽₄& operator/=(const 𝔽₄& other) { return *this = *this / other; }

    friend constexpr bool operator==(const 𝔽₄& elt1, const 𝔽₄& elt2);
    friend constexpr bool operator!=(const 𝔽₄& elt1, const 𝔽₄& elt2);

    friend std::ostream& operator<<(std::ostream& os, const 𝔽₄& elt);

private:
    val_t value_;

    using Table = std::array<std::array<val_t, 4>, 4>;

    static constexpr Table addition
    {{
        {0, 1, 2, 3},
        {1, 0, 3, 2},
        {2, 3, 0, 1},
        {3, 2, 1, 0}
    }};

    static constexpr Table multiplication
    {{
        {0, 0, 0, 0},
        {0, 1, 2, 3},
        {0, 2, 3, 1},
        {0, 3, 1, 2}
    }};

    static constexpr Table division
    {{
        {4, 0, 0, 0},  // 4 serves as a filler (not accessed in the code)
        {4, 1, 3, 2},
        {4, 2, 1, 3},
        {4, 3, 2, 1}
    }};
};

inline constexpr 𝔽₄ α {2};


constexpr 𝔽₄ operator+(const 𝔽₄& elt1, const 𝔽₄& elt2)
{
    return {𝔽₄::addition[elt1.value_][elt2.value_]};
}

constexpr 𝔽₄ operator-(const 𝔽₄& elt1, const 𝔽₄& elt2)
{
    return {𝔽₄::addition[elt1.value_][elt2.value_]};
}

constexpr 𝔽₄ operator*(const 𝔽₄& elt1, const 𝔽₄& elt2)
{
    return {𝔽₄::multiplication[elt1.value_][elt2.value_]};
}

constexpr 𝔽₄ operator/(const 𝔽₄& elt1, const 𝔽₄& elt2)
{
    assert(elt2.value_ != 0 && "Element 0 of F4 is not invertible.");
    if (elt2.value_ == 0)
        throw std::invalid_argument("Element 0 of F4 is not invertible.");
    return {𝔽₄::division[elt1.value_][elt2.value_]};
}

constexpr bool operator==(const 𝔽₄& elt1, const 𝔽₄& elt2)
{
    return elt1.value_ == elt2.value_;
}

constexpr bool operator!=(const 𝔽₄& elt1, const 𝔽₄& elt2)
{
    return elt1.value_ != elt2.value_;
}


std::ostream& operator<<(std::ostream& os, const 𝔽₄& elt)
{
    switch (elt.value_)
    {
    case 0: os << 0; break;
    case 1: os << 1; break;
    case 2: os << "alpha"; break;
    case 3: os << "alpha+1"; break;
    default:
        assert(elt.value_ < 4 && "Invalid F4 value.");
        if (elt.value_ > 3)
            throw std::out_of_range("Invalid F4 value.");
        break;
    }
    return os;
}





/*  Given a field 𝑘, Pol<𝑘, 𝑑> is the vector space 𝑘_𝑑[𝑥], that is,
    the vector space of polynomials in 𝑥 over 𝑘 of degree at most 𝑑.  */
template <typename k, deg_t d>
class Pol
{
public:
    /*  Warning on constructor Pol: {λ₀, λ₁} will be interpreted
        as the polynomial λ₁𝑥 + λ₀, i.e. the list initializer
        is in ascending order of powers of 𝑥.  */
    constexpr Pol(std::array<k, d+1> coeffs = {})
        : coeffs_ {coeffs}
    {
        assert(coeffs.size() == d+1 && "Coefficient array size must be d+1.");
        if (coeffs.size() != d+1)
            throw std::out_of_range("Coefficient array size must be d+1.");
    }


    constexpr deg_t deg() const
    {
        for (deg_t degree {d}; degree != 0; --degree)
            if (coeffs_[degree] != k{})
                return degree;

        /*  In this implementation, 0 is of degree 0.
            This is okay because the degree is only used to verify
            there is no overflow in polynomial multiplication.  */
        return 0;
    }

    template <typename K, deg_t D>
    friend constexpr Pol<K, D> operator+(const Pol<K, D>& pol1,
                                         const Pol<K, D>& pol2);

    template <typename K, deg_t D>
    friend constexpr Pol<K, D> operator-(const Pol<K, D>& pol1,
                                         const Pol<K, D>& pol2);

    template <typename K, deg_t D>
    friend constexpr Pol<K, D> operator*(const Pol<K, D>& pol1,
                                         const Pol<K, D>& pol2);

    constexpr Pol& operator+=(const Pol& other) { return *this = *this + other; }
    constexpr Pol& operator-=(const Pol& other) { return *this = *this - other; }    
    constexpr Pol& operator*=(const Pol& other) { return *this = *this * other; }

    template <typename K, deg_t D>
    friend constexpr bool operator==(const Pol<K, D>& pol1,
                                     const Pol<K, D>& pol2);

    template <typename K, deg_t D>
    friend constexpr bool operator!=(const Pol<K, D>& pol1,
                                     const Pol<K, D>& pol2);


    // increment operator to generate all polynomials
    constexpr Pol& operator++()
    {
        bool carry {true};
        for (deg_t i {}; carry && i <= d; ++i)
            carry = ++coeffs_[i] == k{};
        return *this;
    }

    // polynomial evaluation
    constexpr k operator()(const k& scalar) const
    {
        k result {coeffs_[0]};
        k scalar_pow_i {1};
        for (deg_t i {1}; i <= d; ++i)
        {
            scalar_pow_i *= scalar;
            result += coeffs_[i] * scalar_pow_i;
        }
        return result;
    }

    template <typename K, deg_t D>
    friend std::ostream& operator<<(std::ostream& os, const Pol<K, D>& poly);

private:
    std::array<k, d+1> coeffs_;  // coeffs_[i] is the coefficient in front of 𝑥^i
};

template <typename k, deg_t d>
inline constexpr Pol<k, d> 𝑥 {{0, 1}};


template <typename k, deg_t d>
constexpr Pol<k, d> operator+(const Pol<k, d>& pol1,
                              const Pol<k, d>& pol2)
{
    std::array<k, d+1> sum {};
    for (deg_t i {}; i <= d; ++i)
        sum[i] = pol1.coeffs_[i] + pol2.coeffs_[i];
    return {sum};
}

template <typename k, deg_t d>
constexpr Pol<k, d> operator-(const Pol<k, d>& pol1,
                              const Pol<k, d>& pol2)
{
    std::array<k, d+1> diff {};
    for (deg_t i {}; i <= d; ++i)
        diff[i] = pol1.coeffs_[i] - pol2.coeffs_[i];
    return {diff};
}

template <typename k, deg_t d>
constexpr Pol<k, d> operator*(const Pol<k, d>& pol1,
                              const Pol<k, d>& pol2)
{
    assert(d >= pol1.deg() + pol2.deg() && "Degree overflow.");
    if (d < pol1.deg() + pol2.deg())
        throw std::overflow_error("Degree overflow.");

    std::array<k, d+1> prod {};
    for (deg_t i {}; i <= d; ++i)
        for (deg_t j {}; j <= i; ++j)
            prod[i] += pol1.coeffs_[j] * pol2.coeffs_[i-j];
    return {prod};
}

template <typename k, deg_t d>
constexpr bool operator==(const Pol<k, d>& pol1,
                          const Pol<k, d>& pol2)
{
    return pol1.coeffs_ == pol2.coeffs_;
}

template <typename k, deg_t d>
constexpr bool operator!=(const Pol<k, d>& pol1,
                          const Pol<k, d>& pol2)
{
    return pol1.coeffs_ != pol2.coeffs_;
}

template <typename k, deg_t d>
inline std::ostream& operator<<(std::ostream& os, const Pol<k, d>& poly)
{
    for (deg_t i {}; i <= d; ++i)
    {
        switch (d-i)
        {
        case 0: os << poly.coeffs_[d-i]; break;
        case 1: os << poly.coeffs_[d-i] << ".x + "; break;
        default: os << poly.coeffs_[d-i] << ".x^" << d-i << " + "; break;
        }
    }
    return os;
}




// Lagrange interpolation
template <typename k, deg_t d>
constexpr Pol<k, d> lagrange(std::array<k, d+1> key_indexes,
                             std::array<k, d+1> key_values)
{
    Pol<k, d> result {};
    for (deg_t i {}; i <= d; ++i)
    {
        Pol<k, d> term_i {{1}};
        for (deg_t j {}; j <= d; ++j)
        {
            if (i==j) continue;
            term_i *= Pol<k, d> {{key_values[i] * key_indexes[j] /
                                  (key_indexes[j] - key_indexes[i]),
                                  key_values[i] /
                                  (key_indexes[i] - key_indexes[j])}};
        }
        result += term_i;
    }
    return result;
}


#endif
