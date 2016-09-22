// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include <DopeVector/DopeVector.hpp>
#include <DopeVector/Grid.hpp>

using namespace dope;

int main(int argc, char *argv[])
{
	Index2 size(10, 10);
	Grid<std::size_t, 2> grid2D(size, std::size_t(0));
	for (std::size_t i = 0; i < size[0]; ++i) {
		DopeVector<std::size_t, 1> grid_row = grid2D[i];
		for (std::size_t j = 0; j < size[1]; ++j)
			grid_row[j] = i * size[0] + j;
	}

	std::cout << "Initial Grid values:\n";
	for (std::size_t i = 0; i < size[0]; ++i) {
		DopeVector<std::size_t, 1> grid_row = grid2D[i];
		for (std::size_t j = 0; j < size[1]; ++j)
			std::cout << grid_row[j] << '\t';
		std::cout << '\n';
	}

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	Grid<std::size_t, 2> grid2_naive_transposition(size);
	for (std::size_t i = 0; i < size[0]; ++i)
		for (std::size_t j = 0; j < size[1]; ++j)
			grid2_naive_transposition[j][i] = grid2D[i][j];

	std::cout << "\nNaive transposition in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() << " ns." << std::endl;

	std::cout << "Transposition (naive):\n";
	for (std::size_t i = 0; i < size[0]; ++i) {
		auto grid_row = grid2_naive_transposition[i];
		for (std::size_t j = 0; j < size[1]; ++j)
			std::cout << grid_row[j] << '\t';
		std::cout << '\n';
	}



	Index2 order = {{1, 0}};

	start = std::chrono::steady_clock::now();

	auto grid2D_permuted = grid2D.permute(order);

	std::cout << "\nDope Transposition in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() << " ns." << std::endl;

	for (std::size_t i = 0; i < size[0]; ++i) {
		auto grid_row = grid2D_permuted[i];
		for (std::size_t j = 0; j < size[1]; ++j)
			std::cout << grid_row[j] << '\t';
		std::cout << '\n';
	}



	Grid<std::size_t, 10> big_grid(Index<10>(10));
	Index<10> new_order(1, 0, 2, 5, 4, 7, 3, 6, 9, 8);

	start = std::chrono::steady_clock::now();

	big_grid.permute(new_order);

	std::cout << "\nBig grid (10 side length X 10 dimensions) permuted in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() << " ns." << std::endl;

	return 0;
}
