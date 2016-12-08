#include <boost/yap/expression.hpp>

#include <boost/hana/maximum.hpp>

#include <iostream>


struct get_arity
{
    template <typename Tuple>
    auto operator() (
        boost::yap::expression<
            boost::yap::expr_kind::placeholder,
            Tuple
        > const & expr
    ) { return expr.value(); }

    template <typename Tuple>
    auto operator() (
        boost::yap::expression<
            boost::yap::expr_kind::terminal,
            Tuple
        > const & expr
    ) {
        using namespace boost::hana::literals;
        return 0_c;
    }

    template <typename Expr>
    auto operator() (Expr const & expr)
    {
        return boost::hana::maximum(
            boost::hana::transform(
                expr.elements,
                [](auto const & element) {
                    return boost::yap::transform(element, get_arity{});
                }
            )
        );
    }
};

int main ()
{
    using namespace boost::yap::literals;

    auto expr_1 = 1_p + 2.0;

    auto expr_1_fn = [expr_1](auto &&... args) {
        auto const arity = boost::yap::transform(expr_1, get_arity{});
        static_assert(arity.value == sizeof...(args), "Called with wrong number of args.");
        return evaluate(expr_1, args...);
    };

    auto expr_2 = 1_p * 2_p;

    auto expr_2_fn = [expr_2](auto &&... args) {
        auto const arity = boost::yap::transform(expr_2, get_arity{});
        static_assert(arity.value == sizeof...(args), "Called with wrong number of args.");
        return evaluate(expr_2, args...);
    };

    auto expr_3 = (1_p - 2_p) / 2_p;

    auto expr_3_fn = [expr_3](auto &&... args) {
        auto const arity = boost::yap::transform(expr_3, get_arity{});
        static_assert(arity.value == sizeof...(args), "Called with wrong number of args.");
        return evaluate(expr_3, args...);
    };

    // Displays "5"
    std::cout << expr_1_fn(3.0) << std::endl;

    // Displays "6"
    std::cout << expr_2_fn(3.0, 2.0) << std::endl;

    // Displays "0.5"
    std::cout << expr_3_fn(3.0, 2.0) << std::endl;

    // Static-asserts with "Called with wrong number of args."
    //std::cout << expr_3_fn(3.0) << std::endl;

    // Static-asserts with "Called with wrong number of args."
    //std::cout << expr_3_fn(3.0, 2.0, 1.0) << std::endl;

    return 0;
}
