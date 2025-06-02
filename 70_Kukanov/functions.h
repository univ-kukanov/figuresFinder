#pragma once
#include <set>
#include <vector>
#include "ElementPosition.h"
#include "Error.h"
#include "Figure.h"

void readDataFromFile(string filename, vector<string>& lines, set<Error>& errors);

int* parseMatrixData(vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize);

bool outputDataToFile(string filename, vector<string>& output, set<Error>& errors);

void extractLargestFiguresFromMatrix(int* matrix, int numberOfRows, int	numberOfColumns, set<Figure>& figures);

void findFigureInMatrixByGivenElement(int* matrix, int* passedMatrix, int row, int column, int element, Figure& newFigure, int numberOfRows, int numberOfColumns);

void generateOutputMatrix(set<Figure>& figures, vector<string>& output, int maxElementSize, int numberOfRows, int numberOfColumns);

bool isInIntRange(string number);