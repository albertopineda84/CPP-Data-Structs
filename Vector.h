/*
Alberto Pineda
CS189

Vector Implementation with Selection and Quick Sorts
*/

#pragma once

/* Sort() and QSort() added */

template<typename T>
class Vector
{
public:
	//////////////////////////////////////////////////////////////////////////
	/// ConstIterator
	class ConstIterator
	{
		// Even though having vanilla iterator inherit from the const one
		//sounds hinky, it is actually the universal standard.It does help some difficult bits. 
		// Maybe if they both had a VanillaIterator?  Well, there are actually a set of flags in the system's iterator that does stuff like that. 
		// I am intentionally not naming this const_iterator so it doesn't look like I am using those system flags.
	public:
		ConstIterator()
		{
			_current = nullptr;
		}
		ConstIterator( T *start )
		{
			_current = ( start );
		}
		const T& GetData() const
		{
			return *_current;
		}
		void Next()
		{
			_current = _current + 1;
		}
		bool IsEqual( const ConstIterator &rhs ) const
		{
			return _current == rhs._current;
		}
	public: //ConstIterator was not able to access mCurrent when it was "protected"?
		T *_current;
	}; //////////////////////////////////////////////////////////////////////////
	   /// Iterator
	class Iterator : public ConstIterator
	{
	public:
		Iterator( T *start )
		{
			_current = start;
		}
		T& GetData()
		{
			return *_current;
		}
	}; //////////////////////////////////////////////////////////////////////////
	   /// List List()
	explicit Vector( int initSize = 0 ) //define a default c'tor so we have something to start with
	{
		_size = initSize;
		_capacity = initSize + SPARE_CAPACITY;
		_data = new T[_capacity];
	}
	Vector( const Vector &rhs )
	{
		_size = rhs._size;
		_capacity = rhs._capacity;
		_data = nullptr;

		_data = new T[_capacity];

		for ( int i = 0; i < _size; ++i )
			_data[i] = rhs._data[i];
	}
	Vector operator= ( const Vector & rhs ) 
	{
		if (_size != rhs._size)
			_size = rhs._size;
		
		if (_capacity < rhs._capacity)
			Reserve(rhs.mCapcity);

		for ( int i = 0; i < _size; ++i )
			_data[i] = rhs._data[i];

		return rhs;
	}
	~Vector()
	{
		delete[] _data;
	}
	void PushFront( T data )
	{
		if (_size == _capacity)
			Reserve( 2 * _capacity + 1 );
		++_size;

		for ( int i = _size - 1; i > 0; --i )
			_data[i] = _data[i - 1];
		_data[0] = data;
	}
	void PushBack( T data)
	{
		if (_size == _capacity)
			Reserve( 2 * _capacity + 1 );
		_data[_size++] = data; //make element at index _size equal value of tData and then increment _size by 1
	}
	void PopFront()
	{
		if (_size == 0)
			return;

		--_size;

		for (int i = 0; i < _size - 1; ++i)
			_data[i] = _data[i + 1];
	}
	void PopBack()
	{
		if (_size == 0 )
			return;
		--_size; //though the element past endmarker technically has a value, we can ignore it; not included in main array
	}
	T& Front() const
	{
		return _data[0];
	}
	T& Back() const
	{
		return _data[_size - 1];
	}
	int Size() const
	{
		return _size;
	}
	void Clear()
	{
		while (_size != 0 )
			--_size;
	}
	void Erase( Iterator what ) //erase what tWhat is looking at and move all items after twhat down 1
	{
		if (_size == 0 )
			return;

		--_size;

		while ( !what.IsEqual(End()) )
		{
			*what._current = *(what._current + 1);
		}
	}
	T& At( int where )
	{
		return _data[where];
	}
	void Reserve( int howMuch )
	{
		if (howMuch < _size)
			return;
		
		T* oldData = _data;

		_data = new T[howMuch];
		
		for ( int i = 0; i < _size; ++i )
			_data[i] = oldData[i];

		_capacity = howMuch;

		delete[] oldData;
	}
	int Capacity() const
	{
		return _capacity;
	}

	void Sort() //selection sort
	{

		for (int i = 0; i < _size - 1; ++i) //don't need to check last item; array will be sorted at this point
		{
			int min = i; //assume minimum is at current position

			for (int j = i + 1; j < _size; ++j)
				if (_data[j] < _data[min])
					min = j;

			if (min != i) //if the nested loop did nothing, the item in index i is in the correct position
			{				//and there is no need to swap
				T temp = _data[min];
				_data[min] = _data[i];
				_data[i] = temp;
			}
		}
	}

	void QSort() //quick sort; will involve recursive calls on subarrays
	{
		QSort(_data, 0, _size - 1);
	}
	void QSort(T *arr, int lo, int hi) //it makes creating subarrays easier when you have two int parameters
	{

		if (lo < hi)
		{
			int partition = partitioner(arr, lo, hi);

			QSort(arr, lo, partition - 1);
			QSort(arr, partition + 1, hi);
		}
	}
	int partitioner(T* arr, int lo, int hi)
	{
		T pivot = arr[hi];
		--hi;

		while (lo < hi + 1)
		{
			if (pivot < arr[hi]) //if the pivot value is less than the value to its left, swap them
			{
				arr[hi + 1] = arr[hi];
				arr[hi] = pivot;
				--hi;
			}
			else //send the value to the left of pivot to the left side of the array if it's greater than the pivot
			{
				T temp = arr[lo];
				arr[lo] = arr[hi];
				arr[hi] = temp;
				++lo;
			}
		}

		return lo;
	}

	Iterator Begin()
	{
		return Iterator( _data );
	}
	Iterator End()
	{
		return Iterator( _data + _size );
	}						
	ConstIterator Begin() const
	{
		return Iterator( _data );
	}
	ConstIterator End() const
	{
		return Iterator( _data + _size );
	}
private:
	T* _data;
	int _size;
	int _capacity;

	static const int SPARE_CAPACITY = 16;
	static T sUndefinedT;// See Front for details.
};
template<typename T> T Vector<T>::sUndefinedT;
