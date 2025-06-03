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
	if (type == tooFewElements || type == tooManyElements) {
		expColumnCount = newExpCount;
		columnCount = newCount;
	}
	else {
		expRowCount = newExpCount;
		rowCount = newCount;
	}
}

Error::Error(ErrorType newType, int newExpCount, int newCount, int newCurrentRow) {
	type = newType;
	expColumnCount = newExpCount;
	columnCount = newCount;
	currentRow = newCurrentRow;
}

Error::Error(ErrorType newType, ElementPosition newPos, string newMatrixElement) {
	type = newType;
	pos = newPos;
	matrixElement = newMatrixElement;
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

ErrorType Error::getErrorType() {
	return type;
}

int Error::getExpColumnCount() {
	return expColumnCount;
}

int Error::getExpRowCount() {
	return expRowCount;
}

int Error::getColumnCount() {
	return columnCount;
}

int Error::getRowCount() {
	return rowCount;
}

int Error::getCurrentRow() {
	return currentRow;
}

string Error::getMatrixElement() {
	return matrixElement;
}

ElementPosition Error::getPos() {
	return pos;
}

string Error::getErrorInputFileWay() {
	return errorInputFileWay;
}

string Error::getErrorOutputfileWay() {
	return errorOutputfileWay;
}

string Error::generateErrorMessage() const {
	string errorString;
	switch (type) {
	case noError:
		break;

	case rowCountError:
		errorString = "The program accepts matrices with sizes from 1x1 to 1000x1000. Please enter valid row count.";
		break;

	case columnCountError:
		errorString = "The program accepts matrices with sizes from 1x1 to 1000x1000. Please enter valid column count.";
		break;

	case incorrectDimensionsCount:
		errorString = "Matrix dimensions specified incorrectly. Matrix size should be defined by two integers - row count and column count.";
		break;

	case tooFewElements:
		errorString = "Matrix doesn't match specified dimensions: row " + to_string(currentRow) + " has too few elements. Expected: "
			+ to_string(expColumnCount) + ", actual: " + to_string(columnCount) + ". Please enter a properly sized matrix.";
		break;

	case tooFewRows:
		errorString = "Matrix doesn't match specified dimensions: too few rows. Expected: " + to_string(expRowCount) + ", actual: "
			+ to_string(rowCount) + ". Please enter a properly sized matrix.";
		break;

	case tooManyElements:
		errorString = "Matrix doesn't match specified dimensions: row " + to_string(currentRow) + " has too many elements. Expected: "
			+ to_string(expColumnCount) + ", actual: " + to_string(columnCount) + ". Please enter a properly sized matrix.";
		break;

	case tooManyRows:
		errorString = "Matrix doesn't match specified dimensions: too many rows. Expected: " + to_string(expRowCount) + ", actual: "
			+ to_string(rowCount) + ". Please enter a properly sized matrix.";
		break;

	case matrixSizeNotInt:
		errorString = "Matrix dimensions specified incorrectly. Please enter two positive integers.";
		break;

	case matrixElementNotInt:
		errorString = "Matrix element \"" + matrixElement + "\" at row " + to_string(pos.getRow()) + " column "
			+ to_string(pos.getColumn()) + " is not an integer. Matrix elements must be integers only.";
		break;

	case matrixElementNotInRange:
		errorString = "Matrix element \"" + matrixElement + "\" at row " + to_string(pos.getRow()) + " column "
			+ to_string(pos.getColumn()) + " is out of valid range [-2147483648..2147483647].";
		break;

	case inFileNotExist:
		errorString = "Input file specified incorrectly. The file might not exist. Path: " + errorInputFileWay;
		break;

	case outFileCreateFail:
		errorString = "Output file specified incorrectly. The location might not exist or you don't have write permissions. Path: " + errorOutputfileWay;
		break;

	case inFileIsEmpty:
		errorString = "The specified input file is empty.";
		break;
	}

	return errorString;
}

bool Error::operator<(const Error& other) const {
	if (type != other.type)
	{
		return type < other.type;
	}
	if (expRowCount != other.expRowCount) {
		return expRowCount < other.expRowCount;
	}
	if (rowCount != other.rowCount) {
		return rowCount < other.rowCount;
	}
	if (columnCount != other.columnCount) {
		return columnCount < other.columnCount;
	}
	if (expColumnCount != other.expColumnCount) {
		return expColumnCount < other.expColumnCount;
	}
	if (currentRow != other.currentRow) {
		return currentRow < other.currentRow;
	}
	if (!(pos == other.pos)) {
		return pos < other.pos;
	}
	if (matrixElement != other.matrixElement) {
		return matrixElement < other.matrixElement;
	}
	if (errorInputFileWay != other.errorInputFileWay) {
		return errorInputFileWay < other.errorInputFileWay;
	}
	return errorOutputfileWay < other.errorOutputfileWay;
}

bool Error::operator==(const Error& other) const {
	return (type == other.type && expColumnCount == other.expColumnCount &&
		expRowCount == other.expRowCount && columnCount == other.columnCount &&
		rowCount == other.rowCount && currentRow == other.currentRow &&
		matrixElement == other.matrixElement && pos == other.pos &&
		errorInputFileWay == other.errorInputFileWay && errorOutputfileWay == other.errorOutputfileWay);
}