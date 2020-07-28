/*
Alberto Pineda
CS 189

Assignment 5: Queue Implementation
*/

#pragma once

#include <list>

template<typename T>

class Queue
{
public:
	Queue() {}; //create an empty queue
	Queue( const Queue& other )
	{
		mQueue = other.mQueue;
	}
	~Queue()
	{
		while ( mQueue.size() != 0 )
			Pop();
	}
	T Front() const
	{
		return mQueue.front();
	}
	void Pop()
	{
		mQueue.pop_front();
	}
	void Push( const T& tData )
	{
		mQueue.push_back( tData );
	}
	int Size() const
	{
		return mQueue.size();
	}
private:
	std::list<T> mQueue;
};