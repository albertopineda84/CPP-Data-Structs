#include "Gift.h"
#include "XBoxGame.h"
#include "WorldPeace.h"

int main()
{
	Gift<XBoxGame> skyrim;
	Gift<WorldPeace> imaginary;

	skyrim.Shake();
	return 0;
}