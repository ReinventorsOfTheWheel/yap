#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;

namespace reference_returning {

    struct number
    {
        double value;
    };

    number a_result{3.0};
    number const the_result{13.0};

    template <typename A, typename B>
    inline number const & eval_plus (A a, B b)
    { return the_result; }

    template <typename A, typename B>
    inline number & eval_minus (A a, B b)
    { return a_result; }

}

TEST(reference_returns, test_reference_returns)
{
    term<reference_returning::number> unity = {{1.0}};
    auto plus_expr = unity + reference_returning::number{1.0};

    // TODO: This has to fail due to the general implicit conversion rules
    // (see [conv]/3-6).  This needs to be noted in docs.
#if 0
    {
        reference_returning::number const & n = plus_expr;
        EXPECT_EQ(&n, &reference_returning::the_result);
    }
#endif

    {
        reference_returning::number const & n = evaluate(plus_expr);
        EXPECT_EQ(&n, &reference_returning::the_result);
    }

    static_assert(
        std::is_same_v<
            decltype(evaluate(plus_expr)),
            reference_returning::number const &
        >
    );

    auto minus_expr = unity - reference_returning::number{1.0};

    {
        reference_returning::number & n = evaluate(minus_expr);
        EXPECT_EQ(&n, &reference_returning::a_result);
    }

    static_assert(
        std::is_same_v<
            decltype(evaluate(minus_expr)),
            reference_returning::number &
        >
    );

    using namespace bp17::literals;

    {
        reference_returning::number & n = evaluate(0_p, reference_returning::a_result);
        EXPECT_EQ(&n, &reference_returning::a_result);
    }

    static_assert(
        std::is_same_v<
            decltype(evaluate(0_p, reference_returning::a_result)),
            reference_returning::number &
        >
    );

    {
        reference_returning::number const & n = evaluate(0_p, reference_returning::the_result);
        EXPECT_EQ(&n, &reference_returning::the_result);
    }

    static_assert(
        std::is_same_v<
            decltype(evaluate(0_p, reference_returning::the_result)),
            reference_returning::number const &
        >
    );
}