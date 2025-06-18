#pragma once
#include <set>
#include <vector>
#include "ElementPosition.h"
#include "Error.h"
#include "Figure.h"

const int MAX_MATRIX_DIMENSION = 1000;
const int MAX_MATRIX_ELEMENT_VALUE = 2147483647;
const int MIN_MATRIX_ELEMENT_VALUE = -2147483647 - 1;


/*! ������� ���������� ������ � �����
* param[in] filename - ����, ������ ����� ����������� ������
* param[out] lines - ��������� � ����� ������
* param[out] errors - ��������� � ��������
*/
void readDataFromFile(const string& filename, vector<string>& lines, set<Error>& errors);

/*! ������� ��������� ��������� �� ������� ����� � �������
* param[in] lines - ��������� � ����� ������
* param[out] errors - ��������� � ��������
* param[out] numberOfRows - ���������� ����� �������
* param[out] numberOfColumns - ���������� �������� �������
* param[out] maxElementSize - ����� ������ �������� �������� �������
* return - ��������� �� ��������� �������
*/
int* parseMatrixData(const vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize);

/*! ������� ������ ������ � ����
* param[in] filename - ����, ���� ����� ���������� ������
* param[in] output - ������ ��������� �����
* param[in] errors - ��������� � ��������
* return - ������ �� ������ ����
*/
bool outputDataToFile(const string& filename, vector<string>& output, const set<Error>& errors);

/*! ������ ��������� ���������� ����� �� �������
* param[in] matrix - ������� � �������
* param[in] numberOfRows - ���������� �������� �������
* param[in] numberOfColumns - ���������� ����� �������
* param[out] figures - ��������� ���������� ������
*/
void extractLargestFiguresFromMatrix(const int* matrix, const int numberOfRows, const int numberOfColumns, set<Figure>& figures);

/*! ����������� ������� ������ ������ �� ��������� ��������
* param[in] matrix - �������� �������
* param[in|out] passedMatrix - ������� � ����������� ����������
* param[in] row - ������ � ������� ���������
* param[in] column - ������� � ������� ���������
* param[in] element - �������� �������� ��������
* param[out] newFigure - ����� ������
* param[in] numberOfRows - ���������� ����� � �������
* param[in] numberOfColumns - ���������� �������� � �������
*/
void findFigureInMatrixByGivenElement(const int* matrix, int* passedMatrix, const int row, const int column, const int element, Figure& newFigure, const int numberOfRows, const int numberOfColumns);

/*! ������� �������� ������ ��� ������ � ����
* param[in] figures - ��������� ���������� ������
* param[out] output - ������ ����� ��� ������ � ����
* param[in] maxElementSize - ������������ ����� �������� �������
* param[in] numberOfRows - ���������� ����� �������
* param[in] numberOfColumns - ���������� �������� �������
*/
void generateOutputMatrix(const set<Figure>& figures, vector<string>& output, const int maxElementSize, const int numberOfRows, const int numberOfColumns);

/*! ������� �������� �������� �� �������������� � ������������ ���������(int)
* param[in] number - ������������� �������
* return - ����������� �� ������� � ������������ ���������
*/
bool isInIntRange(const string& number);

/*! ������� �������� ������� ������� �� �������������� � ������������ ���������
* param[in] dimension - ������������� ������
* return - ����������� �� ������ � ������������ ���������
*/
bool isDimensionInRange(const string& dimension);

/*! ������� ��������� �������� �������
* param[in] dimensions - ������ � ��������� �������
* param[out] numberOfRows - ���������� �����
* param[out] numberOfColumns - ���������� ��������
* param[out] errors - ��������� � ��������
* return - ������� ������
*/
bool parseMatrixDimensions(const vector<string>& dimensions, int* numberOfRows, int* numberOfColumns, set<Error>& errors);

/*! ������� ��������� ������ �������
* param[in] currentRow - ����� ������� ������(���������� � 1)
* param[in] line - ������ ��� ���������
* param[in] numberOfColumns - ���������� ��������
* param[in|out] maxElementSize - ������� ������������ ����� �������� �������
* param[out] matrix - �������
* param[out] errors - ��������� � ��������
* param[in|out] isErrorFound - ���� �� ������� ������
*/
void parseMatrixRow(const int currentRow, const string& line, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound);

/*! ������� �������� �������� ������� �� ������� ������
* param[in] element - ������� ��� ��������
* param[in] currentRow - ����� ������� ������(���������� � 1)
* param[in] currentColumn - ����� �������� �������(���������� � 1)
* param[in] numberOfColumns - ���������� ��������
* param[in|out] maxElementSize - ������� ������������ ����� �������� �������
* param[out] matrix - �������
* param[out] errors - ��������� � ��������
* param[in|out] isErrorFound - ���� �� ������� ������
*/
void validateMatrixElement(const string& element, const int currentRow, const int currentColumn, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound);

/*! ������� �������� ��������� �� ������ ��� ������ � �������
* param[in] inputData - ������ ����� � ������� �� �����
* param[in] errors - ��������� � ��������
* return - ������ � ���������� �� ������ (��� ���������� ������ ���������� ������ ������)
*/
string composeErrorOutput(const vector<string>& inputData, const set<Error>& errors);