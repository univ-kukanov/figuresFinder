#pragma once
#include <string>
#include "ElementPosition.h"

using namespace std;

class Error {
private:
	ErrorType type;
	int expColumnCount;
	int expRowCount;
	int columnCount;
	int rowColumn;
	string matrixElement;
	ElementPosition pos;
	string errorInputFileWay;
	string errorOutputfileWay;
public:
	string generateErrorMessage();
};