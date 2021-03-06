#include <boost/yap/expression.hpp>

#include <boost/assign/list_of.hpp>

#include <map>
#include <iostream>


template <typename Key, typename Value, typename Allocator>
struct map_list_of_transform
{
    template <typename Fn, typename Key2, typename Value2>
    auto operator() (boost::yap::call_tag, Fn const & fn, Key2 && key, Value2 && value)
    {
        boost::yap::transform(fn, *this);
        map.emplace(
            Key{std::forward<Key2 &&>(key)},
            Value{std::forward<Value2 &&>(value)}
        );
        return 0;
    }

    std::map<Key, Value, Allocator> map;
};


template <boost::yap::expr_kind Kind, typename Tuple>
struct map_list_of_expr
{
    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;

    template <typename Key, typename Value, typename Allocator>
    operator std::map<Key, Value, Allocator> () const
    {
        map_list_of_transform<Key, Value, Allocator> transform;
        boost::yap::transform(*this, transform);
        return transform.map;
    }

    BOOST_YAP_USER_MEMBER_CALL_OPERATOR(::map_list_of_expr)
};

struct map_list_of_tag {};

auto map_list_of = boost::yap::make_terminal<map_list_of_expr>(map_list_of_tag{});


#if __GNUC__ || __clang__
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif


NOINLINE std::map<std::string, int> make_map_with_boost_yap ()
{
    return map_list_of
        ("<", 1)
        ("<=",2)
        (">", 3)
        (">=",4)
        ("=", 5)
        ("<>",6)
        ;
}


NOINLINE std::map<std::string, int> make_map_with_boost_assign ()
{
    return boost::assign::map_list_of
        ("<", 1)
        ("<=",2)
        (">", 3)
        (">=",4)
        ("=", 5)
        ("<>",6)
        ;
}


NOINLINE std::map<std::string, int> make_map_manually ()
{
    std::map<std::string, int> retval;
    retval.emplace("<", 1);
    retval.emplace("<=",2);
    retval.emplace(">", 3);
    retval.emplace(">=",4);
    retval.emplace("=", 5);
    retval.emplace("<>",6);
    return retval;
}


NOINLINE std::map<std::string, int> make_map_inializer_list ()
{
    std::map<std::string, int> retval = {
        {"<", 1},
        {"<=",2},
        {">", 3},
        {">=",4},
        {"=", 5},
        {"<>",6}
    };
    return retval;
}


int main()
{
    {
        std::map<std::string, int> op = make_map_with_boost_yap();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    {
        std::map<std::string, int> op = make_map_with_boost_assign();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    {
        std::map<std::string, int> op = make_map_manually();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    {
        std::map<std::string, int> op = make_map_inializer_list();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    return 0;
}
