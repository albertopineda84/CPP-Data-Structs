#pragma once

#include <list>


template<typename T>

class Stack
{
private:
	class ListNode
	{
	public:
		T mData;
		ListNode *mPrev; //Stack only requires an mPrev; we don't need access to the bottom elements
		ListNode(const T& data = T{}, ListNode *prev = nullptr, ListNode *next = nullptr)
		{					//{} initializes data to whatever the default zero value is for the datatype T
			mData = data;
			mPrev = prev;
			mNext = next;
		}
	};

public:

	class ConstIterator
	{

	public:
		ConstIterator()
		{
			mCurrent = nullptr;
		}
		ConstIterator(ListNode *tStart)
		{
			mCurrent = (tStart);
		}
		const T& GetData() const //recall: const before the fn. signature ensures that a const value is returned, while const after the fn.
		{						//ensures that our function is not changing the value of our object's member(s)
			return mCurrent->mData;
		}
		void Next() 
		{
			mCurrent = mCurrent->mNext; 
		}								
		bool IsEqual(const ConstIterator &rhs) const
		{
			return mCurrent == rhs.mCurrent;
		}
	public:
		ListNode *mCurrent;
	};

	class Iterator : public ConstIterator
	{
	public:
		Iterator(ListNode *tStart)
		{
			mCurrent = tStart;
		}
		T& GetData()
		{
			return mCurrent->mData;
		}
	};
public:
	Stack() {}; //create an empty stack
	Stack(const Stack& other)
	{
		mStack = other.mStack;
	}
	~Stack()
	{
		while (mStack.size() != 0)
			Pop();
		//no need for delete; no data has been dynamically allocated
	}
	void Push(const T& tData)
	{
		mStack.push_front(tData);
	}
	void Pop()
	{
		mStack.pop_front();
	}
	T Top() const //compiler complains that return can't convert 'const int' to 'int &'
	{
		return mStack.front(); //important to use front end when using list to implement stack; access to the front end is O(1) for lists
	}
	int Size() const
	{
		return mStack.size();
	}
	void Erase(Iterator tWhat)
	{
		ListNode *ptr = tWhat.mCurrent;
		ptr->mPrev->mNext = ptr->mNext;
		delete ptr;
		--mSize;
	}
	Iterator Begin()
	{
		return mHead;
	}
	Iterator End()
	{
		return mTail->mPrev;
	}
	ConstIterator Begin() const
	{
		return mHead;
	}
	ConstIterator End() const
	{
		return mTail->mPrev;
	}
private:
	std::list<T> mStack;
};