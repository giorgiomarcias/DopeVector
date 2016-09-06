// Copyright (c) 2016 Giorgio Marcias
//
// This source code is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com

#ifndef DopeVector_hpp
#define DopeVector_hpp

#include <array>
#include <sstream>
#include <stdexcept>
#include <cstring>

namespace dp {

	/// The DopeVector class represents a D-dimensional dope vector
	/// (https://en.wikipedia.org/wiki/Dope_vector) of scalar type T. Given an
	/// array stored sequentially in memory, this class wraps it provinding a
	/// multi-dimensional matrix interface. It is possible to take slices,
	/// windows or even permutations without actually modifying the underlying
	/// memory, no element hence gets moved.
	template < typename T, std::size_t D >
	class DopeVector {
	public:
		/**
		 *    @brief Default constructor.
		 */
		inline DopeVector();

		/**
		 *    @brief Initializer contructor.
		 *
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the D-dimensional matrix.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, D> &size);

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the D-dimensional matrix.
		 *    @param offset             Offsets in each dimension, i.e. jumps in
		 *                              memory.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, D> &size, const std::array<std::size_t, D> &offset);

		/**
		 *    @brief Copy constructor.
		 */
		DopeVector(const DopeVector &other) = default;

		/**
		 *    @brief Move constructor.
		 */
		DopeVector(DopeVector &&other) = default;

		/**
		 *    @brief Copy assignment operator.
		 */
		DopeVector & operator=(const DopeVector &other) = default;

		/**
		 *    @brief Move assignment operator.
		 */
		DopeVector & operator=(DopeVector &&other) = default;

		/**
		 *    @brief Gives access to the i-th sub-matrix in the first dimension,
		 *           i.e. m[i][*]...[*].
		 *    @param i                  The i-th "row" of this matrix.
		 *    @param s                  The output sub-matrix at i.
		 */
		inline void at(const std::size_t i, DopeVector<T, D-1> &s) const;

		/**
		 *    @brief Gives access to the i-th sub-matrix in the first dimension,
		 *           i.e. m[i][*]...[*].
		 *    @param i                  The i-th "row" of this matrix.
		 *    @return The output sub-matrix at i.
		 */
		inline DopeVector<T, D-1> operator[](const std::size_t i) const;

		/**
		 *    @brief Gives access to the i-th sub-matrix in the d-th dimension,
		 *           i.e. m[*]...[i]...[*].
		 *    @param d                  The dimension where to slice.
		 *    @param i                  The i-th "row" of this matrix.
		 *    @param s                  The output sub-matrix at i in the d
		 *                              dimension.
		 */
		inline void slice(const std::size_t d, const std::size_t i, DopeVector<T, D-1> &s) const;

		/**
		 *    @brief Gives access to the i-th sub-matrix in the d-th dimension,
		 *           i.e. m[*]...[i]...[*].
		 *    @param d                  The dimension where to slice.
		 *    @param i                  The i-th "row" of this matrix.
		 *    @return The output sub-matrix at i in the d dimension.
		 */
		inline DopeVector<T, D-1> slice(const std::size_t d, const std::size_t i) const;

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < D goes 
		 *           to 0 <= order[i] < D, i.e. m[*]..[*]_i...[*] swaps with
		 *           m[*]...[i]...[*]_order[i]...[*].
		 *    @param order              A permutation of the matrix indices.
		 *    @param p                  The output permuted matrix.
		 *    @note Example: transpose a 2D matrix by swapping the access
		 *                   indices:
		 *                   MArray<T,2> m, mt;
		 *                   std::size_t trans_ord[2] = {1, 0}; 
		 *                   m.permute(trans_ord, mt);
		 */
		inline void permute(const std::array<std::size_t, D> &order, DopeVector<T, D> &p) const;

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < D goes to 0 <= order[i] < D, i.e. m[*]..[*]_i...[*] swaps with m[*]...[i]...[*]_order[i]...[*].
		 *    @param order              A permutation of the matrix indices.
		 *    @return The output permuted matrix.
		 *    @note Example: transpose a 2D matrix by swapping the access indices - MArray<T,2> m, mt; std::size_t trans_ord[2] = {1, 0}; mt = m.permute(trans_ord);
		 */
		inline DopeVector<T, D> permute(const std::array<std::size_t, D> &order) const;

		/**
		 *    @brief Extracts a D-dimensional window from this matrix.
		 *    @param start              The initial offset of the window in each dimension.
		 *    @param size               The sizes of the window.
		 *    @param w                  The output sub-matrix.
		 */
		inline void window(const std::array<std::size_t, D> &start, std::array<std::size_t, D> &size, DopeVector<T, D> &w) const;

		/**
		 *    @brief Extracts a D-dimensional window from this matrix.
		 *    @param start              The initial offset of the window in each dimension.
		 *    @param size               The sizes of the window.
		 *    @return The output sub-matrix.
		 */
		inline DopeVector<T, D> window(const std::array<std::size_t, D> &start, const std::array<std::size_t, D> &size) const;

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
		virtual inline void import(const DopeVector<T, D> &o);

		/**
		 *    @brief Gives the size of this matrix in the d dimension.
		 *    @param d                  The dimension whose size is requested.
		 *    @return The size of this matrix at dimension d.
		 */
		inline std::size_t sizeAt(const std::size_t d) const;

		/**
		 *    @brief Gives the sizes of this matrix.
		 *    @param s                  The output array that will contain the sizes of this matrix.
		 */
		inline void allSizes(std::array<std::size_t, D> &s) const;

		/**
		 *    @brief Gives the total size (number of elements in memory) of this matrix.
		 *    @return The total number of elements in memory.
		 */
		inline std::size_t size() const;

		/**
		 *    @brief Gives the total offset, from the beginning of the stored array, of the i-th element at dimension d.
		 *    @param i                  The element whose offset is requested.
		 *    @param d                  The dimension whose i-th element offset is requested.
		 *    @return The total offset from the beggining of the stored array of the i-th element at dimension d.
		 */
		inline std::size_t accumulatedOffset(const std::size_t i, const std::size_t d = 0) const;

	private:
		friend class DopeVector<T, D+1>;
		T                          *_array;                 ///< Pointer in memory to the first element of this matrix.
		std::size_t                 _accumulatedOffset;     ///< Offset of the first element of this matrix from the beginning of the stored array.
		std::array<std::size_t, D>  _size;                  ///< Sizes of this matrix, for each dimension.
		std::array<std::size_t, D>  _offset;                ///< Jumps' offsets from the beginning of a "row" to the beginning of the next one, for each dimension.
	};



	/// The DopeVector class represents a 1-dimensional dope vector (https://en.wikipedia.org/wiki/Dope_vector) of scalar type T.
	/// Given an array stored sequentially in memory, this class wraps it provinding a mono-dimensional matrix interface.
	/// It is possible to take slices, windows or even permutations without actually modifying the underlying memory, no element
	/// hence moved.
	/// This actually is the basis of the recursive class MArray<T, D> above.
	template < typename T >
	class DopeVector<T, 1> {
	public:
		/**
		 *    @brief Default constructor.
		 */
		inline DopeVector();

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Size of the 1-dimensional matrix.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::size_t size);

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Size of the 1-dimensional matrix.
		 *    @param offset             Offset in memory from one element to the next one.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::size_t size, const std::size_t offset);

		/**
		 *    @brief Initializer contructor.
		 *
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the 1-dimensional matrix.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, 1> &size);

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the 1-dimensional matrix.
		 *    @param offset             Offsets in each dimension, i.e. jumps in
		 *                              memory.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, 1> &size, const std::array<std::size_t, 1> &offset);

		/**
		 *    @brief Copy constructor.
		 */
		DopeVector(const DopeVector &other) = default;

		/**
		 *    @brief Move constructor.
		 */
		DopeVector(DopeVector &&other) = default;

		/**
		 *    @brief Copy assignment operator.
		 */
		DopeVector & operator=(const DopeVector &other) = default;

		/**
		 *    @brief Move assignment operator.
		 */
		DopeVector & operator=(DopeVector &&other) = default;

		/**
		 *    @brief Gives constant access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector
		 *    @return The output element at i.
		 */
		inline const T & operator[](const std::size_t i) const;

		/**
		 *    @brief Gives access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector
		 *    @return The output element at i.
		 */
		inline T & operator[](const std::size_t i);

		/**
		 *    @brief Gives constant access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector.
		 *    @return The output element at i.
		 */
		inline const T & slice(const std::size_t i) const;

		/**
		 *    @brief Gives access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector.
		 *    @return The output element at i.
		 */
		inline T & slice(const std::size_t i);

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < 1 goes
		 *           to 0 <= order[i] < 1, i.e. m[0] swaps with m[order[0]].
		 *    @param order              A permutation of the matrix indices.
		 *    @param p                  The output permuted matrix.
		 *    @note In practice, this does nothing. It is kept for coherency
		 *          with higher dimensional MArray.
		 */
		inline void permute(const std::array<std::size_t, 1> &order, DopeVector<T, 1> &p) const;

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < 1 goes to 0 <= order[i] < 1, i.e. m[0] swaps with m[order[0]].
		 *    @param order              A permutation of the matrix indices.
		 *    @return The output permuted matrix.
		 *    @note In practice, this does nothing. It is kept for coherency with higher dimensional MArray.
		 */
		inline DopeVector<T, 1> permute(const std::array<std::size_t, 1> &order) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @param w                  The output sub-vector.
		 */
		inline void window(const std::array<std::size_t, 1> &start, const std::array<std::size_t, 1> &size, DopeVector<T, 1> &w) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @return The output sub-vector.
		 */
		inline DopeVector<T, 1> window(const std::array<std::size_t, 1> &start, const std::array<std::size_t, 1> &size) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @param w                  The output sub-vector.
		 */
		inline void window(const std::size_t start, const std::size_t size, DopeVector<T, 1> &w) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @return The output sub-vector.
		 */
		inline DopeVector<T, 1> window(const std::size_t start, const std::size_t size) const;

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
		virtual inline void import(const DopeVector<T, 1> &o);

		/**
		 *    @brief Gives the size of this vector
		 *    @param d                  The dimension whose size is requested
		 *                              (MUST be 0).
		 *    @return The size of this matrix at dimension d.
		 */
		inline std::size_t sizeAt(const std::size_t d) const;

		/**
		 *    @brief Gives the size of this vector.
		 *    @param s                  The output size of this vector.
		 */
		inline void allSizes(std::array<std::size_t, 1> &s) const;

		/**
		 *    @brief Gives the total size (number of elements in memory) of this
		 *           vector.
		 *    @return The total number of elements in memory.
		 */
		inline std::size_t size() const;

		/**
		 *    @brief Gives the total offset, from the beginning of the stored
		 *           array, of the i-th element.
		 *    @param i                  The element whose offset is requested.
		 *    @return The total offset from the beggining of the stored array of
		 *            the i-th element.
		 */
		inline std::size_t accumulatedOffset(const std::size_t i = 0) const;

	private:
		friend class DopeVector<T, 2>;
		T                          *_array;                 ///< Pointer in memory to the first element of this vector.
		std::size_t                 _accumulatedOffset;     ///< Offset of the first element of this vector from the beginning of the stored array.
		std::array<std::size_t, 1>  _size;                  ///< Sizes of this matrix, for each dimension.
		std::array<std::size_t, 1>  _offset;                ///< Jumps' offsets from the beginning of a "row" to the beginning of the next one, for each dimension.
	};

#include <DopeVector/inlines/DopeVector.inl>

}

#endif /* DopeVector_hpp */
