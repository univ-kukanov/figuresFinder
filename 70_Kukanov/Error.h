#pragma once
#include <string>
#include "ElementPosition.h"

using namespace std;

//!Перечень возможных ошибок
enum ErrorType {
	noError, rowCountError, columnCountError, incorrectDimensionsCount, tooFewElements, tooFewRows, tooManyElements, tooManyRows, matrixSizeNotInt,
	matrixElementNotInt, matrixElementNotInRange, inFileNotExist, outFileCreateFail, inFileIsEmpty
};

//!Класс, представляющий ошибку
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
	/*! Установить тип ошибки
		\param[in] newType - новый тип ошибки
	*/
	void setErrorType(ErrorType newType);
	/*! Установить ожидаемое количество столбцов
		\param[in] newExpColumnCount - новое ожидаемое количество столбцов
	*/
	void setExpColumnCount(int newExpColumnCount);
	/*! Установить ожидаемое количество строк
		\param[in] newExpRowCount - новое ожидаемое количество строк
	*/
	void setExpRowCount(int newExpRowCount);
	/*! Установить реальное количество столбцов
		\param[in] newColumnCount - новое реальное количество столбцов
	*/
	void setColumnCount(int newColumnCount);
	/*! Установить реальное количество строк
		\param[in] newRowCount - новое реальное количество строк
	*/
	void setRowCount(int newRowCount);
	/*! Установить значение элемента матрицы
		\param[in] newMatrixElement - новое значение элемента матрицы
	*/
	void setMatrixElement(string newMatrixElement);
	/*! Установить позицию элемента матрицы
		\param[in] newPos - новая позиция элемента матрицы
	*/
	void setPos(ElementPosition newPos);
	/*! Установить путь файла с входными данными
		\param[in] errorInputFilePath - новый путь файла с входными данными
	*/
	void setErrorInputFilePath(string errorInputFilePath);
	/*! Установить путь файла для выходных данных
		\param[in] errorOutputFilePath - новый путь файла для выходных данных
	*/
	void setErrorOutputFilePath(string errorOutputFilePath);

	/*! Получить тип ошибки
		\return - тип ошибки
	*/
	ErrorType getErrorType();
	/*! Получить ожидаемое количество столбцов
		\return - ожидаемое количество столбцов
	*/
	int getExpColumnCount();
	/*! Получить ожидаемое количество строк
		\return - ожидаемое количество строк
	*/
	int getExpRowCount();
	/*! Получить реальное количество столбцов
		\return - реальное количество столбцов
	*/
	int getColumnCount();
	/*! Получить реальное количество строк
		\return - реальное количество строк
	*/
	int getRowCount();
	/*! Получить номер текущей строки
		\return - номер текущей строки
	*/
	int getCurrentRow();
	/*! Получить значение элемента матрицы
		\return - значение элемента матрицы
	*/
	string getMatrixElement();
	/*! Получить позицию элемента матрицы
		\return - позиция элемента матрицы
	*/
	ElementPosition getPos();
	/*! Получить путь файла с входными данными
		\return - путь файла с входными данными
	*/
	string getErrorInputFilePath();
	/*! Получить путь файла для выходных данных
		\return - путь файла для выходных данных
	*/
	string getErrorOutputFilePath();

	/*! Генерация сообщения об ошибке
		\return - сообщение об ошибке
	*/
	string generateErrorMessage() const;

	/*! Стандартный конструктор */
	Error();
	/*! Конструктор с типом ошибки
		\param[in] newType - тип ошибки
	*/
	Error(ErrorType newType);
	/*! Конструктор для ошибки в файле
		\param[in] newType - тип ошибки
		\param[in] newErrorFilePath - путь файла
	*/
	Error(ErrorType newType, string newErrorFilePath);
	/*! Конструктор для ошибки в количестве строк
		\param[in] newType - тип ошибки
		\param[in] newExpCount - ожидаемое количество строк
		\param[in] newCount - реальное количество строк
	*/
	Error(ErrorType newType, int newExpCount, int newCount);
	/*! Конструктор для ошибки в количестве элементов в строке
		\param[in] newType - тип ошибки
		\param[in] newExpCount - ожидаемое количество элементов
		\param[in] newCount - реальное количество элементов
		\param[in] newCurrentRow - текущая строка
	*/
	Error(ErrorType newType, int newExpCount, int newCount, int newCurrentRow);	
	/*! Конструктор для ошибки в элементе матрицы
		\param[in] newType - тип ошибки
		\param[in] newPos - позиция элемента
		\param[in] newMatrixElement - элемент матрицы
	*/
	Error(ErrorType newType, ElementPosition newPos, string newMatrixElement);	

	/*! Перегрузка оператора "<"
		\param[in] other - объект для сравнения
		\return - меньше ли левый объект
	*/
	bool operator<(const Error& other) const;
	/*! Перегрузка оператора "=="
		\param[in] other - объект для сравнения
		\return - равны ли объекты
	*/
	bool operator==(const Error& other) const;
};