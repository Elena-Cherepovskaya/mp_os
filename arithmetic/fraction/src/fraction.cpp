#include "../include/fraction.h"

#include <cmath>

fraction::fraction(big_integer const & n, big_integer const & d)
    : _numerator(n)
    , _denominator(d)
{
}

fraction::fraction(
    big_integer &&numerator,
    big_integer &&denominator):
        _numerator(std::forward<big_integer>(numerator)),
        _denominator(std::forward<big_integer>(denominator))
{
}

// fraction::fraction(fraction const & rh)
//     : _numerator(rh._numerator)
//     , _denominator(rh._denominator)
// {
// }

// fraction & fraction::operator=(
//     fraction const &other)
// {
//    if (this != &other)
//    {
//        _numerator = other._numerator;
//        _denominator = other._denominator;
//    }
//     return *this;
// }

fraction::fraction(
    fraction &&other) noexcept:
        _numerator(std::move(other._numerator)),
        _denominator(std::move(other._denominator))
{
}

fraction &fraction::operator=(
    fraction &&other) noexcept
{
    if (this != &other)
    {
        _numerator = std::move(other._numerator);
        _denominator = std::move(other._denominator);
    }
    return *this;
}

void fraction::reduce(fraction & a)
{
    bool a_sign = a._numerator.get_sign();

    a._numerator.set_sign(false);
    big_integer const d = gcd(a._numerator, a._denominator);

    a._numerator /= d;
    a._denominator /= d;

    a._numerator.set_sign(a_sign);
}

void fraction::make_common_denominator(fraction & a, fraction & b)
{
    big_integer const new_denominator = lcm(a._denominator, b._denominator);

    big_integer c = new_denominator / a._denominator;
    a._numerator *= c;
    a._denominator *= c;

    c = new_denominator / b._denominator;
    b._numerator *= c;
    b._denominator *= c;
}

void fraction::simple_add(fraction & a, fraction const & b)
{
    fraction tmp_b(b);
    if (a._denominator != b._denominator)
        make_common_denominator(a, tmp_b);

    a._numerator += tmp_b._numerator;

    reduce(a);
}

fraction &fraction::operator+=(
    fraction const &other)
{
    if (this->_numerator.get_sign() == other._numerator.get_sign())
    {
        simple_add(*this, other);
        return *this;
    }

    if (!this->_numerator.get_sign())
        *this -= other;
    else
    {
        fraction tmp(other);
        tmp -= *this;
    }

    return *this;
}

fraction fraction::operator+(
    fraction const &other) const
{
    fraction res(*this);
    res += other;
    return res;
}

void fraction::simple_subtr(fraction & a, fraction const & b)
{
    fraction tmp_b(b);
    if (a._denominator != b._denominator)
        make_common_denominator(a, tmp_b);

    a._numerator -= tmp_b._numerator;

    reduce(a);
}

fraction &fraction::operator-=(
    fraction const &other)
{
    if (!this->_numerator.get_sign() && !other._numerator.get_sign())
    {
        if (*this >= other)
        {
            simple_subtr(*this, other);
            return *this;
        }
        else
        {
            fraction tmp(other);
            simple_subtr(tmp, *this);
            *this = tmp;
            this->_numerator.set_sign(true);
        }

        return *this;
    }

    if ((this->_numerator.get_sign() && other._numerator.get_sign()) || (this->_numerator.get_sign() && !other._numerator.get_sign()))
    {
        // this < 0
        this->_numerator.set_sign(false);
        fraction tmp(other);
        tmp._numerator.set_sign(false);

        if (*this > other)
        {
            simple_subtr(*this, tmp);
            this->_numerator.set_sign(true);
        }
        else
        {
            simple_subtr(tmp, *this);
            *this = tmp;
        }
    }

    return *this;
}

fraction fraction::operator-(
    fraction const &other) const
{
    fraction res(*this);
    res -= other;
    return res;
}

void fraction::simple_multiply(fraction & a, fraction const & b)
{
    a._numerator *= b._numerator;
    a._denominator *= b._denominator;

    reduce(a);
}

fraction &fraction::operator*=(
    fraction const &other)
{
    bool res_sign = this->_numerator.get_sign() ^ other._numerator.get_sign();
    simple_multiply(*this, other);
    this->_numerator.set_sign(res_sign);

    return *this;
}

fraction fraction::operator*(
    fraction const &other) const
{
    fraction res(*this);
    res *= other;
    return res;
}

void fraction::simple_division(fraction & a, fraction const & b)
{
    fraction tmp_b(b._denominator, b._numerator);

    simple_multiply(a, tmp_b);

    reduce(a);
}

fraction &fraction::operator/=(
    fraction const &other)
{
    bool const res_sign = _numerator.get_sign() ^ other._numerator.get_sign();

    simple_division(*this, other);
    _numerator.set_sign(res_sign);

    return *this;
}

fraction fraction::operator/(
    fraction const &other) const
{
    fraction res(*this);
    res /= other;
    return res;
}

bool fraction::operator==(
    fraction const &other) const
{
    if (_numerator.get_sign() != other._numerator.get_sign())
        return false;

    fraction a(_numerator * other._denominator, this->_denominator * other._denominator);

    fraction b(other._numerator * _denominator, other._denominator * _denominator);

    return _numerator == other._numerator;
}

bool fraction::operator!=(
    fraction const &other) const
{
    return !(*this == other);
}

bool fraction::operator>=(
    fraction const &other) const
{
    return !(*this < other);
}

bool fraction::operator>(
    fraction const &other) const
{
    if (_numerator.get_sign() != other._numerator.get_sign())
        return !_numerator.get_sign();

    fraction a(_numerator * other._denominator, this->_denominator * other._denominator);

    fraction b(other._numerator * _denominator, other._denominator * _denominator);

    //отрицательные
    return (_numerator.get_sign()) ? a._numerator < b._numerator : a._numerator > b._numerator;
}

bool fraction::operator<=(
    fraction const &other) const
{
    return !(*this > other);
}

bool fraction::operator<(
    fraction const &other) const
{
    if (_numerator.get_sign() != other._numerator.get_sign())
        return _numerator.get_sign();

    fraction a(_numerator * other._denominator, this->_denominator * other._denominator);

    fraction b(other._numerator * _denominator, other._denominator * _denominator);

    //отрицательные
    return (_numerator.get_sign()) ? a._numerator > b._numerator : a._numerator < b._numerator;
}

std::ostream &operator<<(
    std::ostream &stream,
    fraction const &obj)
{
    stream << obj._numerator << '/' << obj._denominator;
    return stream;
}

std::istream &operator>>(
    std::istream &stream,
    fraction &obj)
{
    std::string str;

    getline(stream, str, '/');
    obj._numerator = big_integer(str);

    getline(stream, str);
    obj._denominator = big_integer(str);

    return stream;
}

fraction fraction::sin(
    fraction const &epsilon) const
{
    big_integer const one = big_integer(1);

    fraction summ(*this);
    bool cur_sign = true;
    fraction x(*this);
    x *= x;
    x *= *this;

    fraction n(one, one);
    n *= fraction(big_integer(2), one);
    n *= fraction(big_integer(3), one);
    int index_n = 3;
    while(true)
    {
        auto const cur_summ = (x / n);
        if (cur_summ < epsilon)
            break;

        if (cur_sign)
            summ -= cur_summ;
        else
            summ += cur_summ;

        cur_sign = !cur_sign;

        for(int i = 0; i < 2; ++i)
        {
            x *= *this;
            n *= fraction(big_integer(index_n), one);
            ++index_n;
        }
    }

    reduce(summ);

    return summ;
}

fraction fraction::cos(
    fraction const &epsilon) const
{
    big_integer const one = big_integer(1);

    fraction summ(one, one);
    bool cur_sign = true;
    fraction x(*this);
    x *= x;

    fraction n(one, one);
    n *= fraction(big_integer(2), one);

    int index_n = 4;
    while(true)
    {
        auto const cur_summ = (x / n);
        if (cur_summ < epsilon)
            break;

        if (cur_sign)
            summ -= cur_summ;
        else
            summ += cur_summ;

        cur_sign = !cur_sign;

        for(int i = 0; i < 2; ++i)
        {
            x *= *this;
            n *= fraction(big_integer(index_n), one);
            ++index_n;
        }
    }

    reduce(summ);

    return summ;
}

fraction fraction::tg(
    fraction const &epsilon) const
{
    fraction res = sin(epsilon) / cos(epsilon);
    reduce(res);

    return res;
}

fraction fraction::ctg(
    fraction const &epsilon) const
{
    fraction res = cos(epsilon) / sin(epsilon);
    reduce(res);

    return res;
}

fraction fraction::sec(
    fraction const &epsilon) const
{
    return fraction(big_integer(1), big_integer(1)) / cos(epsilon);
}

fraction fraction::cosec(
    fraction const &epsilon) const
{
    return fraction(big_integer(1), big_integer(1)) / sin(epsilon);
}

fraction fraction::arcsin(
    fraction const &epsilon) const
{
    big_integer const one = big_integer(1);

    fraction summ(*this);

    fraction x(*this);
    x *= (*this);
    x *= (*this);

    fraction n(one, one);
    n *= fraction(one, big_integer(2));

    fraction cur_pow(one, big_integer(3));

    int index_numerator = 3;
    int index_denominator = 4;

    while(true)
    {
        auto const cur_summ = (x * n * cur_pow);
        if (cur_summ < epsilon)
            break;

        summ += cur_summ;

        x *= *this;
        x *= *this;
        n *= fraction(big_integer(index_numerator), one);
        n *= fraction(one, big_integer(index_denominator));
        cur_pow = fraction(one, cur_pow._denominator + big_integer(2));

        index_numerator += 2;
        index_denominator += 2;
    }

    reduce(summ);

    return summ;
}

fraction fraction::arccos(
    fraction const &epsilon) const
{
    big_integer const one(1);

    fraction x = fraction(one, one) - this->pow(2);
    x = x.root(2, epsilon);

    return x.arcsin(epsilon);
}

fraction fraction::arctg(
    fraction const &epsilon) const
{
    big_integer const one(1);

    fraction x = this->pow(2) + fraction(one, one);
    x = *this / x.root(2, epsilon);

    return x.arcsin(epsilon);
}

fraction fraction::arcctg(
    fraction const &epsilon) const
{
    big_integer const one(1);

    fraction x = this->pow(2) + fraction(one, one);
    x = fraction(one, one) / x.root(2, epsilon);

    return x.arcsin(epsilon);
}

fraction fraction::arcsec(
    fraction const &epsilon) const
{
    big_integer const one(1);

    fraction x = this->pow(2) - fraction(one, one);
    x = x.root(2, epsilon) / *this;

    return x.arcsin(epsilon);
}

fraction fraction::arccosec(
    fraction const &epsilon) const
{
    big_integer const one(1);
    fraction x = fraction(one, one) / *this;

    return x.arcsin(epsilon);
}

fraction fraction::pow(
    size_t degree) const
{
    fraction res(big_integer(1), big_integer(1));
    fraction n(*this);

    while(degree != 0)
    {
        if ((degree & 1) != 0)
            res *= n;

        n *= n;
        degree >>= 1;
    }

    return res;
}

fraction fraction::root(
    size_t degree,
    fraction const &epsilon) const
{
    fraction const one_div_f_degree(big_integer(1), big_integer(degree));
    return (one_div_f_degree * this->ln(epsilon)).exp(epsilon);
}

fraction fraction::log2(
    fraction const &epsilon) const
{
    return ln(*this) / ln(fraction(big_integer(2), big_integer(1)));
}

fraction fraction::ln(
    fraction const &epsilon) const
{
    big_integer const one = big_integer(1);
    fraction const f_one(one, one);

    fraction c((*this - f_one) / (*this + f_one));
    fraction x(c);

    fraction summ(x);
    x *= c;
    x *= c;

    fraction n(one, one);
    n *= fraction(big_integer(3), one);

    while(true)
    {
        auto const cur_summ = (x / n);
        if (cur_summ < epsilon)
            break;

        summ += cur_summ;

        x *= c;
        x *= c;

        n += f_one;
    }

    summ *= fraction(big_integer(2), one);
    reduce(summ);

    return summ;
}

fraction fraction::lg(
    fraction const &epsilon) const
{
    return ln(*this) / ln(fraction(big_integer(10), big_integer(1)));
}

fraction fraction::exp(fraction const &epsilon) const
{
    big_integer const one = big_integer(1);

    fraction summ(fraction(one, one));

    fraction x(*this);

    fraction n(big_integer(1), one);

    int index_n = 2;

    while(true)
    {
        auto const cur_summ = (x / n);
        if (cur_summ < epsilon)
            break;

        summ += cur_summ;

        x *= *this;
        n *= fraction(big_integer(index_n), one);

        index_n += 1;
    }

    reduce(summ);

    return summ;
}

big_integer fraction::gcd(big_integer const & m, big_integer const & n)
{
    if ((n == big_integer::zero) || (n == m))
        return m;

    if ((m == big_integer::zero) || (n == m))
        return n;

    if (m == big_integer(1) || n == big_integer(1))
        return big_integer(1);

    if (m.is_even() && n.is_even())
        return big_integer::move_left(gcd(big_integer::move_right(m), big_integer::move_right(n)));

    if (m.is_even() && n.is_odd())
        return gcd(big_integer::move_right(m), n);

    if (n.is_even() && m.is_odd())
        return gcd(m, big_integer::move_right(n));

    if (m.is_odd() && n.is_odd())
        return (n > m) ? gcd(m, big_integer::move_right(n - m)) : gcd(big_integer::move_right(m - n), n);
}

big_integer fraction::lcm(big_integer const & a, big_integer const & b)
{
    return (a * b) / gcd(a, b);
}
