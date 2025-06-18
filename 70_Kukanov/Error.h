#pragma once
#include <string>
#include "ElementPosition.h"

using namespace std;

//!Перечень возможных ошибок
enum ErrorType {
	noError, rowCountError, columnCountError, incorrectDimensionsCount, tooFewElements, tooFewRows, tooManyElements, tooManyRows, matrixSizeNotInt,
	matrixElementNotInt, matrixElementNotInRange, inFileNotExist, outFileCreateFail, inFileIsEmpty
};

//!Error
class Error {
private:
	ErrorType type = noError;	//!<Тип ошибки
	int expColumnCount = 0;		//!<Ожидаемое количество столбцов
	int expRowCount = 0;		//!<Ожидаемое количество строк
	int columnCount = 0;		//!<Количество столбцов
	int rowCount = 0;			//!<Количество строк
	int currentRow = -1;		//!<Текущая строка
	string matrixElement = "";	//!<Элемент матрицы
	ElementPosition pos;		//!<Позиция элемента
	string errorInputFilePath = "";		//!<Путь файла с входными данными
	string errorOutputFilePath = "";	//!<Путь файла для выходных данных
public:
	void setErrorType(ErrorType newType);						//!<Сеттер типа ошибки
	void setExpColumnCount(int newExpColumnCount);				//!<Сеттер ожидаемого количества столбцов
	void setExpRowCount(int newExpRowCount);					//!<Сеттер ожидаемого количества строк
	void setColumnCount(int newColumnCount);					//!<Сеттер количества столбцов
	void setRowCount(int newRowCount);							//!<Сеттер количества строк
	void setMatrixElement(string newMatrixElement);				//!<Сеттер элемента матрицы
	void setPos(ElementPosition newPos);						//!<Сеттер позиции элемента
	void setErrorInputFilePath(string errorInputFilePath);		//!<Сеттер пути файла с входными данными
	void setErrorOutputFilePath(string errorOutputFilePath);	//!<Сеттер пути файла для выходных данных

	ErrorType getErrorType();			//!<Геттер типа ошибки
	int getExpColumnCount();			//!<Геттер ожидаемого количества столбцов
	int getExpRowCount();				//!<Геттер ожидаемого количества строк
	int getColumnCount();				//!<Геттер количества столбцов
	int getRowCount();					//!<Геттер количества строк
	int getCurrentRow();				//!<Геттер текущей строки
	string getMatrixElement();			//!<Геттер элемента матрицы
	ElementPosition getPos();			//!<Геттер позиции элемента
	string getErrorInputFilePath();		//!<Геттер пути файла с входными данными
	string getErrorOutputFilePath();	//!<Геттер пути файла для выходных данных

	/*! Генерация сообщения об ошибке
		\return - сообщение об ошибке
	*/
	string generateErrorMessage() const;

	Error();																	//!<Стандартный конструктор
	Error(ErrorType newType);													//!<Конструктор с типом ошибки
	Error(ErrorType newType, string newErrorFilePath);							//!<Конструктор для ошибки в файле
	Error(ErrorType newType, int newExpCount, int newCount);					//!<Конструктор для ошибки в количестве строк
	Error(ErrorType newType, int newExpCount, int newCount, int newCurrentRow);	//!<Конструктор для ошибки в количестве элементов в строке
	Error(ErrorType newType, ElementPosition newPos, string newMatrixElement);	//!<Конструктор для ошибки в элементе матрицы

	bool operator<(const Error& other) const;	//!<Перегрузка оператора "<"
	bool operator==(const Error& other) const;	//!<Перегрузка оператора "=="
};