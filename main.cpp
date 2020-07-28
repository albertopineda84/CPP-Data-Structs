#include "FakeInt.h"

#include <iostream>
using namespace std;

int main()
{
	int tMyNum = 1;

	while (tMyNum > 0)
	{
		cout << "Please enter a number less than " << INT_MAX << " to return its word form." << endl;
		cout << "Otherwise, enter a number less than 1 to quit: ";
		cin >> tMyNum;

		while (tMyNum > INT_MAX)
		{
			cerr << "Error! You have entered an invalid value. Please enter a lower number: ";
			cin >> tMyNum;
		}

		if (tMyNum <= 0)
			break;

		FakeInt ThisInt(tMyNum);

		cout << ThisInt.IntWord() << endl;


	}

	system("pause");

	return 0;
}