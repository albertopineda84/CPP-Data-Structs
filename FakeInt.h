#pragma once
#include <string>

class FakeInt
{
public:
	FakeInt();
	FakeInt(int);
	void SetInt(int);
	std::string IntWord() const;
	std::string NumberToString(int) const;
private:
	int mMyNumber;

};