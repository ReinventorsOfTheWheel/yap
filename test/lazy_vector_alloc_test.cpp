#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE 1
#include <boost/yap/expression.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>


int allocations = 0;

void * operator new (std::size_t size)
{
    ++allocations;
    return malloc(size);
}

void operator delete (void * ptr) noexcept
{ free(ptr); }


template <boost::yap::expr_kind Kind, typename Tuple>
struct lazy_vector_expr;


struct take_nth
{
    boost::yap::terminal<lazy_vector_expr, double>
    operator() (boost::yap::terminal<lazy_vector_expr, std::vector<double>> const & expr);

    std::size_t n;
};

template <boost::yap::expr_kind Kind, typename Tuple>
struct lazy_vector_expr
{
    using this_type = lazy_vector_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::lazy_vector_expr)
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(minus, this_type, ::lazy_vector_expr)

    auto operator[] (std::size_t n) const
    { return boost::yap::evaluate(boost::yap::transform(*this, take_nth{n})); }

};

boost::yap::terminal<lazy_vector_expr, double>
take_nth::operator() (boost::yap::terminal<lazy_vector_expr, std::vector<double>> const & expr)
{
    double x = boost::yap::value(expr)[n];
    return boost::yap::make_terminal<lazy_vector_expr, double>(std::move(x));
}

struct lazy_vector :
    lazy_vector_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{
    lazy_vector () {}

    explicit lazy_vector (std::vector<double> && vec)
    { elements = boost::hana::tuple<std::vector<double>>(std::move(vec)); }

    template <boost::yap::expr_kind Kind, typename Tuple>
    lazy_vector & operator+= (lazy_vector_expr<Kind, Tuple> const & rhs)
    {
        std::vector<double> & this_vec = boost::yap::value(*this);
        for (int i = 0, size = (int)this_vec.size(); i < size; ++i) {
            this_vec[i] += rhs[i];
        }
        return *this;
    }
};


TEST(allocations, lazy_vector_alloc_text)
{
    // GTest apparently allocates a ton of strings.  We need to hit "reset"
    // here to measure the allocations in the part of the code we really care
    // about.
    allocations = 0;

    lazy_vector v1{std::vector<double>(4, 1.0)};
    lazy_vector v2{std::vector<double>(4, 2.0)};
    lazy_vector v3{std::vector<double>(4, 3.0)};

    double d1 = (v2 + v3)[2];
    std::cout << d1 << "\n";

    v1 += v2 - v3;
    std::cout << '{' << v1[0] << ',' << v1[1]
              << ',' << v1[2] << ',' << v1[3] << '}' << "\n";

    EXPECT_EQ(allocations, 3);
}
