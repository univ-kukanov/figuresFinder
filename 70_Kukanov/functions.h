#pragma once
#include <set>
#include <vector>
#include "ElementPosition.h"
#include "Error.h"
#include "Figure.h"

const int MAX_MATRIX_DIMENSION = 1000;
const int MAX_MATRIX_ELEMENT_VALUE = 2147483647;
const int MIN_MATRIX_ELEMENT_VALUE = -2147483647 - 1;

void readDataFromFile(const string& filename, vector<string>& lines, set<Error>& errors);

int* parseMatrixData(const vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize);

bool outputDataToFile(const string& filename, vector<string>& output, const set<Error>& errors);

void extractLargestFiguresFromMatrix(const int* matrix, const int numberOfRows, const int numberOfColumns, set<Figure>& figures);

void findFigureInMatrixByGivenElement(const int* matrix, int* passedMatrix, const int row, const int column, const int element, Figure& newFigure, const int numberOfRows, const int numberOfColumns);

void generateOutputMatrix(const set<Figure>& figures, vector<string>& output, const int maxElementSize, const int numberOfRows, const int numberOfColumns);

bool isInIntRange(const string& number);

bool isDimensionInRange(const string& dimension);

bool parseMatrixDimensions(const vector<string>& dimensions, int* numberOfRows, int* numberOfColumns, set<Error>& errors);

void parseMatrixRow(const int currentRow, string line, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound);

void validateMatrixElement(const string element, const int currentRow, const int currentColumn, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound);