#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
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
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        Error error;
        error.setErrorType(inFileNotExist);
        error.setErrorInputFileWay(filename);
        errors.insert(error);
    }
    else {
        while (getline(inFile, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        inFile.close();
    }
}

int* parseMatrixData(vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize)
{
    istringstream dimensionsStream(lines[0]);
    vector<string> dimensions;
    string dimension;

    while (dimensionsStream >> dimension) {
        dimensions.push_back(dimension);
    }

    if (dimensions.size() != 2) {
        Error error;
        error.setErrorType(incorrectDimensionsCount);
        errors.insert(error);
        return NULL;
    }
    else {
        bool noDigitFlag = false;
        for (const auto& dim : dimensions) {
            for (char symbol : dim) {
                if (!isdigit(symbol)) {
                    noDigitFlag = true;
                }
            }
        }

        if (noDigitFlag) {
            Error error;
            error.setErrorType(matrixElementNotInt);
            errors.insert(error);
            return NULL;
        }
        *numberOfRows = stoi(dimensions[0]);
        *numberOfColumns = stoi(dimensions[1]);

        if (*numberOfRows > 1000 || *numberOfRows <= 0) {
            Error error;
            error.setErrorType(rowCountError);
            errors.insert(error);
            return NULL;
        }
        if (*numberOfColumns > 1000 || *numberOfColumns <= 0) {
            Error error;
            error.setErrorType(columnCountError);
            errors.insert(error);
            return NULL;
        }
    }

    int* matrix = new int[(*numberOfRows) * (*numberOfColumns)];
    *maxElementSize = 0;

    bool isErrorFind = false;
    int currentRow = 0;
    for (auto it = lines.begin() + 1; it != lines.end(); ++it) {
        int currentColumn = 0;
        vector<string> splitElements;
        istringstream elements(*it);
        string element;

        while (elements >> element) {
            splitElements.push_back(element);
        }

        if (splitElements.size() > *numberOfColumns) {
            Error error;
            error.setErrorType(tooManyElements);
            error.setExpColumnCount(*numberOfColumns);
            error.setColumnCount(splitElements.size());

            isErrorFind = true;
        }
        else if (splitElements.size() < *numberOfColumns) {
            Error error;
            error.setErrorType(missingNumberOfElements);
            error.setExpColumnCount(*numberOfColumns);
            error.setColumnCount(splitElements.size());

            isErrorFind = true;
        }
        else {
            for (auto& el : splitElements) {
                currentColumn++;
                for (char symbol : el) {
                    if (!isdigit(symbol) && symbol != '-') {
                        Error error;
                        error.setErrorType(matrixElementNotInt);
                        error.setMatrixElement(el);
                        error.setPos(ElementPosition(currentRow, currentColumn));
                        errors.insert(error);

                        isErrorFind = true;
                    }
                }

                if (!isErrorFind) {
                    if (el.size() > *maxElementSize) {
                        *maxElementSize = el.size();
                    }

                    matrix[currentRow * (*numberOfColumns) + currentColumn] = stoi(el);
                }
            }

            currentRow++;
        }
    }
}

bool outputDataToFile(string filename, vector<string>& output, set<Error>& errors)
{
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        return false;
    }

    if (!errors.empty()) {
        for (auto it = errors.begin(); it != errors.end(); ++it) {
            Error error = *it;
            string errorString = error.generateErrorMessage();
            outFile << errorString << "\n";
        }
    }
    else {
        for (auto it = output.begin(); it != output.end(); ++it) {
            outFile << *it << "\n";
        }
    }

    outFile.close();
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