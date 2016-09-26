// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#include <DopeVector/internal/Iterator.hpp>
#include <limits>

namespace dope {

	namespace internal {

		////////////////////////////////////////////////////////////////////////
		// CONSTRUCTORS
		////////////////////////////////////////////////////////////////////////

		template < typename T, SizeType Dimension, bool Const >
		inline Iterator<T, Dimension, Const>::Iterator()
			: _data()
			, _currentIndex(IndexD::Constant(std::numeric_limits<SizeType>::max()))
		{ }

		template < typename T, SizeType Dimension, bool Const >
		inline Iterator<T, Dimension, Const>::Iterator(const DopeVectorRef &dope_vector, const SizeType i)
			: _data(dope_vector)
			, _currentIndex(IndexD::to_index(i, static_cast<DopeVectorType>(dope_vector).allSizes()))
		{ }

		template < typename T, SizeType Dimension, bool Const >
		inline Iterator<T, Dimension, Const>::Iterator(const DopeVectorRef &dope_vector, const IndexD &index)
			: _data(dope_vector)
			, _currentIndex(index)
		{ }

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// INFORMATION
		////////////////////////////////////////////////////////////////////////

		template < typename T, SizeType Dimension, bool Const >
		inline SizeType Iterator<T, Dimension, Const>::to_original() const
		{
			return _data.accumulatedOffset(_currentIndex);
		}

		template < typename T, SizeType Dimension, bool Const >
		inline SizeType Iterator<T, Dimension, Const>::to_position() const
		{
			return IndexD::to_position(_currentIndex, _data.allSizes());
		}

		template < typename T, SizeType Dimension, bool Const >
		inline const typename Iterator<T, Dimension, Const>::IndexD & Iterator<T, Dimension, Const>::to_index() const
		{
			return _currentIndex;
		}

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// DATA ACCESS METHODS
		////////////////////////////////////////////////////////////////////////

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::reference Iterator<T, Dimension, Const>::operator*()
		{
			return static_cast<DopeVectorType>(_data).at(_currentIndex);
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::pointer Iterator<T, Dimension, Const>::operator->()
		{
			return &static_cast<DopeVectorType>(_data).at(_currentIndex);
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::reference Iterator<T, Dimension, Const>::operator[](const SizeType n) const
		{
			return *(*this + n);
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::reference Iterator<T, Dimension, Const>::operator[](const IndexD &n) const
		{
			return *(*this + n);
		}

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// INCREMENT OPERATIONS
		////////////////////////////////////////////////////////////////////////

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator++()
		{
			return *this += static_cast<SizeType>(1);
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator++(int)
		{
			self_type copy(*this);
			++*this;
			return copy;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type & Iterator<T, Dimension, Const>::operator+=(const SizeType n)
		{
			Index<Dimension> size = static_cast<DopeVectorType>(_data).allSizes();
			SizeType tmp_val, carry = n;
			for (SizeType D = Dimension; D > 0 && carry != static_cast<SizeType>(0); --D) {
				SizeType d = D - static_cast<SizeType>(1);
				tmp_val = _currentIndex[d] + carry;
				carry = tmp_val / size[d];
				_currentIndex[d] = tmp_val % size[d];
			}
			if (carry != static_cast<SizeType>(0))
				_currentIndex = IndexD::to_index(static_cast<DopeVectorType>(_data).size(), size);
			return *this;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator+ (const SizeType n) const {
			self_type copy(*this);
			copy += n;
			return copy;
		}

		template < typename T, SizeType Dimension, bool Const > template < class E >
		inline typename Iterator<T, Dimension, Const>::self_type & Iterator<T, Dimension, Const>::operator+=(const internal::StaticArrayExpression<E, SizeType, Dimension> &n)
		{
			Index<Dimension> size = static_cast<DopeVectorType>(_data).allSizes();
			SizeType tmp_val, carry = static_cast<SizeType>(0);
			for (SizeType D = Dimension; D > static_cast<SizeType>(0); --D) {
				SizeType d = D - static_cast<SizeType>(1);
				tmp_val = _currentIndex[d] + n.getAt(d) + carry;
				carry = tmp_val / size[d];
				_currentIndex[d] = tmp_val % size[d];
			}
			if (carry != static_cast<SizeType>(0))
				_currentIndex = IndexD::to_index(static_cast<DopeVectorType>(_data).size(), size);
			return *this;
		}

		template < typename T, SizeType Dimension, bool Const > template < class E >
		inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator+ (const internal::StaticArrayExpression<E, SizeType, Dimension> &n) const {
			self_type copy(*this);
			copy += n;
			return copy;
		}

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// DECREMENT OPERATIONS
		////////////////////////////////////////////////////////////////////////

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type & Iterator<T, Dimension, Const>::operator--() {
			return *this -= static_cast<SizeType>(1);
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator--(int) {
			self_type copy(*this);
			--*this;
			return copy;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type & Iterator<T, Dimension, Const>::operator-=(const SizeType n)
		{
			Index<Dimension> size = static_cast<DopeVectorType>(_data).allSizes();
			difference_type tmp_val, loan, carry = -static_cast<difference_type>(n);
			for (SizeType D = Dimension; D > static_cast<SizeType>(0) && carry != static_cast<difference_type>(0); --D) {
				SizeType d = D - static_cast<SizeType>(1);
				tmp_val = static_cast<difference_type>(_currentIndex[d]) + carry;
				loan = static_cast<difference_type>(0);
				if (tmp_val < static_cast<difference_type>(0))
					loan = std::abs(tmp_val) / static_cast<difference_type>(size[d]) + (std::abs(tmp_val) % static_cast<difference_type>(size[d]) != static_cast<SizeType>(0) ? static_cast<difference_type>(1) : static_cast<difference_type>(0));
				tmp_val += loan * static_cast<difference_type>(size[d]);
				carry = tmp_val / static_cast<difference_type>(size[d]) - loan;
				_currentIndex[d] = static_cast<SizeType>(tmp_val) % size[d];
			}
			if (carry != static_cast<difference_type>(0))
				_currentIndex = IndexD::to_index(static_cast<DopeVectorType>(_data).size(), size);
			return *this;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator- (const SizeType n) const
		{
			self_type copy(*this);
			copy -= n;
			return copy;
		}

		template < typename T, SizeType Dimension, bool Const > template < class E >
		inline typename Iterator<T, Dimension, Const>::self_type & Iterator<T, Dimension, Const>::operator-=(const internal::StaticArrayExpression<E, SizeType, Dimension> &n)
		{
			Index<Dimension> size = static_cast<DopeVectorType>(_data).allSizes();
			difference_type tmp_val, loan, carry = static_cast<difference_type>(0);
			for (SizeType D = Dimension; D > static_cast<SizeType>(0); --D) {
				SizeType d = D - static_cast<SizeType>(1);
				tmp_val = static_cast<difference_type>(_currentIndex[d]) + carry - static_cast<difference_type>(n.getAt(d));
				loan = static_cast<difference_type>(0);
				if (tmp_val < static_cast<difference_type>(0))
					loan = std::abs(tmp_val) / static_cast<difference_type>(size[d]) + (std::abs(tmp_val) % static_cast<difference_type>(size[d]) != static_cast<SizeType>(0) ? static_cast<difference_type>(1) : static_cast<difference_type>(0));
				tmp_val += loan * static_cast<difference_type>(size[d]);
				carry = tmp_val / static_cast<difference_type>(size[d]) - loan;
				_currentIndex[d] = static_cast<SizeType>(tmp_val) % size[d];
			}
			if (carry != static_cast<difference_type>(0))
				_currentIndex = IndexD::to_index(static_cast<DopeVectorType>(_data).size(), size);
			return *this;
		}

		template < typename T, SizeType Dimension, bool Const > template < class E >
		inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator-(const internal::StaticArrayExpression<E, SizeType, Dimension> &n) const
		{
			self_type copy(*this);
			copy -= n;
			return copy;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline typename Iterator<T, Dimension, Const>::difference_type Iterator<T, Dimension, Const>::operator- (const self_type &o) const
		{
			if(_data == o._data)
				return static_cast<difference_type>(o.to_position()) - static_cast<difference_type>(to_position());
			return std::numeric_limits<difference_type>::max();
		}

		////////////////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////////////////
		// BOOLEAN OPERATIONS
		////////////////////////////////////////////////////////////////////////

		template < typename T, SizeType Dimension, bool Const >
		inline bool Iterator<T, Dimension, Const>::operator==(const self_type &o) const
		{
			return static_cast<DopeVectorType>(_data) == static_cast<DopeVectorType>(o._data) && _currentIndex == o._currentIndex;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline bool Iterator<T, Dimension, Const>::operator!=(const self_type &o) const
		{
			return static_cast<DopeVectorType>(_data) == static_cast<DopeVectorType>(o._data) && _currentIndex != o._currentIndex;
		}

		template < typename T, SizeType Dimension, bool Const >
		inline bool Iterator<T, Dimension, Const>::operator< (const self_type &o) const
		{
			return static_cast<DopeVectorType>(_data) == static_cast<DopeVectorType>(o._data) && to_position() < o.to_position();
		}

		template < typename T, SizeType Dimension, bool Const >
		inline bool Iterator<T, Dimension, Const>::operator<=(const self_type &o) const
		{
			return static_cast<DopeVectorType>(_data) == static_cast<DopeVectorType>(o._data) && to_position() <= o.to_position();
		}

		template < typename T, SizeType Dimension, bool Const >
		inline bool Iterator<T, Dimension, Const>::operator> (const self_type &o) const
		{
			return static_cast<DopeVectorType>(_data) == static_cast<DopeVectorType>(o._data) && to_position() > o.to_position();
		}

		template < typename T, SizeType Dimension, bool Const >
		inline bool Iterator<T, Dimension, Const>::operator>=(const self_type &o) const
		{
			return static_cast<DopeVectorType>(_data) == static_cast<DopeVectorType>(o._data) && to_position() >= o.to_position();
		}

		////////////////////////////////////////////////////////////////////////

	} // namespace internal

} // namespace dope
