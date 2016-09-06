// Copyright (c) 2016 Giorgio Marcias
//
// This source code is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include <DopeVector/DopeVectors.hpp>

using namespace dp;

int main(int argc, char *argv[])
{
	std::array<std::size_t, 2> size({{10, 10}});
    DopeVectorExtent<float, 2> f(size);
    for (std::size_t i = 0; i < size[0]; ++i)
        for (std::size_t j = 0; j < size[1]; ++j) {
            if (i == j)
                f[i][j] = 0.0f;
            else
                f[i][j] = std::numeric_limits<float>::max();
        }

	std::array<std::size_t, 2> order = {{1, 0}};
	auto f_permuted = f.permute(order);

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << " ms." << std::endl;

    std::cout << std::endl;

    return 0;
}
