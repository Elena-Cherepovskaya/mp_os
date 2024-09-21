#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H

#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>

// #include <allocator.h>
// #include <allocator_guardant.h>
#include <not_implemented.h>

class big_integer
{

public:
    
    enum class multiplication_rule
    {
        trivial,
        Karatsuba,
        SchonhageStrassen
    };

private:
    
    class multiplication
    {
    public:
        
        virtual ~multiplication() noexcept = default;
    
    public:
        
        virtual big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const = 0;
        
    };
    
    class trivial_multiplication final:
        public multiplication
    {
    
    public:
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };
    
    class Karatsuba_multiplication final:
        public multiplication
    {

    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };
    
    class Schonhage_Strassen_multiplication final:
        public multiplication
    {

    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };

public:
    
    enum class division_rule
    {
        trivial,
        Newton,
        BurnikelZiegler
    };

private:
    
    class division
    {

    public:

        virtual ~division() noexcept = default;
    
    public:
        
        virtual big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;
        
        virtual big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;
        
    };
    
    class trivial_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };
    
    class Newton_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };
    
    class Burnikel_Ziegler_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };

private:

    // int _oldest_digit;
    // unsigned int *_other_digits;
    // allocator *_allocator;

    bool _sign = false;
    std::vector<uint32_t> _digits;
    
public:
    big_integer() = default;

    explicit big_integer(int value);

    explicit big_integer(
        bool sign,
        std::vector<uint32_t> const &digits);

    explicit big_integer(
        std::string const &value_as_string,
        size_t base = 10);

public:

    static big_integer const zero;

    ~big_integer() noexcept = default;
    
    big_integer(
        big_integer const &other);

    big_integer &operator=(
        big_integer const &other);
    
    big_integer(
        big_integer &&other) noexcept;
    
    big_integer &operator=(
        big_integer &&other) noexcept;

    void set_sign(bool);
    bool get_sign() const;

    bool is_even() const; // четный
    bool is_odd() const; // нечетный

private:
    static uint32_t halves_add(uint32_t &, uint32_t, uint32_t);

    static uint32_t halves_subtr(uint32_t &res, uint32_t a, uint32_t b);

public:

    big_integer const & move_left();
    static big_integer const & move_left(big_integer);

    big_integer const & move_right();
    static big_integer const & move_right(big_integer);

public:

    bool operator==(
        big_integer const &other) const;

    bool operator!=(
        big_integer const &other) const;

public:

    bool operator<(
        big_integer const &other) const;

    bool operator>(
        big_integer const &other) const;

    bool operator<=(
        big_integer const &other) const;

    bool operator>=(
        big_integer const &other) const;

public:

    static void simple_add(big_integer &, big_integer const &);

    static void simple_subtr(big_integer &, big_integer const &);

    static void simple_multiply(big_integer &, big_integer const &);

    static big_integer simple_division(big_integer const &, big_integer const &, big_integer &);
    static big_integer simple_division(big_integer const &, big_integer const &);

public:

    big_integer operator-() const;
    
    big_integer &operator+=(
        big_integer const &other);

    big_integer operator+(
        big_integer const &other) const;

    // big_integer operator+(
    //     std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator-=(
        big_integer const &other);

    big_integer operator-(
        big_integer const &other) const;

    big_integer &operator*=(
        big_integer const &other);

    big_integer operator*(
        big_integer const &other) const;

    big_integer &operator/=(
        big_integer const &other);

    big_integer operator/(
        big_integer const &other) const;

    big_integer &operator%=(
        big_integer const &other);

    big_integer operator%(
        big_integer const &other) const;

public:

    big_integer operator~() const;

    big_integer &operator&=(
        big_integer const &other);

    big_integer operator&(
        big_integer const &other) const;

    big_integer &operator|=(
        big_integer const &other);

    big_integer operator|(
        big_integer const &other) const;

    big_integer &operator^=(
        big_integer const &other);

    big_integer operator^(
        big_integer const &other) const;

    big_integer &operator<<=(
        size_t shift);

    big_integer operator<<(
        size_t shift) const;

    big_integer &operator>>=(
        size_t shift);

    big_integer operator>>(
        size_t shift) const;

public:

    // static big_integer &multiply(
    //     big_integer &first_multiplier,
    //     big_integer const &second_multiplier,
    //     allocator *allocator = nullptr,
    //     big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);
    //
    // static big_integer multiply(
    //     big_integer const &first_multiplier,
    //     big_integer const &second_multiplier,
    //     allocator *allocator = nullptr,
    //     big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);
    //
    // static big_integer &divide(
    //     big_integer &dividend,
    //     big_integer const &divisor,
    //     allocator *allocator = nullptr,
    //     big_integer::division_rule division_rule = big_integer::division_rule::trivial,
    //     big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);
    //
    // static big_integer divide(
    //     big_integer const &dividend,
    //     big_integer const &divisor,
    //     allocator *allocator = nullptr,
    //     big_integer::division_rule division_rule = big_integer::division_rule::trivial,
    //     big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);
    //
    // static big_integer &modulo(
    //     big_integer &dividend,
    //     big_integer const &divisor,
    //     allocator *allocator = nullptr,
    //     big_integer::division_rule division_rule = big_integer::division_rule::trivial,
    //     big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);
    //
    // static big_integer modulo(
    //     big_integer const &dividend,
    //     big_integer const &divisor,
    //     allocator *allocator = nullptr,
    //     big_integer::division_rule division_rule = big_integer::division_rule::trivial,
    //     big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

public:
    
    friend std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value);
    
    friend std::istream &operator>>(
        std::istream &stream,
        big_integer &value);
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H