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
		inline DopeVector()
			: _array(nullptr)
			, _accumulatedOffset(0)
			, _size({{0}})
			, _offset({{0}})
		{ }

		/**
		 *    @brief Initializer contructor.
		 *
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the D-dimensional matrix.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, D> &size)
			: _array(const_cast<T*>(array))
			, _accumulatedOffset(accumulatedOffset)
			, _size(size)
			, _offset({{0}})
		{ }

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the D-dimensional matrix.
		 *    @param offset             Offsets in each dimension, i.e. jumps in
		 *                              memory.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, D> &size, const std::array<std::size_t, D> &offset)
			: _array(const_cast<T*>(array))
			, _accumulatedOffset(accumulatedOffset)
			, _size(size)
			, _offset(offset)
		{ }

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
		inline void at(const std::size_t i, DopeVector<T, D-1> &s) const
		{
			if (i >= _size[0]) {
				std::stringstream stream;
				stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
				throw std::out_of_range(stream.str());
			}
			s._array = _array + _offset[0] * i;
			s._accumulatedOffset = accumulatedOffset(i);
			for (std::size_t j = 1; j < D; ++j) {
				s._size[j-1] = _size[j];
				s._offset[j-1] = _offset[j];
			}
		}

		/**
		 *    @brief Gives access to the i-th sub-matrix in the first dimension,
		 *           i.e. m[i][*]...[*].
		 *    @param i                  The i-th "row" of this matrix.
		 *    @return The output sub-matrix at i.
		 */
		inline DopeVector<T, D-1> operator[](const std::size_t i) const
		{
			DopeVector<T, D-1> s;
			at(i, s);
			return s;
		}

		/**
		 *    @brief Gives access to the i-th sub-matrix in the d-th dimension,
		 *           i.e. m[*]...[i]...[*].
		 *    @param d                  The dimension where to slice.
		 *    @param i                  The i-th "row" of this matrix.
		 *    @param s                  The output sub-matrix at i in the d
		 *                              dimension.
		 */
		inline void slice(const std::size_t d, const std::size_t i, DopeVector<T, D-1> &s) const
		{
			if (d >= D) {
				std::stringstream stream;
				stream << "Index " << d << " is out of range [0, " << D-1 << ']';
				throw std::out_of_range(stream.str());
			}
			if (i >= _size[d]) {
				std::stringstream stream;
				stream << "Index " << i << " is out of range [0, " << _size[d]-1 << ']';
				throw std::out_of_range(stream.str());
			}
			s._array = _array + _offset[d] * i;
			s._accumulatedOffset = accumulatedOffset(i, d);
			std::size_t k = 0;
			for (std::size_t j = 0; j < d; ++j, ++k) {
				s._size[k] = _size[j];
				s._offset[k] = _offset[j];
			}
			for (std::size_t j = d+1; j < D; ++j, ++k) {
				s._size[k] = _size[j];
				s._offset[k] = _offset[j];
			}
		}

		/**
		 *    @brief Gives access to the i-th sub-matrix in the d-th dimension,
		 *           i.e. m[*]...[i]...[*].
		 *    @param d                  The dimension where to slice.
		 *    @param i                  The i-th "row" of this matrix.
		 *    @return The output sub-matrix at i in the d dimension.
		 */
		inline DopeVector<T, D-1> slice(const std::size_t d, const std::size_t i) const
		{
			DopeVector<T, D-1> s;
			slice(d, i, s);
			return s;
		}

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
		inline void permute(const std::array<std::size_t, D> &order, DopeVector<T, D> &p) const
		{
			std::array<bool, D> included = {{false}};
			for (std::size_t d = 0; d < D; ++d) {
				if (order[d] >= D) {
					std::stringstream stream;
					stream << "Index " << order[d] << " is out of range [0, " << D-1 << ']';
					throw std::out_of_range(stream.str());
				}
				if (included[order[d]]) {
					std::stringstream stream;
					stream << "Dimension " << order[d] << " duplicated.";
					throw std::invalid_argument(stream.str());
				}
				included[order[d]] = true;
			}
			p._array = _array;
			p._accumulatedOffset = _accumulatedOffset;
			std::size_t newSize[D], newOffset[D];
			std::memcpy(newSize, _size.data(), D * sizeof(std::size_t));
			std::memcpy(newOffset, _offset.data(), D * sizeof(std::size_t));
			for (std::size_t d = 0; d < D; ++d) {
				p._size[d] = newSize[order[d]];
				p._offset[d] = newOffset[order[d]];
			}
		}

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < D goes to 0 <= order[i] < D, i.e. m[*]..[*]_i...[*] swaps with m[*]...[i]...[*]_order[i]...[*].
		 *    @param order              A permutation of the matrix indices.
		 *    @return The output permuted matrix.
		 *    @note Example: transpose a 2D matrix by swapping the access indices - MArray<T,2> m, mt; std::size_t trans_ord[2] = {1, 0}; mt = m.permute(trans_ord);
		 */
		inline DopeVector<T, D> permute(const std::array<std::size_t, D> &order) const
		{
			DopeVector<T, D> p;
			permute(order, p);
			return p;
		}

		/**
		 *    @brief Extracts a D-dimensional window from this matrix.
		 *    @param start              The initial offset of the window in each dimension.
		 *    @param size               The sizes of the window.
		 *    @param w                  The output sub-matrix.
		 */
		inline void window(const std::array<std::size_t, D> &start, std::array<std::size_t, D> &size, DopeVector<T, D> &w) const
		{
			for (std::size_t d = 0; d < D; ++d) {
				if (start[d] >= _size[d]) {
					std::stringstream stream;
					stream << "Index " << start[d] << " is out of range [0, " << _size[d] << ']';
					throw std::out_of_range(stream.str());
				}
				if (start[d] + size[d] > _size[d]) {
					std::stringstream stream;
					stream << "Window size " << size[d] << " is out of range [" << 0 << ", " << _size[d] - start[d] << ']';
					throw std::out_of_range(stream.str());
				}
			}
			std::size_t newAccumulatedOffset = _accumulatedOffset;
			for (std::size_t d = 0; d < D; ++d)
				newAccumulatedOffset += _offset[d] * start[d];
			w._array = _array + (newAccumulatedOffset - _accumulatedOffset);
			w._accumulatedOffset = newAccumulatedOffset;
			w._size = size;
			w._offset = _offset;
		}

		/**
		 *    @brief Extracts a D-dimensional window from this matrix.
		 *    @param start              The initial offset of the window in each dimension.
		 *    @param size               The sizes of the window.
		 *    @return The output sub-matrix.
		 */
		inline DopeVector<T, D> window(const std::array<std::size_t, D> &start, const std::array<std::size_t, D> &size) const
		{
			DopeVector<T, D> w;
			window(start, size, w);
			return w;
		}

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
		virtual inline void import(const DopeVector<T, D> &o)
		{
			if (&o == this)
				return;
			if (_size[0] != o._size[0])
				throw std::out_of_range("Matrixes do not have same size.");
			for (std::size_t i = 0; i < _size[0]; ++i)
				operator[](i).import(o[i]);
		}

		/**
		 *    @brief Gives the size of this matrix in the d dimension.
		 *    @param d                  The dimension whose size is requested.
		 *    @return The size of this matrix at dimension d.
		 */
		inline std::size_t sizeAt(const std::size_t d) const
		{
			if (d >= D) {
				std::stringstream stream;
				stream << "Index " << d << " is out of range [0, " << D-1 << ']';
				throw std::out_of_range(stream.str());
			}
			return _size[d];
		}

		/**
		 *    @brief Gives the sizes of this matrix.
		 *    @param s                  The output array that will contain the sizes of this matrix.
		 */
		inline void allSizes(std::array<std::size_t, D> &s) const
		{
			s = _size;
		}

		/**
		 *    @brief Gives the total size (number of elements in memory) of this matrix.
		 *    @return The total number of elements in memory.
		 */
		inline std::size_t size() const
		{
			std::size_t total = _size[0];
			for (std::size_t i = 1; i < D; ++i)
				total *= _size[i];
			return total;
		}

		/**
		 *    @brief Gives the total offset, from the beginning of the stored array, of the i-th element at dimension d.
		 *    @param i                  The element whose offset is requested.
		 *    @param d                  The dimension whose i-th element offset is requested.
		 *    @return The total offset from the beggining of the stored array of the i-th element at dimension d.
		 */
		inline std::size_t accumulatedOffset(const std::size_t i, const std::size_t d = 0) const
		{
			if (d >= D) {
				std::stringstream stream;
				stream << "Index " << d << " is out of range [0, " << D-1 << ']';
				throw std::out_of_range(stream.str());
			}
			if (i >= _size[d]) {
				std::stringstream stream;
				stream << "Index " << i << " is out of range [0, " << _size[d]-1 << ']';
				throw std::out_of_range(stream.str());
			}
			return  _accumulatedOffset + _offset[d] * i;
		}

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
		inline DopeVector()
			: _array(nullptr)
			, _accumulatedOffset(0)
			, _size({{0}})
			, _offset({{0}})
		{ }

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Size of the 1-dimensional matrix.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::size_t size)
			: _array(const_cast<T*>(array))
			, _accumulatedOffset(accumulatedOffset)
			, _size({{size}})
			, _offset({{0}})
		{ }

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Size of the 1-dimensional matrix.
		 *    @param offset             Offset in memory from one element to the next one.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::size_t size, const std::size_t offset)
			: _array(const_cast<T*>(array))
			, _accumulatedOffset(accumulatedOffset)
			, _size({{size}})
			, _offset({{offset}})
		{ }

		/**
		 *    @brief Initializer contructor.
		 *
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the 1-dimensional matrix.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, 1> &size)
			: _array(const_cast<T*>(array))
			, _accumulatedOffset(accumulatedOffset)
			, _size(size)
			, _offset({{0}})
		{ }

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the 1-dimensional matrix.
		 *    @param offset             Offsets in each dimension, i.e. jumps in
		 *                              memory.
		 */
		inline DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, 1> &size, const std::array<std::size_t, 1> &offset)
			: _array(const_cast<T*>(array))
			, _accumulatedOffset(accumulatedOffset)
			, _size(size)
			, _offset(offset)
		{ }

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
		inline const T & operator[](const std::size_t i) const
		{
			if (i >= _size[0]) {
				std::stringstream stream;
				stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
				throw std::out_of_range(stream.str());
			}
			return *(_array + i * _offset[0]);
		}

		/**
		 *    @brief Gives access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector
		 *    @return The output element at i.
		 */
		inline T & operator[](const std::size_t i)
		{
			if (i >= _size[0]) {
				std::stringstream stream;
				stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
				throw std::out_of_range(stream.str());
			}
			return *(_array + i * _offset[0]);
		}

		/**
		 *    @brief Gives constant access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector.
		 *    @return The output element at i.
		 */
		inline const T & slice(const std::size_t i) const
		{
			return *this[i];
		}

		/**
		 *    @brief Gives access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector.
		 *    @return The output element at i.
		 */
		inline T & slice(const std::size_t i)
		{
			return *this[i];
		}

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < 1 goes
		 *           to 0 <= order[i] < 1, i.e. m[0] swaps with m[order[0]].
		 *    @param order              A permutation of the matrix indices.
		 *    @param p                  The output permuted matrix.
		 *    @note In practice, this does nothing. It is kept for coherency
		 *          with higher dimensional MArray.
		 */
		inline void permute(const std::array<std::size_t, 1> &order, DopeVector<T, 1> &p) const
		{
			for (std::size_t d = 0; d < 1; ++d) {
				if (order[d] >= 1) {
					std::stringstream stream;
					stream << "Index " << order[d] << " is out of range [0, 0]";
					throw std::out_of_range(stream.str());
				}
			}
			if (&p == this)
				return;
			p._array = _array;
			p._accumulatedOffset = _accumulatedOffset;
			std::size_t newSize[1] = {_size[0]};
			std::size_t newOffset[1] = {_offset[0]};
			for (std::size_t d = 0; d < 1; ++d) {
				p._size[d] = newSize[order[d]];
				p._offset[d] = newOffset[order[d]];
			}
		}

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < 1 goes to 0 <= order[i] < 1, i.e. m[0] swaps with m[order[0]].
		 *    @param order              A permutation of the matrix indices.
		 *    @return The output permuted matrix.
		 *    @note In practice, this does nothing. It is kept for coherency with higher dimensional MArray.
		 */
		inline DopeVector<T, 1> permute(const std::array<std::size_t, 1> &order) const
		{
			DopeVector<T, 1> p;
			permute(order, p);
			return p;
		}

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @param w                  The output sub-vector.
		 */
		inline void window(const std::array<std::size_t, 1> &start, const std::array<std::size_t, 1> &size, DopeVector<T, 1> &w) const
		{
			if (start[0] >= _size[0]) {
				std::stringstream stream;
				stream << "Index " << start[0] << " is out of range [0, " << _size[0] << ']';
				throw std::out_of_range(stream.str());
			}
			if (start[0] + size[0] > _size[0]) {
				std::stringstream stream;
				stream << "Window size " << size[0] << " is out of range [" << 0 << ", " << _size[0] - start[0] << ']';
				throw std::out_of_range(stream.str());
			}
			std::size_t newAccumulatedOffset = _accumulatedOffset + _offset[0] * start[0];
			w._array = _array + (newAccumulatedOffset - _accumulatedOffset);
			w._accumulatedOffset = newAccumulatedOffset;
			w._size[0] = size[0];
			w._offset[0] = _offset[0];
		}

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @return The output sub-vector.
		 */
		inline DopeVector<T, 1> window(const std::array<std::size_t, 1> &start, const std::array<std::size_t, 1> &size) const
		{
			DopeVector<T, 1> w;
			window(start, size, w);
			return w;
		}

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @param w                  The output sub-vector.
		 */
		inline void window(const std::size_t start, const std::size_t size, DopeVector<T, 1> &w) const
		{
			if (start >= _size[0]) {
				std::stringstream stream;
				stream << "Index " << start << " is out of range [0, " << _size[0] << ']';
				throw std::out_of_range(stream.str());
			}
			if (start + size > _size[0]) {
				std::stringstream stream;
				stream << "Window size " << size << " is out of range [" << 0 << ", " << _size[0] - start << ']';
				throw std::out_of_range(stream.str());
			}
			w._accumulatedOffset = _accumulatedOffset + _offset[0] * start;
			w._array = _array + (w._accumulatedOffset - _accumulatedOffset);
			w._size[0] = size;
			w._offset[0] = _offset[0];
		}

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @return The output sub-vector.
		 */
		inline DopeVector<T, 1> window(const std::size_t start, const std::size_t size) const
		{
			DopeVector<T, 1> w;
			window(start, size, w);
			return w;
		}

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
		virtual inline void import(const DopeVector<T, 1> &o)
		{
			if (&o == this)
				return;
			if (_size[0] != o._size[0])
				throw std::out_of_range("Matrixes do not have same size.");
			for (std::size_t i = 0; i < _size[0]; ++i)
				operator[](i) = o[i];
		}

		/**
		 *    @brief Gives the size of this vector
		 *    @param d                  The dimension whose size is requested
		 *                              (MUST be 0).
		 *    @return The size of this matrix at dimension d.
		 */
		inline std::size_t sizeAt(const std::size_t d) const
		{
			if (d >= 1) {
				std::stringstream stream;
				stream << "Index " << d << " is out of range [0, " << 0 << ']';
				throw std::out_of_range(stream.str());
			}
			return _size[0];
		}

		/**
		 *    @brief Gives the size of this vector.
		 *    @param s                  The output size of this vector.
		 */
		inline void allSizes(std::array<std::size_t, 1> &s) const
		{
			s = _size;
		}

		/**
		 *    @brief Gives the total size (number of elements in memory) of this
		 *           vector.
		 *    @return The total number of elements in memory.
		 */
		inline std::size_t size() const
		{
			return _size[0];
		}

		/**
		 *    @brief Gives the total offset, from the beginning of the stored
		 *           array, of the i-th element.
		 *    @param i                  The element whose offset is requested.
		 *    @return The total offset from the beggining of the stored array of
		 *            the i-th element.
		 */
		inline std::size_t accumulatedOffset(const std::size_t i = 0) const
		{
			if (i >= _size[0]) {
				std::stringstream stream;
				stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
				throw std::out_of_range(stream.str());
			}
			return _accumulatedOffset + _offset[0] * i;
		}

	private:
		friend class DopeVector<T, 2>;
		T                          *_array;                 ///< Pointer in memory to the first element of this vector.
		std::size_t                 _accumulatedOffset;     ///< Offset of the first element of this vector from the beginning of the stored array.
		std::array<std::size_t, 1>  _size;                  ///< Sizes of this matrix, for each dimension.
		std::array<std::size_t, 1>  _offset;                ///< Jumps' offsets from the beginning of a "row" to the beginning of the next one, for each dimension.
	};

//#include <DopeVector/inlines/DopeVector.inl>

}

#endif /* DopeVector_hpp */
