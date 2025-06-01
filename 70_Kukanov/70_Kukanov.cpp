#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include "ElementPosition.h"
#include "Error.h"
#include "Figure.h"

using namespace std;

int main()
{
    std::cout << "Hello World!\n";
}

void readDataFromFile(string filename, vector<string>& lines, set<Error>& errors)
{
    string line;
    ifstream input(filename);

    if (!input.is_open()) {
        Error error;
        error.setErrorType(inFileNotExist);
        error.setErrorInputFileWay(filename);
    }
    else {
        while (getline(input, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        input.close();
    }
}

int* parseMatrixData(vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize)
{
    return NULL;
}

bool outputDataToFile(string filename, vector<string>& output, set<Error>& errors)
{
    return true;
}

void extractLargestFiguresFromMatrix(int* matrix, int numberOfRows, int	numberOfColumns, set<Figure>& figures)
{
    int largestFigureSize = 2;
    int* passedMatrix = new int[numberOfRows * numberOfColumns];
    fill_n(passedMatrix, numberOfRows * numberOfColumns, 0);

    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            if (*(passedMatrix + i * numberOfColumns + j) == 0) {
                Figure newFigure;
                int newElement = *(matrix + i * numberOfColumns + j);

                findFigureInMatrixByGivenElement(matrix, passedMatrix, i, j, newElement, newFigure, numberOfRows, numberOfColumns);

                int newFigureSize = newFigure.figureSize();
                if (newFigureSize > largestFigureSize) {
                    figures.clear();
                    figures.insert(newFigure);

                    largestFigureSize = newFigureSize;
                }
                else if (newFigureSize == largestFigureSize) {
                    figures.insert(newFigure);
                }
            }
        }
    }
}

void findFigureInMatrixByGivenElement(int* matrix, int* passedMatrix, int row, int column, int element, Figure& newFigure, int numberOfRows, int numberOfColumns)
{
    *(passedMatrix + row * numberOfColumns + column) = 1;
    newFigure.addElement(ElementPosition(row, column));

    if ((row + 1 < numberOfRows) && (*(passedMatrix + (row + 1) * numberOfColumns + column) == 0) && (*(matrix + (row + 1) * numberOfColumns + column) == element)) {
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row + 1, column, element, newFigure, numberOfRows, numberOfColumns);
    }

    if ((row - 1 >= 0) && (*(passedMatrix + (row - 1) * numberOfColumns + column) == 0) && (*(matrix + (row - 1) * numberOfColumns + column) == element)) {
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row - 1, column, element, newFigure, numberOfRows, numberOfColumns);
    }

    if ((column + 1 < numberOfColumns) && (*(passedMatrix + row * numberOfColumns + (column + 1)) == 0) && (*(matrix + row * numberOfColumns + (column + 1)) == element)) {
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column + 1, element, newFigure, numberOfRows, numberOfColumns);
    }

    if ((column - 1 >= numberOfColumns) && (*(passedMatrix + row * numberOfColumns + (column - 1)) == 0) && (*(matrix + row * numberOfColumns + (column - 1)) == element)) {
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column - 1, element, newFigure, numberOfRows, numberOfColumns);
    }
}

void generateOutputMatrix(set<Figure>& figures, vector<string>& output, int maxElementSize, int numberOfRows, int numberOfColumns)
{
    int separatorLength = maxElementSize * numberOfColumns + (numberOfColumns - 1);
    string separator(separatorLength, '-');

    for (auto it = figures.begin(); it != figures.end(); ++it) {
        Figure currentFigure = *it;
        string currentElementValue = to_string(currentFigure.getElementValue());

        for (int i = 0; i < numberOfRows; i++) {
            string newString;

            for (int j = 0; j < numberOfColumns; j++) {
                if (!currentFigure.isElementInFigure(ElementPosition(i, j))) {
                    newString += "* ";
                }
                else {
                    newString += currentElementValue;
                }
            }
            output.push_back(newString);
        }

        if (next(it) != figures.end()) {
            output.push_back(separator);
        }
    }
}