#include "FakeInt.h"
using namespace std;


FakeInt::FakeInt()
{
	mMyNumber = 0;
}

FakeInt::FakeInt(int tNum)
{
	SetInt(tNum);
}

void FakeInt::SetInt(int tNum)
{
	mMyNumber = tNum;
}

string FakeInt::IntWord() const
{
	int tWorking = mMyNumber;
	string tRetVal = "";
	string tWord;

	while (tWorking > 9)
	{
		tWord = NumberToString(tWorking % 10);
		tRetVal = tWord + " " + tRetVal;
		tWorking /= 10;
	}

	tRetVal = NumberToString(tWorking) + " " + tRetVal;

	return tRetVal;
}

string FakeInt::NumberToString(int tNum) const
{
	string tNumArray[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	return tNumArray[tNum];
}