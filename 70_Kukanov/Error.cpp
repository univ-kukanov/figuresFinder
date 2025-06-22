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
		errorString = "Программа принимает на вход матрицы размером от 1x1 до 1000x1000. Введите корректные данные о количестве строк в матрице.";
		break;

	case columnCountError:
		errorString = "Программа принимает на вход матрицы размером от 1x1 до 1000x1000. Введите корректные данные о количестве столбцов в матрице.";
		break;

	case incorrectDimensionsCount:
		errorString = "Размер матрицы указан неверно. Размер матрицы задается двумя целыми числами - количество строк, количество столбцов.";
		break;

	case tooFewElements:
		errorString = "Матрица не соответствует указанным размерам: в строке " + to_string(currentRow) + " недостающее количество элементов. Ожидаемое количество - "
			+ to_string(expColumnCount) + ", введенное количество - " + to_string(columnCount) + ". Введите корректную по размерам матрицу.";
		break;

	case tooFewRows:
		errorString = "Матрица не соответствует указанным размерам: слишком мало строк. Ожидаемое количество - " + to_string(expRowCount) + ", введенное количество - "
			+ to_string(rowCount) + ". Введите корректную по размерам матрицу.";
		break;

	case tooManyElements:
		errorString = "Матрица не соответствует указанным размерам: в строке " + to_string(currentRow) + " слишком много элементов. Ожидаемое количество - "
			+ to_string(expColumnCount) + ", введенное количество - " + to_string(columnCount) + ". Введите корректную по размерам матрицу.";
		break;

	case tooManyRows:
		errorString = "Матрица не соответствует указанным размерам: слишком много строк. Ожидаемое количество - " + to_string(expRowCount) + ", введенное количество - "
			+ to_string(rowCount) + ". Введите корректную по размерам матрицу.";
		break;

	case matrixSizeNotInt:
		errorString = "Размер матрицы указан неверно. Введите два целых положительных числа.";
		break;

	case matrixElementNotInt:
		errorString = "Элемент матрицы \"" + matrixElement + "\" в строке " + to_string(pos.getRow()) + " столбце "
			+ to_string(pos.getColumn()) + " не является целым числом. Элементы матрицы могут быть только целыми числами.";
		break;

	case matrixElementNotInRange:
		errorString = "Элемент матрицы \"" + matrixElement + "\" в строке " + to_string(pos.getRow()) + " столбце "
			+ to_string(pos.getColumn()) + " не соответствует допустимому диапазону значений [-2147483648..2147483647].";
		break;

	case inFileNotExist:
		errorString = "Неверно указан файл с входными данными. Возможно, файл не существует. Путь: " + errorInputFilePath;
		break;

	case outFileCreateFail:
		errorString = "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись. Путь: " + errorOutputFilePath;
		break;

	case inFileIsEmpty:
		errorString = "Указанный входной файл является пустым.";
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