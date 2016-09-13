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

using namespace container;

int main(int argc, char *argv[])
{
	std::array<std::size_t, 2> size({{10, 10}});
    DopeVectorExtent<std::size_t, 2> f(size);
    for (std::size_t i = 0; i < size[0]; ++i)
        for (std::size_t j = 0; j < size[1]; ++j)
            f[i][j] = i * size[0] + j;

	for (std::size_t i = 0; i < size[0]; ++i) {
		DopeVector<std::size_t, 1> f_row = f[i];
        for (std::size_t j = 0; j < size[1]; ++j)
			std::cout << f_row[j] << '\t';
		std::cout << '\n';
	}

	std::array<std::size_t, 2> order = {{1, 0}};

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	auto f_permuted = f.permute(order);

    std::cout << "\nTransposed in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() << " ns." << std::endl;

	for (std::size_t i = 0; i < size[0]; ++i) {
		auto f_row = f_permuted[i];
		for (std::size_t j = 0; j < size[1]; ++j)
			std::cout << f_row[j] << '\t';
		std::cout << '\n';
	}

	DopeVectorExtent<std::size_t, 10> f_big({{10}});
	std::array<std::size_t, 10> new_order = {{1, 0, 2, 5, 4, 7, 3, 6, 9, 8}};

	start = std::chrono::steady_clock::now();

	f_big.permute(new_order);

	std::cout << "\nBig f (10^10) permuted in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() << " ns." << std::endl;

    return 0;
}
