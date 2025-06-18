#pragma once
#include <string>
#include "ElementPosition.h"

using namespace std;

enum ErrorType {
	noError, rowCountError, columnCountError, incorrectDimensionsCount, tooFewElements, tooFewRows, tooManyElements, tooManyRows, matrixSizeNotInt,
	matrixElementNotInt, matrixElementNotInRange, inFileNotExist, outFileCreateFail, inFileIsEmpty
};

class Error {
private:
	ErrorType type = noError;
	int expColumnCount = 0;
	int expRowCount = 0;
	int columnCount = 0;
	int rowCount = 0;
	int currentRow = -1;
	string matrixElement = "";
	ElementPosition pos;
	string errorInputFilePath = "";
	string errorOutputFilePath = "";
public:
	void setErrorType(ErrorType newType);
	void setExpColumnCount(int newExpColumnCount);
	void setExpRowCount(int newExpRowCount);
	void setColumnCount(int newColumnCount);
	void setRowCount(int newRowCount);
	void setMatrixElement(string newMatrixElement);
	void setPos(ElementPosition newPos);
	void setErrorInputFilePath(string errorInputFilePath);
	void setErrorOutputFilePath(string errorOutputFilePath);

	ErrorType getErrorType();
	int getExpColumnCount();
	int getExpRowCount();
	int getColumnCount();
	int getRowCount();
	int getCurrentRow();
	string getMatrixElement();
	ElementPosition getPos();
	string getErrorInputFilePath();
	string getErrorOutputFilePath();

	string generateErrorMessage() const;

	Error();
	Error(ErrorType newType);
	Error(ErrorType newType, string newErrorFilePath);
	Error(ErrorType newType, int newExpCount, int newCount);
	Error(ErrorType newType, int newExpCount, int newCount, int newCurrentRow);
	Error(ErrorType newType, ElementPosition newPos, string newMatrixElement);

	bool operator<(const Error& other) const;
	bool operator==(const Error& other) const;
};