//
// Created by Лена on 01.08.2024.
//
#include <cmath>
#include <gtest/gtest.h>

#include <fraction.h>

TEST(positive_tests, test1)
{
    fraction f(big_integer(3), big_integer(9));

    auto res = f.pow(2);

    std::stringstream ss;
    ss << res;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "1/9");
}

TEST(positive_tests, test2)
{
    big_integer a(15);
    big_integer b(20);
    big_integer c = fraction::gcd(a, b);
    big_integer d = fraction::lcm(a, b);

    std::stringstream ss;
    ss << c << ' ' << d;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "5 60");
}

TEST(positive_tests, test3)
{
    fraction eps(big_integer(1), big_integer(1000));
    fraction a(big_integer(314), big_integer(600));

    fraction b = a.sin(eps);
    fraction c = a.cos(eps);

    fraction res = b.pow(2) + c.pow(2);

    std::stringstream ss;
    ss << res;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "104719887951095152986401/104976000000000000000000");
}

TEST(positive_tests, test4)
{
    fraction eps(big_integer(1), big_integer(100));
    fraction e = fraction(big_integer(1), big_integer(1)).exp(eps);

    e = e.ln(eps);

    std::stringstream ss;
    ss << e;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "2086408/2114907");
}

TEST(positive_tests, test5)
{
    fraction a = fraction(big_integer("3285034645907645745346457568678465400"), big_integer(100));
    fraction b = fraction(big_integer("423534596495087569087908753095322000"), big_integer(1000));

    fraction result_of_sum = a + b;

    std::stringstream ss;
    ss << result_of_sum;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "33273881055571545022552484439879976/1");
}

TEST(positive_tests, test6)
{
    big_integer one(1);

    fraction a = fraction(big_integer("32850346459076457453464575686784654"), one);
    fraction b = fraction(big_integer("000004235345964950875690879087530953224536645754676574564564756757500"), big_integer(100));

    fraction result_of_sub = a - b;

    std::stringstream ss;
    ss << result_of_sub;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "-42353459649508756908790875276681898907381089312281069960782921/1");
}

TEST(positive_tests, test7)
{
    big_integer one(1);

    fraction a = fraction(big_integer("-1"), big_integer("3"));
    fraction b = fraction(big_integer("1"), big_integer("3"));

    fraction result_of_sub = a / b;

    std::stringstream ss;
    ss << result_of_sub;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "-1/1");
}

TEST(positive_tests, test8)
{
    fraction eps = fraction(big_integer(1), big_integer(100));

    fraction x = fraction(big_integer("9"), big_integer("1"));

    x = x.root(2, eps);

    std::stringstream ss;
    ss << x;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "216194227313680944291535780891592681559410753/69973193905781272405874915421009063720703125");
}


