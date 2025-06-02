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

string Error::generateErrorMessage() {
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

	case missingNumberOfElements:
		errorString = "Матрица не соответствует указанным размерам: в строке " + to_string(currentRow) + " недостающее количество элементов. Ожидаемое количество - "
			+ to_string(expColumnCount) + ", введенное количество - " + to_string(columnCount) + ". Введите корректную по размерам матрицу.";
		break;

	case tooManyElements:
		errorString = "Матрица не соответствует указанным размерам: в строке " + to_string(currentRow) + " слишком много элементов. Ожидаемое количество - "
			+ to_string(expColumnCount) + ", введенное количество - " + to_string(columnCount) + ". Введите корректную по размерам матрицу.";
		break;

	case tooManyRows:
		errorString = "Матрица не соответствует указанным размерам: слишком много строк. Ожидаемое количество - " + to_string(expRowCount) + ", введенное количество - " + to_string(rowCount) + ". Введите корректную по размерам матрицу.";
		break;

	case matrixSizeNotInt:
		errorString = "Размер матрицы указан неверно. Введите два целых числа.";
		break;

	case matrixElementNotInt:
		errorString = "Элемент матрицы “" + matrixElement + "” в строке " + to_string(pos.getRow()) + " столбце " + to_string(pos.getColumn()) + " не является целым числом. Элементы матрицы могут быть только целыми числами.";
		break;

	case matrixElementNotInRange:
		errorString = "Элемент матрицы “" + matrixElement + "” в строке " + to_string(pos.getRow()) + " столбце " + to_string(pos.getColumn()) + " не соответствует допустимому диапазону значений [−2147483648..2147483647].";
		break;

	case inFileNotExist:
		errorString = "Неверно указан файл с входными данными. Возможно, файл не существует. Путь: " + errorInputFileWay;
		break;

	case outFileCreateFail:
		errorString = "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись. Путь: " + errorOutputfileWay;
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
	if (rowCount != other.rowCount) {
		return rowCount < other.rowCount;
	}
	if (columnCount != other.columnCount) {
		return columnCount < other.columnCount;
	}
	if (currentRow != other.currentRow) {
		return currentRow < other.currentRow;
	}
	if (!(pos == other.pos)) {
		return pos < other.pos;
	}
	return matrixElement < other.matrixElement;
}