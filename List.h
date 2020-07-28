/*
Alberto Pineda
CS189

Assignment 3: List Implementation
*/

#pragma once
/* List -> Constructor, Destructor, Push, Pop, and Get for Front and Back, Size, Clear, Erase Node iterator GetData, Next, IsEqual (Begin and End needed for iterators) const_iterator GetData, Next, IsEqual */
template<typename T> 
class List 
{
private: ////////////////////////////////////////////////////////////////////////// /// ListNode
	class ListNode 
	{
	public: 
		T mData; // Putting this first makes it easier to see in the debugger
		ListNode *mPrev; 
		ListNode *mNext;
		ListNode( const T& data = T{}, ListNode *prev = nullptr, ListNode *next = nullptr ) //data is initialized with an empty list-initializer
		{																					//of type T
			mData = data;																	
			mPrev = prev;
			mNext = next;
		}
	}; 
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
				mCurrent = nullptr;
			}
			ConstIterator( ListNode *tStart ) //compiler complains -- IT WAS BECAUSE TSTART CAN'T BE CONST
			{
				mCurrent = ( tStart );
			}
			const T& GetData() const
			{
				return mCurrent->mData;
			}
			void Next()
			{
				mCurrent = mCurrent->mNext;
			}
			bool IsEqual( const ConstIterator &rhs ) const
			{
				return mCurrent == rhs.mCurrent;
			}
		public: //ConstIterator was not able to access mCurrent when it was "protected"?
			ListNode *mCurrent;
		}; //////////////////////////////////////////////////////////////////////////
		   /// Iterator
		class Iterator : public ConstIterator
		{
		public: 
			Iterator( ListNode *tStart ) 
			{
				mCurrent = tStart;
			}
			T& GetData() //having GetData() return a reference allows the fn. to be a modifiable lvalue; 
				//i.e. itr.GetData() = 2 would cause no compiler error. it also ensures that mData itself is returned, and not a copy of it
			{
				return mCurrent->mData;
			}
		}; //////////////////////////////////////////////////////////////////////////
		   /// List List()
		List() //define a default c'tor so we have something to start with
		{
			mSize = 0;
			mHead = new ListNode;
			mTail = new ListNode;
			mHead->mNext = mTail; 
			mTail->mPrev = mHead;
			//note: we only have to link mHead to mTail in the c'tors
		}
		List( const List &rhs )
		{
			mSize = 0;
			mHead = new ListNode;
			mTail = new ListNode;
			mHead->mNext = mTail; //creating link from head to tail
			mTail->mPrev = mHead; //creating link from tail to head

			ConstIterator itr = rhs.Begin();
			T& x = mHead->mNext->mData; //using reference b/c we want to work with mData itself, not make a copy
			while ( ! itr.IsEqual( rhs.End() ) )
			{
				x = itr.GetData();
				PushBack( x );

				if ( ! itr.IsEqual( rhs.End() ) )
					itr.Next();
			}
		}
		List operator= ( const List & rhs )
		{
			mSize = rhs.mSize;
			mHead = rhs.mHead;
			mTail = rhs.mTail;

			ConstIterator itr = rhs.Begin();
			T& x = mHead->mNext->mData;
			while ( ! itr.IsEqual( rhs.End() ) )
			{
				x = itr.GetData();
				PushBack( x );

				if ( ! itr.IsEqual( rhs.End() ) )
					itr.Next();
			}

			return rhs;
		}
		~List()
		{
			Clear();
			delete mHead;
			delete mTail;
		}
		void PushFront( T tData )
		{
			ListNode *ptr = mHead->mNext;
			ListNode *newNode = new ListNode( tData, ptr->mPrev, ptr ); //initialize a new node with its previous at mHead
			++mSize;											//and with its next at the old Begin()
			ptr->mPrev = ptr->mPrev->mNext = newNode;				//now, link newNode to mHead and to old Begin()
		}
		void PushBack( T tData )
		{
			ListNode *ptr = mTail;
			ListNode *newNode = new ListNode( tData, ptr->mPrev, ptr ); //new node pointed to by ptr
			++mSize;											
			ptr->mPrev = ptr->mPrev->mNext = newNode;				
		}
		void PopFront()
		{
			Erase( mHead->mNext );
		}
		void PopBack()
		{
			Erase( mTail->mPrev );
		}
		T& Front() const
		{
			return mHead->mNext->mData;
		}
		T& Back() const 
		{
			return mTail->mPrev->mData;
		}
		int Size() const
		{
			return mSize;
		}
		void Clear()
		{
			while ( mSize != 0 )
				PopFront();
		}
		void Erase( Iterator tWhat )
		{
			ListNode *ptr = tWhat.mCurrent;
			ptr->mPrev->mNext = ptr->mNext;
			ptr->mNext->mPrev = ptr->mPrev;
			delete ptr;
			--mSize;
		}
		Iterator Begin()
		{
			return mHead->mNext;
		}
		Iterator End()
		{
			return mTail;
		}
		ConstIterator Begin() const
		{
			return mHead->mNext;
		}
		ConstIterator End() const
		{
			return mTail;
		}
private:
	ListNode *mHead; 
	ListNode *mTail; 
	int mSize; 
	static T sUndefinedT;// See Front for details.
};
template<typename T> T List<T>::sUndefinedT;