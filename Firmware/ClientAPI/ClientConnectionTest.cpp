#include <iostream>
#include "ClientConnection.h"

using namespace std;

void TestClientLoad()
{
	Client forge(1232, "Crack", 3);
	ClientConnection conn(forge);
}

int main(void)
{
	cout << "Begin Test" << endl;
	TestClientLoad();

	return 0;
}