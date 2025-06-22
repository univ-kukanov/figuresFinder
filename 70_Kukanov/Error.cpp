#include "Error.h"

Error::Error() {
	type = noError;
}

Error::Error(ErrorType newType) {
	type = newType;
}

Error::Error(ErrorType newType, string newErrorFilePath) {
	type = newType;
	if (type == inFileNotExist) {
		errorInputFilePath = newErrorFilePath;
	}
	else {
		errorOutputFilePath = newErrorFilePath;
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

void Error::setErrorInputFilePath(string newErrorInputFilePath) {
	errorInputFilePath = newErrorInputFilePath;
}

void Error::setErrorOutputFilePath(string newErrorOutputFilePath) {
	errorOutputFilePath = newErrorOutputFilePath;
}

ErrorType Error::getErrorType() const {
	return type;
}

int Error::getExpColumnCount() const {
	return expColumnCount;
}

int Error::getExpRowCount() const {
	return expRowCount;
}

int Error::getColumnCount() const {
	return columnCount;
}

int Error::getRowCount() const {
	return rowCount;
}

int Error::getCurrentRow() const {
	return currentRow;
}

string Error::getMatrixElement() const {
	return matrixElement;
}

ElementPosition Error::getPos() const {
	return pos;
}

string Error::getErrorInputFilePath() const {
	return errorInputFilePath;
}

string Error::getErrorOutputFilePath() const {
	return errorOutputFilePath;
}

string Error::generateErrorMessage() const {
	string errorString;
	switch (type) {
	case noError:
		break;

	case rowCountError:
		errorString = "��������� ��������� �� ���� ������� �������� �� 1x1 �� 1000x1000. ������� ���������� ������ � ���������� ����� � �������.";
		break;

	case columnCountError:
		errorString = "��������� ��������� �� ���� ������� �������� �� 1x1 �� 1000x1000. ������� ���������� ������ � ���������� �������� � �������.";
		break;

	case incorrectDimensionsCount:
		errorString = "������ ������� ������ �������. ������ ������� �������� ����� ������ ������� - ���������� �����, ���������� ��������.";
		break;

	case tooFewElements:
		errorString = "������� �� ������������� ��������� ��������: � ������ " + to_string(currentRow) + " ����������� ���������� ���������. ��������� ���������� - "
			+ to_string(expColumnCount) + ", ��������� ���������� - " + to_string(columnCount) + ". ������� ���������� �� �������� �������.";
		break;

	case tooFewRows:
		errorString = "������� �� ������������� ��������� ��������: ������� ���� �����. ��������� ���������� - " + to_string(expRowCount) + ", ��������� ���������� - "
			+ to_string(rowCount) + ". ������� ���������� �� �������� �������.";
		break;

	case tooManyElements:
		errorString = "������� �� ������������� ��������� ��������: � ������ " + to_string(currentRow) + " ������� ����� ���������. ��������� ���������� - "
			+ to_string(expColumnCount) + ", ��������� ���������� - " + to_string(columnCount) + ". ������� ���������� �� �������� �������.";
		break;

	case tooManyRows:
		errorString = "������� �� ������������� ��������� ��������: ������� ����� �����. ��������� ���������� - " + to_string(expRowCount) + ", ��������� ���������� - "
			+ to_string(rowCount) + ". ������� ���������� �� �������� �������.";
		break;

	case matrixSizeNotInt:
		errorString = "������ ������� ������ �������. ������� ��� ����� ������������� �����.";
		break;

	case matrixElementNotInt:
		errorString = "������� ������� \"" + matrixElement + "\" � ������ " + to_string(pos.getRow()) + " ������� "
			+ to_string(pos.getColumn()) + " �� �������� ����� ������. �������� ������� ����� ���� ������ ������ �������.";
		break;

	case matrixElementNotInRange:
		errorString = "������� ������� \"" + matrixElement + "\" � ������ " + to_string(pos.getRow()) + " ������� "
			+ to_string(pos.getColumn()) + " �� ������������� ����������� ��������� �������� [-2147483648..2147483647].";
		break;

	case inFileNotExist:
		errorString = "������� ������ ���� � �������� �������. ��������, ���� �� ����������. ����: " + errorInputFilePath;
		break;

	case outFileCreateFail:
		errorString = "������� ������ ���� ��� �������� ������. ��������, ���������� ������������ �� ���������� ��� ��� ���� �� ������. ����: " + errorOutputFilePath;
		break;

	case inFileIsEmpty:
		errorString = "��������� ������� ���� �������� ������.";
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
	if (errorInputFilePath != other.errorInputFilePath) {
		return errorInputFilePath < other.errorInputFilePath;
	}
	return errorOutputFilePath < other.errorOutputFilePath;
}

bool Error::operator==(const Error& other) const {
	return (type == other.type && expColumnCount == other.expColumnCount &&
		expRowCount == other.expRowCount && columnCount == other.columnCount &&
		rowCount == other.rowCount && currentRow == other.currentRow &&
		matrixElement == other.matrixElement && pos == other.pos &&
		errorInputFilePath == other.errorInputFilePath && errorOutputFilePath == other.errorOutputFilePath);
}