#include "Error.h"

Error::Error() {
	type = noError;
}

Error::Error(ErrorType newType) {
	type = newType;
}

Error::Error(ErrorType newType, string newErrorFileWay) {
	type = newType;
	if (type == inFileNotExist) {
		errorInputFileWay = newErrorFileWay;
	}
	else {
		errorOutputfileWay = newErrorFileWay;
	}
}

Error::Error(ErrorType newType, int newExpCount, int newCount) {
	type = newType;
	if (type == missingNumberOfElements || type == tooManyElements) {
		expColumnCount = newExpCount;
		columnCount = newCount;
	}
	else {
		expRowCount = newExpCount;
		rowCount = newCount;
	}
}

Error::Error(ErrorType newType, ElementPosition newPos, string newMatrixElement) {

}

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
	if (type != other.type)
	{
		return type < other.type;
	}
	if (rowCount != other.rowCount) {
		return rowCount < other.rowCount;
	}
	if (columnCount != other.columnCount) {
		return columnCount < other.columnCount;
	}
	if (!(pos == other.pos)) {
		return pos < other.pos;
	}
	return matrixElement < other.matrixElement;
}