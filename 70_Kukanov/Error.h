#pragma once
#include <string>
#include "ElementPosition.h"

using namespace std;

enum ErrorType {
	noError, rowCountError, columnCountError, incorrectDimensionsCount, missingNumberOfElements, tooManyElements, tooManyRows, matrixSizeNotInt,
	matrixElementNotInt, matrixElementNotInRange, inFileNotExist, outFileCreateFail, inFileIsEmpty
};

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
	void setErrorType(ErrorType newType);
	void setExpColumnCount(int newExpColumnCount);
	void setExpRowCount(int newExpRowCount);
	void setColumnCount(int newColumnCount);
	void setRowCount(int newRowCount);
	void setMatrixElement(string newMatrixElement);
	void setPos(ElementPosition newPos);
	void setErrorInputFileWay(string newErrorInputFileWay);
	void setErrorOutputfileWay(string newErrorOutputfileWay);

	string generateErrorMessage();
};