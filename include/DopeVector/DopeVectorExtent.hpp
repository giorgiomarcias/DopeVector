// Copyright (c) 2016 Giorgio Marcias
//
// This source code is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com

#ifndef DopeVectorExtent_hpp
#define DopeVectorExtent_hpp

#include <DopeVector/DopeVector.hpp>
#include <memory>

namespace dp {

	/// The DopeVectorExtent class is a wrapper of DopeVector providing a
	/// built-in memory storage and management.
	template < typename T, std::size_t D >
	class DopeVectorExtent : public DopeVector<T, D> {
	public:

		// constructors and assignment /////////////////////////////////////////

		/**
		 *    @brief Default constructor.
		 */
		inline DopeVectorExtent() = default;

		/**
		 *    @brief Initializer contructor.
		 *    @param size               Sizes of the D-dimensional matrix.
		 */
		inline explicit DopeVectorExtent(const std::array<std::size_t, D> &size);

		/**
		 *    @brief Copy constructor.
		 */
		DopeVectorExtent(const DopeVectorExtent<T, D> &other) = default;

		/**
		 *    @brief Move constructor.
		 */
		DopeVectorExtent(DopeVectorExtent &&other) = default;

		/**
		 *    @brief Copy assignment operator.
		 */
		inline DopeVectorExtent & operator=(const DopeVectorExtent &other);

		/**
		 *    @brief Move assignment operator.
		 */
		DopeVectorExtent & operator=(DopeVectorExtent &&other) = default;

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
		inline void import(const DopeVector<T, D> &o);

		////////////////////////////////////////////////////////////////////////



		// matrix modifiers ////////////////////////////////////////////////////

		/**
		 *    @brief Changes the sizes of this matrix.
		 *    @param size               The new sizes.
		 */
		inline void resize(const std::array<std::size_t, D> &size);

		/**
		 *    @brief Empties this array, making its size 0.
		 */
		inline void clear();

		////////////////////////////////////////////////////////////////////////



	private:
		std::unique_ptr<T[]>    _arrayPtr;              ///< Sharp pointer in memory to this matrix, with automatic storage (de)allocation.
	};

}

#include <DopeVector/inlines/DopeVectorExtent.inl>

#endif /* DopeVectorExtent_hpp */
