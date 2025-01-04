/*  Testing file is long and comprehensive because
    an error of implementation potentially means
    losing all your bitcoins.  */


#include "algebra.h"
#include <gtest/gtest.h>
#include <functional>
#include <type_traits>


#ifdef NDEBUG
    #define EXPECT_REJECTION(statement, exception) \
        EXPECT_THROW(statement, exception)
#else
    #define EXPECT_REJECTION(statement, exception) \
        EXPECT_DEATH(statement, ".*")
#endif


// maximum size of a group
using group_size_t = std::uint_fast32_t;


namespace cst
{
    constexpr deg_t max_degree_tested {3};
}




// Test cases for 𝔽₄

static constexpr auto 𝔽₄_elts {std::array{𝔽₄{0}, 𝔽₄{1}, α, α+1}};
static constexpr auto 𝔽₄_star {std::array{𝔽₄{1}, α, α+1}};

TEST(F4_Test, Constructor_Rejections)
{
    EXPECT_NO_THROW(𝔽₄{0});
    EXPECT_NO_THROW(𝔽₄{3});
    EXPECT_REJECTION(𝔽₄{4}, std::out_of_range);
    EXPECT_REJECTION(
        𝔽₄{std::numeric_limits<val_t>::max()},
        std::out_of_range
    );
}


/*  Verifiying field axioms is sufficient to verify
    that 𝔽₄ is correctly implemented,
    as all fields of cardinal 4 are isomorphic.  */

TEST(F4_Test, Has_Four_Elements)
{
    EXPECT_EQ(𝔽₄_elts.size(), 4UZ);
    for (val_t i {}; i < 𝔽₄_elts.size(); ++i)
        for (val_t j {}; j < i; ++j)
            EXPECT_NE(𝔽₄_elts[i], 𝔽₄_elts[j]);
}

template <typename G, group_size_t N,
          typename Op, typename Inv_Op>
void is_abelian_group(const std::array<G, N>& group_elts, G neut)
{
    Op op{};
    Inv_Op inv{};
    for (const auto& elt : group_elts)
    {
        EXPECT_EQ(op(elt, neut), elt);              // neutrality
        EXPECT_EQ(op(elt, inv(neut, elt)), neut);   // inverse
        for (const auto& elt2 : group_elts)
        {
            EXPECT_EQ(op(elt,elt2), op(elt2,elt));  // commutativity
            for (const auto& elt3 : group_elts)
                EXPECT_EQ(op(op(elt,elt2),elt3),
                          op(elt,op(elt2,elt3)));   // associativity
        }
    }
}

template <typename k, group_size_t N>
void distributivity(const std::array<k, N>& field_elts)
{
    for (const auto& elt : field_elts)
        for (const auto& elt2 : field_elts)
            for (const auto& elt3 : field_elts)
                EXPECT_EQ(elt * (elt2+elt3),
                          elt*elt2 + elt*elt3);
}

TEST(F4_Test, Is_Field)
{
    is_abelian_group
        <𝔽₄, 4, std::plus<𝔽₄>, std::minus<𝔽₄>>
        (𝔽₄_elts, 𝔽₄{0});
    is_abelian_group
        <𝔽₄, 3, std::multiplies<𝔽₄>, std::divides<𝔽₄>>
        (𝔽₄_star, 𝔽₄{1});
    distributivity<𝔽₄, 4>(𝔽₄_elts);
}


//  Defining names for operator+= and the like

#define TEST_ALGEBRA__DEFINE_OPERATOR_WRAPPER(name, op)            \
    struct name                                                    \
    {                                                              \
        template <typename T>                                      \
        constexpr T& operator()(T& lhs, const T& rhs)              \
        {                                                          \
            return lhs op rhs;                                     \
        }                                                          \
    }

TEST_ALGEBRA__DEFINE_OPERATOR_WRAPPER(plus_equal, +=);
TEST_ALGEBRA__DEFINE_OPERATOR_WRAPPER(minus_equal, -=);
TEST_ALGEBRA__DEFINE_OPERATOR_WRAPPER(multiplies_equal, *=);
TEST_ALGEBRA__DEFINE_OPERATOR_WRAPPER(divides_equal, /=);

#undef TEST_ALGEBRA__DEFINE_OPERATOR_WRAPPER


template <typename k, group_size_t L, group_size_t R,
          typename Op, typename Op_Eq>
void op_equal_concordance(const std::array<k, L>& left_elts,
                          const std::array<k, R>& right_elts)
{
    Op op{};
    Op_Eq op_eq{};
    for (auto elt : left_elts)
    {   
        for (const auto& elt2 : right_elts)
        {
            k result{op(elt, elt2)};
            op_eq(elt, elt2);
            EXPECT_EQ(result, elt);
        }
    }
}

template <typename G, group_size_t L, group_size_t R,
          typename Op, typename Inv_Op>
void inv_op_concordance(const std::array<G, L>& left_elts,
                        const std::array<G, R>& right_elts,
                        G neut)
{
    Op op{};
    Inv_Op inv{};
    for (const auto& elt : left_elts)
        for (const auto& elt2 : right_elts)
            EXPECT_EQ(inv(elt, elt2),
                      op(elt, inv(neut, elt2)));
}

template <typename G, group_size_t N,
          typename Op, typename Un>
void unary_op_concordance(std::array<G, N> group_elts, G neut)
{
    Op op{};
    Un un{};
    for (const auto& elt : group_elts)
        EXPECT_EQ(un(elt), op(neut, elt));
}


TEST(F4_Test, Operators_Concordance)
{
    op_equal_concordance
        <𝔽₄, 4, 4, std::plus<𝔽₄>, plus_equal>
        (𝔽₄_elts, 𝔽₄_elts);
    op_equal_concordance
        <𝔽₄, 4, 4, std::minus<𝔽₄>, minus_equal>
        (𝔽₄_elts, 𝔽₄_elts);
    op_equal_concordance
        <𝔽₄, 4, 4, std::multiplies<𝔽₄>, multiplies_equal>
        (𝔽₄_elts, 𝔽₄_elts);
    op_equal_concordance
        <𝔽₄, 4, 3, std::divides<𝔽₄>, divides_equal>
        (𝔽₄_elts, 𝔽₄_star);

    inv_op_concordance
        <𝔽₄, 4, 4, std::plus<𝔽₄>, std::minus<𝔽₄>>
        (𝔽₄_elts, 𝔽₄_elts, 𝔽₄{0});
    inv_op_concordance
        <𝔽₄, 4, 3, std::multiplies<𝔽₄>, std::divides<𝔽₄>>
        (𝔽₄_elts, 𝔽₄_star, 𝔽₄{1});

    unary_op_concordance
        <𝔽₄, 4, std::minus<𝔽₄>, std::negate<𝔽₄>>
        (𝔽₄_elts, 𝔽₄{0});
}


TEST(F4_Test, Division_By_Zero)
{
    for (const auto& elt : 𝔽₄_elts)
        EXPECT_REJECTION(elt/𝔽₄{0}, std::invalid_argument);
}




// Test cases for Pol

constexpr group_size_t pow(group_size_t N, deg_t d)
{
    if (d==0) return 1;
    switch (d%2)
    {
    case 0: return pow(N*N, d/2);
    case 1: return N*pow(N, d-1);
    }
}

TEST(PolTest, Power_Function)
{
    EXPECT_EQ(pow(static_cast<group_size_t>(6),
                  static_cast<deg_t>(5)),
              static_cast<group_size_t>(6*6*6*6*6));

    EXPECT_EQ(pow(static_cast<group_size_t>(6),
                  static_cast<deg_t>(0)),
              static_cast<group_size_t>(1));
              
    EXPECT_EQ(pow(static_cast<group_size_t>(0),
                  static_cast<deg_t>(5)),
              static_cast<group_size_t>(0));

    EXPECT_EQ(pow(static_cast<group_size_t>(0),
                  static_cast<deg_t>(0)),
              static_cast<group_size_t>(1));
}

template <typename k, group_size_t N, deg_t d>
constexpr auto generate_pol_elts()
{
    constexpr auto nb_pol {pow(N, d+1)};
    std::array<Pol<k, d>, nb_pol> pol_elts {};
    Pol<k, d> current_pol {}; 

    for (group_size_t i {0}; i < nb_pol; ++i)
        pol_elts[i] = ++current_pol;
    return pol_elts;
} 

template <typename k, group_size_t N, deg_t d>
static constexpr std::array<Pol<k, d>, pow(N,d+1)>
    Pol_elts {generate_pol_elts<k, N, d>()};

// TODO: write tests on Pol_elts

template <typename k, group_size_t N, deg_t d>
void pol_elts_correctly_generated()
{
    EXPECT_EQ((Pol_elts<k, N, d>.size()), pow(N,d+1));
    for (group_size_t i {}; i < Pol_elts<k, N, d>.size(); ++i)
        for (group_size_t j {}; j < i; ++j)
            EXPECT_NE((Pol_elts<k, N, d>[i]),
                      (Pol_elts<k, N, d>[j]));
}

template <typename k, group_size_t N, deg_t... Is>
void pol_elts_all_correctly_generated(
    std::integer_sequence<deg_t, Is...>)
{
    (pol_elts_correctly_generated<k, N, Is>(), ...);
}

TEST(PolTest, Pol_Elts_Correctly_Generated)
{
    pol_elts_all_correctly_generated<𝔽₄, 4>(
        std::make_integer_sequence<deg_t,
        cst::max_degree_tested+1>{}
    );
}

// TEST(Has_Two_Pow_N_Elements)
// {

// }


// TODO: rewrite the two following tests

TEST(PolTest, Constructor)
{
    Pol<𝔽₄, 2> p {{𝔽₄{1}, α, α+1}};
    EXPECT_EQ(p.deg(), 2);
}

TEST(PolTest, Addition)
{
    Pol<𝔽₄, 1> p1 {{𝔽₄{1}, α}};
    Pol<𝔽₄, 1> p2 {{𝔽₄{0}, 𝔽₄{1}}};
    Pol<𝔽₄, 1> expected {{𝔽₄{1}, α+1}};
    EXPECT_EQ(p1 + p2, expected);
}


template <typename k, group_size_t N, deg_t d>
void pol_is_abelian_group()
{
    is_abelian_group <Pol<k, d>, pow(N, d+1),
        std::plus<Pol<k, d>>, std::minus<Pol<k, d>>>
        (Pol_elts<k, N, d>, Pol<k, d>{});
}

template <typename k, group_size_t N, deg_t... Is>
void pol_are_abelian_groups(
    std::integer_sequence<deg_t, Is...>)
{
    (pol_is_abelian_group<k, N, Is>(), ...);
}

TEST(PolTest, Are_Groups)
{
    pol_are_abelian_groups<𝔽₄, 4>(
        std::make_integer_sequence<deg_t,
        cst::max_degree_tested+1>{}
    );
}


// TODO: write tests for multiplication,
// polynomial eval, lagrange, ...




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
