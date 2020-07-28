#pragma once

#include <string>
#include <cstdlib>

/* Basic binary search tree with private node class and core contract functions. Add and Remove and Contains must be done with loops, not recursion.
We won't be balancing trees until next week, so Remove can go ahead and just mark a node as "Dead". That is, the node stays there so it can route numbers left and right, but it doesn't count as being in the tree if you search for it.
*/ template <typename T>// T must respond to < operator 
class Tree {
private:
	struct Node
	{
		T mData = {};
		Node *mLeft = nullptr;
		Node *mRight = nullptr;
		bool mDead = false; //initialize a node's "dead status" to false

		Node(const T& data, Node *left, Node * right)
		{
			mData = data;
			mLeft = left;
			mRight = right;
			mDead = false;
		}
	};
	Node *root;

	void PrivateAdd( const T& x, Node *& tPtr )
	{
		if (tPtr == nullptr)
			tPtr = new Node(x, nullptr, nullptr);
		else if (x < tPtr->mData)
			PrivateAdd(x, tPtr->mLeft);
		else if (tPtr->mData < x) //using only operator< would assure that you would only have to overload <; imo, you might as well do both
			PrivateAdd(x, tPtr->mRight);
		else if (x == tPtr->mData && tPtr->mDead)
			tPtr->mDead = false; //if you encounter a dead node, mark it active
	}

	Node *Container(const T& tWhat) const //returns the pointer which contains the value we are looking for
	{
		Node *tSearch = root;
		if (tSearch == nullptr)
			return nullptr;

		while (tSearch->mData != tWhat) //continue through loop while tWhat is not the same as our current leaf's value, unless the
		{			//leaf is dead or the value has been found
			if (tWhat < tSearch->mData)
				tSearch = tSearch->mLeft;
			else if (tWhat > tSearch->mData)
				tSearch = tSearch->mRight;
		}
		return tSearch;
	}
	void Copy(Node *tLeaf)
	{
		if (tLeaf != nullptr) //do nothing if there is nothing at tLeaf
		{
			Add(tLeaf->mData); //Add handles all dynamic memory allocation
			if (tLeaf->mDead) //if a node is actually dead, we want it in the tree still but we want it marked as dead
				Remove(tLeaf->mData);
			Copy(tLeaf->mLeft);
			Copy(tLeaf->mRight);

		}
	}
	void Delete(Node *tWhat) //deletes entire tree recursively
	{
		if (tWhat == nullptr)
			return;
		Delete(tWhat->mLeft);
		Delete(tWhat->mRight);
		delete tWhat;
	}
	std::string Dump(Node *tPtr) const
	{
		std::string tRetVal = "";
		char tConversion[20]; //fair amount of buffer space. if it was an unknown amount, use a vector of chars

		if (tPtr != nullptr) //only continue if there is actually value at the node
		{
			tRetVal += Dump(tPtr->mLeft);
			if (!tPtr->mDead)
			{
				_itoa_s(tPtr->mData, tConversion, 10); //note: only works cause we know mData is int; need a
				tRetVal += tConversion; //way to convert generic template types to string
				tRetVal += " ";
			}
			tRetVal += Dump(tPtr->mRight);
		}
		return tRetVal;
	}
public:
	Tree()
	{
		root = nullptr;
	}
	Tree( const Tree& tOther ) //we'll make the copy c'tor copy even the dead nodes into the new tree (but we have to make sure to mark them dead)
	{
		root = nullptr;
		Copy(tOther.root);
	}
	Tree & operator= ( const Tree& tOther )
	{
		return tOther;
	}
	~Tree()
	{
		Delete(root); 
	}
	void Add( const T& tWhat )
	{
		PrivateAdd( tWhat, root );									//do nothing if value is equal
	}
	void Remove( const T& tWhat )
	{
		Node *tSearch = Container(tWhat);

		if (tSearch != nullptr && !tSearch->mDead) //if value at tSearch has not yet been removed and it exists, remove it
			tSearch->mDead = true;
	}
	bool Contains( const T& tWhat ) const //is slightly faster than a recursive version of this fn.
	{
		Node *tSearch = root;
		if ( tSearch == nullptr || tSearch->mDead )
			return false;
		while ( tSearch->mData != tWhat && ! tSearch->mDead ) //continue through loop while tWhat is not the same as our current leaf's value, unless the
		{												//leaf is dead or the value has been found
			if ( tWhat < tSearch->mData )
			{
				tSearch = tSearch->mLeft;
				if ( tSearch == nullptr  || tSearch->mDead ) //means that our tWhat is not in the tree
					return false;
			}
			else if ( tWhat > tSearch->mData )
			{
				tSearch = tSearch->mRight;
				if ( tSearch == nullptr || tSearch->mDead )
					return false;
			}
		}

		return true; //return true if the loop exits successfully; means tSearch->mData == tWhat
	}
	std::string Dump() const //create a function that calls dump on root
	{
		if (Dump(root) == "") //this condition means that all nodes in the tree have been removed
			return "Tree is empty.";
		else if (root != nullptr && !root->mDead)
			return Dump(root);
		else if (root->mDead)
			return (Dump(root->mLeft) + Dump(root->mRight));
	}
};