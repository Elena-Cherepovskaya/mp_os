#ifndef MP_OS_FRACTION_H
#define MP_OS_FRACTION_H

#include <big_integer.h>

class fraction final
{
private:

    big_integer _numerator;
    big_integer _denominator;

public:

    fraction() = default;

    fraction(
        big_integer const &,
        big_integer const &);

    fraction(
        big_integer &&numerator,
        big_integer &&denominator);

public:
    ~fraction() noexcept = default;

    fraction(
        fraction const &) = default;

    fraction & operator=(
        fraction const &) = default;

    fraction(
        fraction &&other) noexcept;

    fraction &operator=(
        fraction &&other) noexcept;

public:

    fraction &operator+=(
        fraction const &other);

    fraction operator+(
        fraction const &other) const;

    fraction &operator-=(
        fraction const &other);

    fraction operator-(
        fraction const &other) const;

    fraction &operator*=(
        fraction const &other);

    fraction operator*(
        fraction const &other) const;

    fraction &operator/=(
        fraction const &other);

    fraction operator/(
        fraction const &other) const;

    static void simple_add(fraction &, fraction const &);

    static void simple_subtr(fraction &, fraction const &);

    static void simple_multiply(fraction &, fraction const &);

    static void simple_division(fraction &, fraction const &);

    static void reduce(fraction &);

    static void make_common_denominator(fraction &, fraction &);

public:

    bool operator==(
        fraction const &other) const;

    bool operator!=(
        fraction const &other) const;

public:

    bool operator>=(
        fraction const &other) const;

    bool operator>(
        fraction const &other) const;

    bool operator<=(
        fraction const &other) const;

    bool operator<(
        fraction const &other) const;

public:

    friend std::ostream &operator<<(
        std::ostream &stream,
        fraction const &obj);

    friend std::istream &operator>>(
        std::istream &stream,
        fraction &obj);

public:

    fraction sin(
        fraction const &epsilon) const;

    fraction cos(
        fraction const &epsilon) const;

    fraction tg(
        fraction const &epsilon) const;

    fraction ctg(
        fraction const &epsilon) const;

    fraction sec(
        fraction const &epsilon) const;

    fraction cosec(
        fraction const &epsilon) const;

    fraction arcsin(
        fraction const &epsilon) const;

    fraction arccos(
        fraction const &epsilon) const;

    fraction arctg(
        fraction const &epsilon) const;

    fraction arcctg(
        fraction const &epsilon) const;

    fraction arcsec(
        fraction const &epsilon) const;

    fraction arccosec(
        fraction const &epsilon) const;

    static big_integer gcd(big_integer const &, big_integer const &);

    static big_integer lcm(big_integer const &, big_integer const &);

public:

    fraction pow(
        size_t degree) const;


public:

    fraction root(
        size_t degree,
        fraction const &epsilon) const;

public:

    fraction log2(
        fraction const &epsilon) const;

    fraction ln(
        fraction const &epsilon) const;

    fraction lg(
        fraction const &epsilon) const;

    fraction exp(
        fraction const &epsilon) const;

};

#endif //MP_OS_FRACTION_H