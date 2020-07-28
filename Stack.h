/*
Alberto Pineda
CS189

Assignment 5: Stack Implementation
*/

#pragma once

#include <list>


template<typename T>

class Stack
{
public:
	Stack() {}; //create an empty stack
	Stack( const Stack& other )
	{
		mStack = other.mStack;
	}
	~Stack()
	{
		while ( mStack.size() != 0 )
			Pop();
		//no need for delete; no data has been dynamically allocated
	}
	void Push( const T& tData )
	{
		mStack.push_front( tData );
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
private:
	std::list<T> mStack;
};