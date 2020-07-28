#pragma once
template<typename T>
class Gift
{
	T item;
public:
	void Shake()
	{
		item.MakeNoise();
	}
};