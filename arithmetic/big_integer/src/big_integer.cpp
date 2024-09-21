#include "../include/big_integer.h"

#include <string>


// big_integer &big_integer::trivial_multiplication::multiply(
//     big_integer &first_multiplier,
//     big_integer const &second_multiplier) const
// {
//     throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
// }
//
// big_integer &big_integer::Karatsuba_multiplication::multiply(
//     big_integer &first_multiplier,
//     big_integer const &second_multiplier) const
// {
//     throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
// }
//
// big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
//     big_integer &first_multiplier,
//     big_integer const &second_multiplier) const
// {
//     throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
// }
//
// big_integer &big_integer::trivial_division::divide(
//     big_integer &dividend,
//     big_integer const &divisor,
//     big_integer::multiplication_rule multiplication_rule) const
// {
//     throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
// }
//
// big_integer &big_integer::trivial_division::modulo(
//     big_integer &dividend,
//     big_integer const &divisor,
//     big_integer::multiplication_rule multiplication_rule) const
// {
//     throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
// }
//
// big_integer &big_integer::Newton_division::divide(
//     big_integer &dividend,
//     big_integer const &divisor,
//     big_integer::multiplication_rule multiplication_rule) const
// {
//     throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
// }
//
// big_integer &big_integer::Newton_division::modulo(
//     big_integer &dividend,
//     big_integer const &divisor,
//     big_integer::multiplication_rule multiplication_rule) const
// {
//     throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
// }
//
// big_integer &big_integer::Burnikel_Ziegler_division::divide(
//     big_integer &dividend,
//     big_integer const &divisor,
//     big_integer::multiplication_rule multiplication_rule) const
// {
//     throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
// }
//
// big_integer &big_integer::Burnikel_Ziegler_division::modulo(
//     big_integer &dividend,
//     big_integer const &divisor,
//     big_integer::multiplication_rule multiplication_rule) const
// {
//     throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
// }

big_integer const big_integer::zero = big_integer(0);

big_integer::big_integer(int value)
{
    _sign = value < 0;
    if (_sign)
        _digits.push_back(-value);
    else
        _digits.push_back(value);
}

big_integer::big_integer(bool sign,
    std::vector<uint32_t> const &digits)
    : _sign(sign)
    , _digits(digits)
{
}


big_integer::big_integer(
    std::string const & value_as_string,
    size_t base)
{
    big_integer const long_base(base);
    bool sign = false;
    for(auto c : value_as_string)
    {
        if (c == '-')
        {
            sign = true;
            continue;
        }

        if (base <= 10)
        {
            if ((c >= '0') && (c < '0' + base))
            {
                (*this) *= long_base;
                (*this) += big_integer((int)(c - '0'));
            }
        }
    }
    _sign = sign;
}


big_integer::big_integer(big_integer const &other)
    : _sign(other._sign)
    , _digits(other._digits)
{
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other)
    {
        _sign = other._sign;
        _digits = other._digits;
    }
    return *this;
}

big_integer::big_integer(big_integer &&other) noexcept
    : _sign(other._sign)
    , _digits(std::move(other._digits))
{
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    if (this != &other)
    {
        _sign = other._sign;
        _digits = std::move(other._digits);
    }
    return *this;
}

void big_integer::set_sign(bool const s)
{
    this->_sign = s;
}

bool big_integer::get_sign() const
{
    return this->_sign;
}

bool big_integer::is_even() const
{
    if (*this == zero)
        return true;

    return (this->_digits[0] & 1) == 0;
}

bool big_integer::is_odd() const
{
    return !is_even();
}


bool big_integer::operator==(
    big_integer const &other) const
{
    if (this->_sign != other._sign || this->_digits.size() != other._digits.size())
        return false;

    for(auto it = this->_digits.rbegin(), it_other = other._digits.rbegin(); it != this->_digits.rend(); ++it, ++it_other)
    {
        if (*it != *it_other)
            return false;
    }

    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    return !(*this == other);
}

bool big_integer::operator<(
    big_integer const &other) const
{
    return !(*this >= other);
}

bool big_integer::operator>(
    big_integer const &other) const
{
    if (this->_sign != other._sign)
        return !this->_sign;

    if (!this->_sign)
    {
        if (this->_digits.size() == other._digits.size())
        {
            for(auto it = this->_digits.rbegin(), it_other = other._digits.rbegin(); it != this->_digits.rend(); ++it, ++it_other)
            {
                if (*it == *it_other)
                    continue;

                return *it > *it_other;
            }
        }

        return this->_digits.size() > other._digits.size();
    }
    else
    {
        // Отрицательное
        if (this->_digits.size() == other._digits.size())
        {
            for(auto it = this->_digits.rbegin(), it_other = other._digits.rbegin(); it != this->_digits.rend(); ++it, ++it_other)
            {
                if (*it == *it_other)
                    continue;

                return *it < *it_other;
            }
        }

        return this->_digits.size() < other._digits.size();
    }
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    return !(*this > other);
}

bool big_integer::operator>=(big_integer const &other) const
{
    return (*this > other || *this == other);
}

big_integer big_integer::operator-() const
{
    auto res(*this);
    res._sign = !res._sign;
    return res;
}

uint32_t big_integer::halves_add(uint32_t &res, uint32_t a, uint32_t b)
{
    uint32_t c = 0;

    uint32_t const a_right_half = a & 0xffff;
    uint32_t const a_left_half = a >> 16;

    uint32_t const b_right_half = b & 0xffff;
    uint32_t const b_left_half = b >> 16;

    uint32_t tmp_res = a_right_half + b_right_half;
    res = tmp_res;
    tmp_res = a_left_half + b_left_half;

    c = tmp_res >> 16;
    res += (tmp_res << 16);

    return c;
}

void big_integer::simple_add(big_integer & res, big_integer const & rh)
{
    uint32_t c = 0;
    
    auto it_res = res._digits.begin();
    auto it_rh = rh._digits.cbegin();

    while (
        (it_res != res._digits.end()) ||
        (it_rh != rh._digits.cend()) || 
        (c != 0))
    {
        if (it_res == res._digits.end())
        {
            res._digits.push_back(0);
            it_res = res._digits.end();
            --it_res;
        }

        halves_add(*it_res, *it_res, c);
        c = halves_add(*it_res, *it_res, (it_rh == rh._digits.cend() ? 0 : *it_rh));

        if (it_res != res._digits.end())
            ++it_res;

        if (it_rh != rh._digits.cend())            
            ++it_rh;
    }

    if (c != 0)
        res._digits.push_back(c);
}

// a > b
uint32_t big_integer::halves_subtr(uint32_t &res, uint32_t a, uint32_t b)
{
    uint32_t c = 0;

    uint32_t const a_right_half = a & 0xffff;
    uint32_t const a_left_half = a >> 16;

    uint32_t const b_right_half = b & 0xffff;
    uint32_t const b_left_half = b >> 16;

    uint32_t tmp_res = a_right_half - b_right_half;
    c = tmp_res >> 16 & 0x1;

    res = tmp_res & 0xffff;

    tmp_res = a_left_half - b_left_half - c;
    c = tmp_res >> 16 & 0x1;

    res |= (tmp_res & 0xffff) << 16;

    return c;
}

// res >= a
void big_integer::simple_subtr(big_integer & res, big_integer const & rh)
{
    uint32_t c = 0;
    
    auto it_res = res._digits.begin();
    auto it_rh = rh._digits.cbegin();

    while (it_res != res._digits.end())
    {
        halves_subtr(*it_res, *it_res, c);
        c = halves_subtr(*it_res, *it_res, (it_rh == rh._digits.cend() ? 0 : *it_rh));

        if (it_res != res._digits.end())
            ++it_res;

        if (it_rh != rh._digits.cend())            
            ++it_rh;

    }

    while (*res._digits.rbegin() == 0 && (res._digits.size() > 1))
        res._digits.erase(--res._digits.end());
}


big_integer &big_integer::operator+=(
    big_integer const &other)
{
    if (this->_sign == other._sign)
    {
        simple_add(*this, other);
        return *this;
    }

    if (!this->_sign)
        *this -= other;
    else
    {
        big_integer tmp(other);
        tmp -= *this;
    }

    return *this;
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    big_integer res(*this);
    res += other;
    return res;
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    if (!this->_sign && !other._sign)
    {
        if (*this >= other)
        {
            simple_subtr(*this, other);
            return *this;
        }
        else
        {
            big_integer tmp(other);
            simple_subtr(tmp, *this);
            *this = tmp;
            this->_sign = true;
        }

        return *this;
    }

    if ((this->_sign && other._sign) || (this->_sign && !other._sign))
    {
        // this < 0
        this->_sign = false;
        big_integer tmp(other);
        tmp._sign = false;

        if (*this > other)
        {
            simple_subtr(*this, tmp);
            this->_sign = true;
        }
        else
        {
            simple_subtr(tmp, *this);
            *this = tmp;
        }
    }

    return *this;
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    big_integer res(*this);
    res -= other;
    return res;
}

big_integer const & big_integer::move_left()
{
    uint32_t cur_bit;
    uint32_t prev_bit = 0;

    for (auto it = _digits.begin(); it != _digits.end(); ++it)
    {
        cur_bit = (*it) & 0x80000000ul;
        cur_bit >>= 31;

        (*it) <<= 1;
        (*it) |= prev_bit;

        prev_bit = cur_bit;
    }

    if (prev_bit)
        _digits.push_back(prev_bit);

    return *this;
}

big_integer const & big_integer::move_left(big_integer a)
{
    return a.move_left();
}

big_integer const & big_integer::move_right()
{
    uint32_t prev_bit = 0;

    for (auto it = _digits.rbegin(); it != _digits.rend(); ++it)
    {
        uint32_t const cur_bit = ((*it) & 0x1ul) << 31;

        (*it) >>= 1;
        (*it) |= prev_bit;

        prev_bit = cur_bit;
    }

    if ((*_digits.rbegin() == 0) && _digits.size() > 1)
        _digits.pop_back();

    return *this;
}

big_integer const & big_integer::move_right(big_integer a)
{
    return a.move_right();
}

void big_integer::simple_multiply(big_integer & res, big_integer const & rh)
{
    // a * b
    big_integer a(res);
    a.set_sign(false);

    big_integer tmp_res(0);

    for (auto it = rh._digits.cbegin(); it != rh._digits.cend(); ++it)
    {
        auto b = *it;

        for (int i = 0; i < 32; ++i)
        {
            if (b & 0x1)
                tmp_res += a;

            a.move_left();
            b >>= 1;
        }
    }

    res = tmp_res;
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    bool res_sign = this->_sign ^ other._sign;
    simple_multiply(*this, other);
    this->_sign = res_sign;

    return *this;
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    big_integer res(*this);
    res *= other;
    return res;
}

big_integer big_integer::simple_division(big_integer const & res, big_integer const & rh, big_integer & remainder)
{
    big_integer divisible(res);
    divisible._sign = false;

    big_integer divider(rh);
    divider._sign = false;

    big_integer tmp_res(0);
    big_integer s(1);

    while (true)
    {
        if (divisible >= divider)
        {
            divider.move_left();
            s.move_left();

            if (divisible < divider)
            {
                divider.move_right();
                s.move_right();
                break;
            }
        }
        else
            break;
    }

    while(divisible >= rh)
    {
        if (divisible >= divider)
        {
            divisible -= divider;
            tmp_res += s;
        }

        divider.move_right();
        s.move_right();
    }

    remainder = divisible;
    return tmp_res;
}

big_integer big_integer::simple_division(big_integer const & res, big_integer const &rh)
{
    big_integer m;
    return simple_division(res, rh, m);
}

// this > other
big_integer &big_integer::operator/=(
    big_integer const &other)
{
    bool const res_sign = _sign ^ other._sign;

    *this = simple_division(*this, other);
    _sign = res_sign;

    return *this;
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    big_integer res(*this);
    res /= other;
    return res;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    bool this_sign = this->_sign;
    bool other_sign = other._sign;

    this->_sign = false;
    big_integer tmp_other(other);
    tmp_other._sign = false;

    if (tmp_other > *this)
    {
        if (this_sign)
            this->_sign = false;
    }
    else
    {
        big_integer tmp_remainder(0);
        simple_division(*this, other, tmp_remainder);
        *this = tmp_remainder;
    }

    return *this;
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    big_integer res(*this);
    res %= other;
    return res;
}


big_integer big_integer::operator~() const
{
    big_integer res(*this);

    for (auto it = res._digits.begin(); it != res._digits.end(); ++it)
        ~(*it);

    return res;
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    auto it_this = this->_digits.begin();
    auto it_other = other._digits.begin();

    while (it_this != this->_digits.end())
    {
        if (it_other == other._digits.end())
        {
            *it_this &= 0;
            continue;
        }

        *it_this &= *it_other;

        ++it_this;
        ++it_other;
    }

    return *this;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    big_integer res(*this);
    res &= other;
    return res;
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    auto it_this = this->_digits.begin();
    auto it_other = other._digits.begin();

    while (it_this != this->_digits.end())
    {
        if (it_other == other._digits.end())
        {
            *it_this |= 0;
            continue;
        }

        *it_this |= *it_other;

        ++it_this;
        ++it_other;
    }

    return *this;
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    big_integer res(*this);
    res |= other;
    return res;
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    auto it_this = this->_digits.begin();
    auto it_other = other._digits.begin();

    while (it_this != this->_digits.end())
    {
        if (it_other == other._digits.end())
        {
            *it_this ^= 0;
            continue;
        }

        *it_this ^= *it_other;

        ++it_this;
        ++it_other;
    }

    return *this;
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    big_integer res(*this);
    res ^= other;
    return res;
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    for (auto i = 0; i < shift; ++i)
        move_left(this->move_left());

    return *this;
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    big_integer res(*this);
    res <<= shift;
    return res;
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    for (auto i = 0; i < shift; ++i)
        move_right(this->move_left());

    return *this;
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    big_integer res(*this);
    res >>= shift;
    return res;
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    big_integer const zero(0);
    big_integer tmp_value(value);
    tmp_value._sign = false;

    big_integer cur_remainder(0);
    big_integer const d(10);

    std::vector<char> res_buffer;

    while(true)
    {
        tmp_value = big_integer::simple_division(tmp_value, d, cur_remainder);
        res_buffer.push_back((char)(cur_remainder._digits[0]) + '0');
        if (tmp_value == zero)
            break;
    }

    if (value._sign)
        res_buffer.push_back('-');

    for (auto it = res_buffer.rbegin(); it != res_buffer.rend(); ++it)
        stream << *it;

    return stream;
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    std::string str;
    std::getline (stream, str);
    value = big_integer(str);
    return stream;
}
