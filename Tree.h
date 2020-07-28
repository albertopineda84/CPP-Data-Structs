#pragma once

#include <string>
#include <cstdlib>
#include "Stack.h"


template <typename T>// T must respond to < operator 
class Tree {
private:
	struct Node
	{
		T mData = {};
		Node *mLeft = nullptr;
		Node *mRight = nullptr;
		int mHeight = 0;
	};
	Node *root;
public:
	Tree()
	{
		root = nullptr;
	}
	Tree(const Tree& tOther)
	{
		root = nullptr;
		Copy(tOther.root);
	}
	void Copy(Node *tLeaf)
	{
		if (tLeaf != nullptr) //do nothing if there is nothing at tLeaf
		{
			Add(tLeaf->mData); //Add handles all dynamic memory allocation
			Copy(tLeaf->mLeft);
			Copy(tLeaf->mRight);

		}
	}
	Tree & operator= (const Tree& tOther)
	{
		return tOther;
	}
	~Tree()
	{
		DeleteTree(root);
	}
	void DeleteTree(Node *tWhat)
	{
		if (tWhat == nullptr)
			return;
		DeleteTree(tWhat->mLeft);
		DeleteTree(tWhat->mRight);
		delete tWhat;
	}
	void Delete(const T& tWhat)
	{
		if (!Contains(tWhat))
			return;

		Node *tSearch = root;
		Node *tParent = nullptr;
		Stack<Node*> tTrail; //keeps track of all the nodes that can possibly be affected by the deletion of a descendent node

		while (tSearch->mData != tWhat) //keep traversing until you arrive at the node to be deleted
		{
			tParent = tSearch;
			tTrail.Push(tParent);
			if (tWhat < tSearch->mData) //set new tSearch equal to left of current
				tSearch = tSearch->mLeft;
			else if (tWhat > tSearch->mData)
				tSearch = tSearch->mRight;
		}
		if (tSearch->mLeft == nullptr && tSearch->mRight == nullptr) //has no children
		{
			if (tParent->mLeft == tSearch)
				tParent->mLeft = nullptr;
			else if (tParent->mRight == tSearch)
				tParent->mRight = nullptr;
			delete tSearch;

			tParent->mHeight = Height(tParent);
		}
		else if (tSearch->mLeft != nullptr && tSearch->mRight != nullptr) //has two children
		{
			Node *tNextHighest = tSearch->mRight;
			tTrail.Push(tSearch);
			while (tNextHighest->mLeft != nullptr)
			{
				tTrail.Push(tNextHighest); //we continue adding to the trail because we need to update the heights of all the nodes
				tNextHighest = tNextHighest->mLeft; //whose heights could have been changed as a result of deleting the "next highest" node
			}
			tSearch->mData = tNextHighest->mData;
			Delete(tNextHighest->mData); //works recursively in case the right side's left-most node has a child
		}
		else //has one child node
		{
			if (tSearch->mLeft == nullptr) //has right child
			{
				if (tParent->mLeft == tSearch) //move all pointers to tWhat to it's child, instead
					tParent->mLeft = tSearch->mRight;
				else if (tParent->mRight == tSearch)
					tParent->mRight = tSearch->mRight;
			}
			else
			{
				if (tParent->mLeft == tSearch)
					tParent->mLeft = tSearch->mLeft;
				else if (tParent->mRight == tSearch)
					tParent->mRight = tSearch->mLeft;
			}

			delete tSearch;

			tParent->mHeight = Height(tParent);
		}
		//after all the work is done, must call Balance() in case imbalance was created during delete
		//in order to detect an imbalance, must update the heights on our stack of node pointers
		Node *tPtr;
		while (tTrail.Size()  > 0)
		{
			tPtr = tTrail.Top();
			
			tPtr->mHeight = Height(tPtr);

			Balance(tPtr);
			tTrail.Pop();
		}
	}
	int Height(Node * tPtr)
	{
		if (tPtr == nullptr)
			return -1; //because we started with 0
		else
		{
			if (Height(tPtr->mLeft) > Height(tPtr->mRight))
				tPtr->mHeight = Height(tPtr->mLeft) + 1;
			else
				tPtr->mHeight = Height(tPtr->mRight) + 1;
		}

		return tPtr->mHeight;
	}
	void Add(const T& tWhat)
	{
		Node *tSearch = root;
		Node *tParent = nullptr;
		Stack<Node*> tTrail;
		while (tSearch != nullptr) //keep traversing until you arrive at the spot where the new value would be
		{
			tParent = tSearch;
			tTrail.Push(tParent);
			if (tWhat < tSearch->mData) //set new tSearch equal to left of current
				tSearch = tSearch->mLeft;

			else if (tWhat > tSearch->mData)
				tSearch = tSearch->mRight;
			else
				return;
		}
		if (tSearch == nullptr) //if false, we have already added the value or it already exists in the tree
		{
			tSearch = new Node; //add a new node with no value
			tSearch->mData = tWhat; //give the node value
			if (tParent == nullptr) //if there is nothing in the parent node, make tSearch the new root; this must be spot for new value
				root = tSearch;
			else if (tWhat < tParent->mData) //if the value is less than the parent's
			{
				tParent->mLeft = tSearch; //put the node on the left

				tTrail.Push(tParent->mLeft);

				tParent->mHeight = Height(tParent);

				Balance(tParent);

			} //will only increment if a new node is added to the larger subtree
			else if (tWhat > tParent->mData) //if the value is greater
			{
				tParent->mRight = tSearch; //put node on the right

				tTrail.Push(tParent->mRight);

				tParent->mHeight = Height(tParent);
				
				Balance(tParent);
			}

			Node *tPtr = nullptr;
			while (tTrail.Size()  > 0)
			{
				tPtr = tTrail.Top();
				
				tPtr->mHeight = Height(tPtr);

				Balance(tPtr);

				tTrail.Pop();
			}
		}									
	}
	void Balance(Node * tPtr)
	{
		if (tPtr == nullptr || Height(tPtr->mLeft) - Height(tPtr->mRight) < 2 || Height(tPtr->mRight) - Height(tPtr->mLeft) < 2)
			return; //have to check both sides for height imbalance (seems obvious, but don't ever forget)
		
		if (Height(tPtr->mLeft) - Height(tPtr->mRight) > 1 && tPtr->mLeft != nullptr) //means imbalance is to the left of tPtr
		{													//if left-left is bigger, perform single left rotation; left-right is bigger -> double left rotation
			if (Height(tPtr->mLeft->mLeft) > Height(tPtr->mLeft->mRight))
				tPtr->mLeft = SingleLeft(tPtr->mLeft);
			else if (Height(tPtr->mLeft->mRight) > Height(tPtr->mLeft->mLeft)) //we definitely DON'T want to rotate if the heights are equal
				tPtr = DoubleLeft(tPtr);
		}
		else if (Height(tPtr->mRight) - Height(tPtr->mLeft) > 1 && tPtr->mRight != nullptr)
		{
			if (Height(tPtr->mRight->mRight) > Height(tPtr->mRight->mLeft))
				tPtr->mRight = SingleRight(tPtr->mRight);
			else if (Height(tPtr->mRight->mLeft) > Height(tPtr->mRight->mRight))
				tPtr = DoubleRight(tPtr);
		}
	}
	Node *SingleLeft(Node* k2)
	{
		Node *k1 = k2->mLeft;
		k2->mLeft = k1->mRight;
		k1->mRight = k2;
		
		k2->mHeight = Height(k2);

		k1->mHeight = Height(k1);

		return k1;
	}
	Node *SingleRight(Node* k1)
	{
		Node *k2 = k1->mRight;
		k1->mRight = k2->mLeft;
		k2->mLeft = k1;

		k1->mHeight = Height(k1);

		k2->mHeight = Height(k2);

		return k2;
	}
	Node *DoubleLeft(Node* k3)
	{
		k3->mLeft = SingleRight(k3->mLeft);
		return SingleLeft(k3);
	}
	Node *DoubleRight(Node* k1)
	{
		k1->mRight = SingleLeft(k1->mRight);
		return SingleRight(k1);
	}
	bool Contains(const T& tWhat) const
	{
		Node *tSearch = root;
		if (tSearch == nullptr)
			return false;
		while (tSearch->mData != tWhat) //continue through loop while tWhat is not the same as our current leaf's value, unless the
		{												//leaf is dead or the value has been found
			if (tWhat < tSearch->mData)
			{
				tSearch = tSearch->mLeft;
				if (tSearch == nullptr) //means that our tWhat is not in the tree
					return false;
			}
			else if (tWhat > tSearch->mData)
			{
				tSearch = tSearch->mRight;
				if (tSearch == nullptr)
					return false;
			}
		}

		return true; //return true if the loop exits successfully; means tSearch->mData == tWhat
	}
	
	std::string Dump() const //create a function that calls dump on root
	{
		if (root != nullptr)
			return Dump(root);
		else
			return "Tree is empty.";
	}
	std::string Dump(Node *tPtr) const
	{
		std::string tRetVal = "";
		char tConversion[20]; //fair amount of buffer space

		if (tPtr != nullptr) //only continue if there is actually value at the node
		{
			tRetVal += Dump(tPtr->mLeft);

			_itoa_s(tPtr->mData, tConversion, 10); //only works b/c we know our data is int; need a way
			tRetVal += tConversion; //to convert generic template type to string
			tRetVal += " ";

			tRetVal += Dump(tPtr->mRight);
		}
		return tRetVal;
	}

	Node *GetRoot() { return root; }
};