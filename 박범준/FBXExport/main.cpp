#include "library.h"
#include "Converter.h"

int main()
{
	string FileName;
	cout << "[FBX Data Exporter]" << endl;
	cout << "FBX FILE INPUT: ";
	cin >> FileName;

	Converter::Instance()->Initialize(FileName);
}