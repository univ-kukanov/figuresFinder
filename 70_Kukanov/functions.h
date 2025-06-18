#pragma once
#include <set>
#include <vector>
#include "ElementPosition.h"
#include "Error.h"
#include "Figure.h"


const int MAX_MATRIX_DIMENSION = 1000;					//!<Максимальный размер матрицы
const int MAX_MATRIX_ELEMENT_VALUE = 2147483647;		//!<Максимальный допустимый элемент матрицы
const int MIN_MATRIX_ELEMENT_VALUE = -2147483647 - 1;	//!<Минимальный допустимый элемент матрицы


/*! Функция считывания данных с файла
	\param[in] filename - файл, откуда будут считываться данные
	\param[out] lines - считанные с файла строки
	\param[out] errors - контейнер с ошибками
*/
void readDataFromFile(const string& filename, vector<string>& lines, set<Error>& errors);

/*! Функция обработки считанных из матрицы строк в матрицу
	\param[in] lines - считанные с файла строки
	\param[out] errors - контейнер с ошибками
	\param[out] numberOfRows - количество строк матрицы
	\param[out] numberOfColumns - количество столбцов матрицы
	\param[out] maxElementSize - длина самого длинного элемента матрицы
	\return - указатель на созданную матрицу
*/
int* parseMatrixData(const vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize);

/*! Функция вывода данных в файл
	\param[in] filename - файл, куда будут выводиться данные
	\param[in] output - вектор выводимых строк
	\param[in] errors - контейнер с ошибками
	\return - удачно ли создан файл
*/
bool outputDataToFile(const string& filename, vector<string>& output, const set<Error>& errors);

/*! Функия выделения наибольших фигур из матрицы
	\param[in] matrix - матрица с числами
	\param[in] numberOfRows - количество столбцов матрицы
	\param[in] numberOfColumns - количество строк матрицы
	\param[out] figures - найденные наибольшие фигуры
*/
void extractLargestFiguresFromMatrix(const int* matrix, const int numberOfRows, const int numberOfColumns, set<Figure>& figures);

/*! Рекурсивная функция поиска фигуры по заданному элементу
	\param[in] matrix - исходная матрица
	\param[in,out] passedMatrix - матрица с пройденными элементами
	\param[in] row - строка с текущим элементом
	\param[in] column - столбец с текущим элементом
	\param[in] element - значение текущего элемента
	\param[out] newFigure - новая фигура
	\param[in] numberOfRows - количество строк в матрице
	\param[in] numberOfColumns - количество столбцов в матрице
*/
void findFigureInMatrixByGivenElement(const int* matrix, int* passedMatrix, const int row, const int column, const int element, Figure& newFigure, const int numberOfRows, const int numberOfColumns);

/*! Функция создания данных для вывода в файл
	\param[in] figures - найденные наибольшие фигуры
	\param[out] output - вектор строк для вывода в файл
	\param[in] maxElementSize - максимальная длина элемента матрицы
	\param[in] numberOfRows - количество строк матрицы
	\param[in] numberOfColumns - количество столбцов матрицы
*/
void generateOutputMatrix(const set<Figure>& figures, vector<string>& output, const int maxElementSize, const int numberOfRows, const int numberOfColumns);

/*! Функция проверки элемента на принадлежность к разрешенному диапазону(int)
	\param[in] number - проверяющийся элемент
	\return - принадлежит ли элемент к разрешенному диапазону
*/
bool isInIntRange(const string& number);

/*! Функция проверки размера матрицы на принадлежность к разрешенному диапазону
	\param[in] dimension - проверяющийся размер
	\return - принадлежит ли размер к разрешенному диапазону
*/
bool isDimensionInRange(const string& dimension);

/*! Функция обработки размеров матрицы
	\param[in] dimensions - вектор с размерами матрицы
	\param[out] numberOfRows - количество строк
	\param[out] numberOfColumns - количество столбцов
	\param[out] errors - контейнер с ошибками
	\return - наличие ошибок
*/
bool parseMatrixDimensions(const vector<string>& dimensions, int* numberOfRows, int* numberOfColumns, set<Error>& errors);

/*! Функция обработки строки матрицы
	\param[in] currentRow - номер текущей строки(начинается с 1)
	\param[in] line - строка для обработки
	\param[in] numberOfColumns - количество столбцов
	\param[in,out] maxElementSize - текущая максимальная длина элемента матрицы
	\param[out] matrix - матрица
	\param[out] errors - контейнер с ошибками
	\param[in,out] isErrorFound - была ли найдена ошибка
*/
void parseMatrixRow(const int currentRow, const string& line, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound);

/*! Функция проверки элемента матрицы на наличие ошибок
	\param[in] element - элемент для проверки
	\param[in] currentRow - номер текущей строки(начинается с 1)
	\param[in] currentColumn - номер текущего столбца(начинается с 1)
	\param[in] numberOfColumns - количество столбцов
	\param[in,out] maxElementSize - текущая максимальная длина элемента матрицы
	\param[out] matrix - матрица
	\param[out] errors - контейнер с ошибками
	\param[in,out] isErrorFound - была ли найдена ошибка
*/
void validateMatrixElement(const string& element, const int currentRow, const int currentColumn, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound);

/*! Функция создания сообщения об ошибке для вывода в консоль
	\param[in] inputData - вектор строк с данными из файла
	\param[in] errors - контейнер с ошибками
	\return - строка с сообщением об ошибке (при отсутствии ошибок возвращает пустую строку)
*/
string composeErrorOutput(const vector<string>& inputData, const set<Error>& errors);