#include "Error.h"

void Error::setErrorType(ErrorType newType) {
	type = newType;
}

void Error::setExpColumnCount(int newExpColumnCount) {
	expColumnCount = newExpColumnCount;
}

void Error::setExpRowCount(int newExpRowCount) {
	expRowCount = newExpRowCount;
}

void Error::setColumnCount(int newColumnCount) {
	columnCount = newColumnCount;
}

void Error::setRowCount(int newRowCount) {
	rowCount = newRowCount;
}

void Error::setMatrixElement(string newMatrixElement) {
	matrixElement = newMatrixElement;
}

void Error::setPos(ElementPosition newPos) {
	pos = newPos;
}

void Error::setErrorInputFileWay(string newErrorInputFileWay) {
	errorInputFileWay = newErrorInputFileWay;
}

void Error::setErrorOutputfileWay(string newErrorOutputfileWay) {
	errorOutputfileWay = newErrorOutputfileWay;
}

ErrorType Error::getErrorType() const {
	return type;
}

string Error::generateErrorMessage() {
	switch (type) {
	case noError:
		break;

	case rowCountError:
		break;

	case columnCountError:
		break;

	case incorrectDimensionsCount:
		break;

	case missingNumberOfElements:
		break;

	case tooManyElements:
		break;

	case tooManyRows:
		break;

	case matrixSizeNotInt:
		break;

	case matrixElementNotInt:
		break;

	case matrixElementNotInRange:
		break;

	case inFileNotExist:
		break;

	case outFileCreateFail:
		break;

	case inFileIsEmpty:
		break;
	}

	string str = "testError";
	return str;
}

bool Error::operator<(const Error& other) const {
	if (type < other.type)
	{
		return true;
	}
	return false;
}