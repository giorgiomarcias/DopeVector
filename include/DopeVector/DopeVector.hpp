// Copyright (c) 2016 Giorgio Marcias
//
// This source code is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com

#ifndef DopeVector_hpp
#define DopeVector_hpp

#include <sstream>
#include <stdexcept>
#include <cstring>
#include <DopeVector/Common/Index.hpp>

namespace container {

	/// The DopeVector class represents a D-dimensional dope vector
	/// (https://en.wikipedia.org/wiki/Dope_vector) of scalar type T. Given an
	/// array stored sequentially in memory, this class wraps it provinding a
	/// multi-dimensional matrix interface. It is possible to take slices,
	/// windows or even permutations without actually modifying the underlying
	/// memory, no element hence gets moved.
    template < typename T, SizeType Dimension, SizeType ... Args >
	class DopeVector {
	public:

        ////////////////////////////////////////////////////////////////////////
        // CONSTANTS
        ////////////////////////////////////////////////////////////////////////

        static constexpr SizeType D = Dimension;

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // TYPEDEFS
        ////////////////////////////////////////////////////////////////////////

        typedef Index< D > IndexD;

        ////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// CONSTRUCTORS
		////////////////////////////////////////////////////////////////////////

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
        inline explicit DopeVector(T *array, const SizeType accumulatedOffset, const IndexD &size);

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the D-dimensional matrix.
		 *    @param offset             Offsets in each dimension, i.e. jumps in
		 *                              memory.
		 */
        inline explicit DopeVector(T *array, const SizeType accumulatedOffset, const IndexD &size, const IndexD &offset);

		/**
		 *    @brief Copy constructor.
		 */
		DopeVector(const DopeVector &other) = default;

		/**
		 *    @brief Move constructor.
		 */
		DopeVector(DopeVector &&other) = default;

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// ASSIGNMENT OPERATORS
		////////////////////////////////////////////////////////////////////////

		/**
		 *    @brief Copy assignment operator.
		 */
		DopeVector & operator=(const DopeVector &other) = default;

		/**
		 *    @brief Move assignment operator.
		 */
		DopeVector & operator=(DopeVector &&other) = default;

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
        virtual inline void import(const DopeVector &o);

        /**
         *    @brief Swap this vector with vector o.
         *    @param o                  The DopeVector to swap with.
         */
        virtual inline void swap(DopeVector &o);

        /**
         *    @brief Swap vector v0 with vector v1.
         *    @param o                  The DopeVector to swap with.
         */
        friend inline void swap(DopeVector<T, Dimension, Args...> &v0, DopeVector<T, Dimension, Args...> &v1);

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// ACCESS METHODS
		////////////////////////////////////////////////////////////////////////

		/**
		 *    @brief Gives access to the i-th sub-matrix in the first dimension,
		 *           i.e. m[i][*]...[*].
		 *    @param i                  The i-th "row" of this matrix.
		 *    @param s                  The output sub-matrix at i.
		 */
        inline void at(const SizeType i, DopeVector<T, Dimension-1, Args...> &s) const;

        /**
         *    @brief Gives access to the i-th sub-matrix in the first dimension,
         *           i.e. m[i][*]...[*].
         *    @param i                  The i-th "row" of this matrix.
         *    @return The output sub-matrix at i.
         */
        inline DopeVector<T, Dimension-1, Args...> at(const SizeType i) const;

        /**
         *    @brief Gives access to the element at index i
         *    @param i                  The index of the element.
         *    @return The element at index i.
         */
        inline const T & at(const IndexD &i) const;

        /**
         *    @brief Gives access to the element at index i
         *    @param i                  The index of the element.
         *    @return The element at index i.
         */
        inline T & at(const IndexD &i);


		/**
		 *    @brief Gives access to the i-th sub-matrix in the first dimension,
		 *           i.e. m[i][*]...[*].
		 *    @param i                  The i-th "row" of this matrix.
		 *    @return The output sub-matrix at i.
		 */
        inline DopeVector<T, Dimension-1, Args...> operator[](const SizeType i) const;

        /**
         *    @brief Gives access to the element at index i
         *    @param i                  The index of the element.
         *    @return The element at index i.
         */
        inline const T & operator[](const IndexD &i) const;

        /**
         *    @brief Gives access to the element at index i
         *    @param i                  The index of the element.
         *    @return The element at index i.
         */
        inline T & operator[](const IndexD &i);

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// REDUCTION METHODS
		////////////////////////////////////////////////////////////////////////

        /**
		 *    @brief Gives access to the i-th sub-matrix in the d-th dimension,
		 *           i.e. m[*]...[i]...[*].
		 *    @param d                  The dimension where to slice.
		 *    @param i                  The i-th "row" of this matrix.
		 *    @param s                  The output sub-matrix at i in the d
		 *                              dimension.
		 */
        inline void slice(const SizeType d, const SizeType i, DopeVector<T, Dimension-1, Args...> &s) const;

		/**
		 *    @brief Gives access to the i-th sub-matrix in the d-th dimension,
		 *           i.e. m[*]...[i]...[*].
		 *    @param d                  The dimension where to slice.
		 *    @param i                  The i-th "row" of this matrix.
		 *    @return The output sub-matrix at i in the d dimension.
		 */
        inline DopeVector<T, Dimension-1, Args...> slice(const SizeType d, const SizeType i) const;

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < D goes 
		 *           to 0 <= order[i] < D, i.e. m[*]..[*]_i...[*] swaps with
		 *           m[*]...[i]...[*]_order[i]...[*].
		 *    @param order              A permutation of the matrix indices.
		 *    @param p                  The output permuted matrix.
		 *    @note Example: transpose a 2D matrix by swapping the access
		 *                   indices:
         *                   DopeVector<T,2> m, mt;
         *                   SizeType trans_ord[2] = {1, 0};
		 *                   m.permute(trans_ord, mt);
		 */
        inline void permute(const IndexD &order, DopeVector &p) const;

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < D goes to 0 <= order[i] < D, i.e. m[*]..[*]_i...[*] swaps with m[*]...[i]...[*]_order[i]...[*].
		 *    @param order              A permutation of the matrix indices.
		 *    @return The output permuted matrix.
         *    @note Example: transpose a 2D matrix by swapping the access indices - DopeVector<T,2> m, mt; SizeType trans_ord[2] = {1, 0}; mt = m.permute(trans_ord);
		 */
        inline DopeVector permute(const IndexD &order) const;

		/**
		 *    @brief Extracts a D-dimensional window from this matrix.
		 *    @param start              The initial offset of the window in each dimension.
		 *    @param size               The sizes of the window.
		 *    @param w                  The output sub-matrix.
		 */
        inline void window(const IndexD &start, IndexD &size, DopeVector &w) const;

		/**
		 *    @brief Extracts a D-dimensional window from this matrix.
		 *    @param start              The initial offset of the window in each dimension.
		 *    @param size               The sizes of the window.
		 *    @return The output sub-matrix.
		 */
        inline DopeVector window(const IndexD &start, const IndexD &size) const;

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// INFORMATION
		////////////////////////////////////////////////////////////////////////

		/**
		 *    @brief Gives the size of this matrix in the d dimension.
		 *    @param d                  The dimension whose size is requested.
		 *    @return The size of this matrix at dimension d.
		 */
        inline SizeType sizeAt(const SizeType d) const;

		/**
		 *    @brief Gives the sizes of this matrix.
		 *    @param s                  The output array that will contain the sizes of this matrix.
		 */
        inline void allSizes(IndexD &s) const;

        /**
         *    @brief Gives the sizes of this matrix.
         */
        inline const IndexD & allSizes() const;

		/**
		 *    @brief Gives the total size (number of elements in memory) of this matrix.
		 *    @return The total number of elements in memory.
		 */
        inline SizeType size() const;

		/**
		 *    @brief Gives the total offset, from the beginning of the stored array, of the i-th element at dimension d.
		 *    @param i                  The element whose offset is requested.
		 *    @param d                  The dimension whose i-th element offset is requested.
		 *    @return The total offset from the beggining of the stored array of the i-th element at dimension d.
		 */
        inline SizeType accumulatedOffset(const SizeType i, const SizeType d = 0) const;

		////////////////////////////////////////////////////////////////////////



	private:
        friend class DopeVector<T, Dimension+1, Args...>;
        T       *_array;                 ///< Pointer in memory to the first element of this matrix.
        SizeType _accumulatedOffset;     ///< Offset of the first element of this matrix from the beginning of the stored array.
        IndexD   _size;                  ///< Sizes of this matrix, for each dimension.
        IndexD   _offset;                ///< Jumps' offsets from the beginning of a "row" to the beginning of the next one, for each dimension.
	};



	/// The DopeVector class represents a 1-dimensional dope vector (https://en.wikipedia.org/wiki/Dope_vector) of scalar type T.
	/// Given an array stored sequentially in memory, this class wraps it provinding a mono-dimensional matrix interface.
	/// It is possible to take slices, windows or even permutations without actually modifying the underlying memory, no element
	/// hence moved.
    /// This actually is the basis of the recursive class DopeVector<T, D, Args...> above.
    template < typename T, SizeType ... Args >
    class DopeVector<T, 1, Args ...> {
	public:

        ////////////////////////////////////////////////////////////////////////
        // CONSTANTS
        ////////////////////////////////////////////////////////////////////////

        static const SizeType D = 1;

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
		// CONSTRUCTORS
		////////////////////////////////////////////////////////////////////////

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
        inline DopeVector(const T *array, const SizeType accumulatedOffset, const SizeType size);

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Size of the 1-dimensional matrix.
		 *    @param offset             Offset in memory from one element to the next one.
		 */
        inline DopeVector(const T *array, const SizeType accumulatedOffset, const SizeType size, const SizeType offset);

		/**
		 *    @brief Initializer contructor.
		 *
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the 1-dimensional matrix.
		 */
        inline DopeVector(const T *array, const SizeType accumulatedOffset, const Index1 &size);

		/**
		 *    @brief Initializer contructor.
		 *    @param array              Pointer to (part of) an array in memory
		 *                              to wrap.
		 *    @param accumulatedOffset  Offset from the origin of the array.
		 *    @param size               Sizes of the 1-dimensional matrix.
		 *    @param offset             Offsets in each dimension, i.e. jumps in
		 *                              memory.
		 */
        inline DopeVector(const T *array, const SizeType accumulatedOffset, const Index1 &size, const Index1 &offset);

		/**
		 *    @brief Copy constructor.
		 */
		DopeVector(const DopeVector &other) = default;

		/**
		 *    @brief Move constructor.
		 */
		DopeVector(DopeVector &&other) = default;

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// ASSIGNMENT OPERATORS
		////////////////////////////////////////////////////////////////////////

		/**
		 *    @brief Copy assignment operator.
		 */
		DopeVector & operator=(const DopeVector &other) = default;

		/**
		 *    @brief Move assignment operator.
		 */
		DopeVector & operator=(DopeVector &&other) = default;

		/**
		 *    @brief Copies all single elements from o to this matrix.
		 *    @param o                  The matrix to copy from.
		 */
        virtual inline void import(const DopeVector<T, 1, Args...> &o);

        /**
         *    @brief Swap this vector with DopeVector o.
         *    @param o                  The DopeVector to swap with.
         */
        virtual inline void swap( DopeVector<T, 1, Args...> &o);

        /**
         *    @brief Swap vector v0 with vector v1.
         *    @param o                  The DopeVector to swap with.
         */
        friend inline void swap(DopeVector<T, 1, Args...> &v0, DopeVector<T, 1, Args...> &v1);

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// ACCESS METHODS
		////////////////////////////////////////////////////////////////////////

        /**
         *    @brief Gives constant access to the i-th element, i.e. m[i].
         *    @param i                  The i-th element of this vector
         *    @return The output element at i.
         */
        inline const T & at(const SizeType i) const;

        /**
         *    @brief Gives access to the i-th element, i.e. m[i].
         *    @param i                  The i-th element of this vector
         *    @return The output element at i.
         */
        inline T & at(const SizeType i);

        /**
         *    @brief Gives constant access to the i-th element, i.e. m[i].
         *    @param i                  The i-th element of this vector
         *    @return The output element at i.
         */
        inline const T & at(const Index1 i) const;

        /**
         *    @brief Gives access to the i-th element, i.e. m[i].
         *    @param i                  The i-th element of this vector
         *    @return The output element at i.
         */
        inline T & at(const Index1 i);

		/**
		 *    @brief Gives constant access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector
		 *    @return The output element at i.
		 */
        inline const T & operator[](const SizeType i) const;

		/**
		 *    @brief Gives access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector
		 *    @return The output element at i.
		 */
        inline T & operator[](const SizeType i);

        /**
         *    @brief Gives constant access to the i-th element, i.e. m[i].
         *    @param i                  The i-th element of this vector
         *    @return The output element at i.
         */
        inline const T & operator[](const Index1 i) const;

        /**
         *    @brief Gives access to the i-th element, i.e. m[i].
         *    @param i                  The i-th element of this vector
         *    @return The output element at i.
         */
        inline T & operator[](const Index1 i);

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// REDUCTION METHODS
		////////////////////////////////////////////////////////////////////////

		/**
		 *    @brief Gives constant access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector.
		 *    @return The output element at i.
		 */
        inline const T & slice(const SizeType i) const;

		/**
		 *    @brief Gives access to the i-th element, i.e. m[i].
		 *    @param i                  The i-th element of this vector.
		 *    @return The output element at i.
		 */
        inline T & slice(const SizeType i);

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < 1 goes
		 *           to 0 <= order[i] < 1, i.e. m[0] swaps with m[order[0]].
		 *    @param order              A permutation of the matrix indices.
		 *    @param p                  The output permuted matrix.
		 *    @note In practice, this does nothing. It is kept for coherency
         *          with higher dimensional DopeVector.
		 */
        inline void permute(const Index1 &order, DopeVector<T, 1, Args...> &p) const;

		/**
		 *    @brief Reorders the sub-matrixes s.t. the one at 0 <= i < 1 goes to 0 <= order[i] < 1, i.e. m[0] swaps with m[order[0]].
		 *    @param order              A permutation of the matrix indices.
		 *    @return The output permuted matrix.
         *    @note In practice, this does nothing. It is kept for coherency with higher dimensional DopeVector.
		 */
        inline DopeVector<T, 1, Args...> permute(const Index1 &order) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @param w                  The output sub-vector.
		 */
        inline void window(const Index1 &start, const Index1 &size, DopeVector<T, 1, Args...> &w) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @return The output sub-vector.
		 */
        inline DopeVector<T, 1, Args...> window(const Index1 &start, const Index1 &size) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @param w                  The output sub-vector.
		 */
        inline void window(const SizeType start, const SizeType size, DopeVector<T, 1, Args...> &w) const;

		/**
		 *    @brief Extracts a 1-dimensional window from this vector.
		 *    @param start              The initial offset of the window.
		 *    @param size               The size of the window.
		 *    @return The output sub-vector.
		 */
        inline DopeVector<T, 1, Args...> window(const SizeType start, const SizeType size) const;

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// INFORMATION
		////////////////////////////////////////////////////////////////////////

		/**
		 *    @brief Gives the size of this vector
		 *    @param d                  The dimension whose size is requested
		 *                              (MUST be 0).
		 *    @return The size of this matrix at dimension d.
		 */
        inline SizeType sizeAt(const SizeType d) const;

		/**
		 *    @brief Gives the size of this vector.
		 *    @param s                  The output size of this vector.
		 */
        inline void allSizes(Index1 &s) const;

        /**
         *    @brief Gives the sizes of this matrix.
         */
        inline const Index1 & allSizes() const;

		/**
		 *    @brief Gives the total size (number of elements in memory) of this
		 *           vector.
		 *    @return The total number of elements in memory.
		 */
        inline SizeType size() const;

		/**
		 *    @brief Gives the total offset, from the beginning of the stored
		 *           array, of the i-th element.
		 *    @param i                  The element whose offset is requested.
		 *    @return The total offset from the beggining of the stored array of
		 *            the i-th element.
		 */
        inline SizeType accumulatedOffset(const SizeType i = 0) const;

		////////////////////////////////////////////////////////////////////////



	private:
        friend class DopeVector<T, 2, Args...>;
        T       *_array;                 ///< Pointer in memory to the first element of this vector.
        SizeType _accumulatedOffset;     ///< Offset of the first element of this vector from the beginning of the stored array.
        Index1   _size;                  ///< Sizes of this matrix, for each dimension.
        Index1   _offset;                ///< Jumps' offsets from the beginning of a "row" to the beginning of the next one, for each dimension.
	};

    template < typename T, SizeType ... Args >
    using DopeVector1D = DopeVector<T, 1, Args...>;

    template < typename T, SizeType ... Args >
    using DopeVector2D = DopeVector<T, 2, Args...>;

    template < typename T, SizeType ... Args >
    using DopeVector3D = DopeVector<T, 3, Args...>;

    //////////////////////////////////////////////////////////////////////////
    // TO DO
    //////////////////////////////////////////////////////////////////////////
    /*
    template < typename T, SizeType Size >
    class DopeVector< T, 1, Size > { };

    template < T, SizeType Size >
    using FixedDopeVector1D = DopeVector<T, 1, Size>;



    template < typename T, SizeType SizeU, SizeType SizeV >
    class DopeVector< T, 2, SizeU, SizeV > { };

    template < typename T, SizeType SizeU, SizeType SizeV >
    using FixedDopeVector2D = DopeVector<T, 2, SizeU, SizeV>;



    template < typename T, SizeType SizeU, SizeType SizeV, SizeType SizeW >
    class DopeVector< T, 3, SizeU, SizeV, SizeW > { };

    template < typename T, SizeType SizeU, SizeType SizeV, SizeType SizeW >
    using FixedDopeVector3D = DopeVector<T, 3, SizeU, SizeV, SizeW>;

    */
}

#include <DopeVector/inlines/DopeVector.inl>

#endif /* DopeVector_hpp */
