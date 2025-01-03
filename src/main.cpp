#include <iostream>
#include "algebra.h"

int main()
{
    // TODO: add these tests in test_algebra
    std::cout << 𝔽₄{1}/α << '\n';
    Pol<𝔽₄, 2> y {{1}};
    std::cout << "y: " << y << '\n';
    std::cout << Pol<𝔽₄, 2>{{α}} + 𝑥<𝔽₄,2> + 𝑥<𝔽₄,2>*𝑥<𝔽₄,2> << '\n';
    std::cout << 𝑥<𝔽₄,1>(α) << '\n';
    std::cout << Pol<𝔽₄, 2>{{1, α+1, α}}(α+1) << '\n';
    std::cout << lagrange<𝔽₄, 1>(std::array<𝔽₄, 2>{{1, α}},
                                 std::array<𝔽₄, 2>{{1, α}})
              << '\n';
    std::cout << lagrange<𝔽₄, 1>(std::array<𝔽₄, 2>{{1, α}},
                                 std::array<𝔽₄, 2>{{α, 1}})(1);

    return 0;
}
