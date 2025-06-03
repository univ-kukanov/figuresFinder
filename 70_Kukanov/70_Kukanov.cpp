#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include "ElementPosition.h"
#include "Error.h"
#include "Figure.h"
#include "functions.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Error." << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];
    vector<string> inputData;
    vector<string> outputData;
    set<Error> errors;
    set<Figure> figures;

    readDataFromFile(inputFilename, inputData, errors);

    if (!errors.empty()) {
        auto it = errors.begin();
        Error error = *it;
        string errorString = error.generateErrorMessage();
        cerr << errorString << endl;
        return 1;
    }

    if (inputData.size() == 0) {
        Error error(inFileIsEmpty);
        string errorString = error.generateErrorMessage();
        cerr << errorString << endl;
        return 1;
    }

    int rows = 0;
    int columns = 0;
    int maxElementSize = 0;

    int* matrix = parseMatrixData(inputData, errors, &rows, &columns, &maxElementSize);

    if (errors.empty()) {
        extractLargestFiguresFromMatrix(matrix, rows, columns, figures);

        if (!figures.empty()) {
            generateOutputMatrix(figures, outputData, maxElementSize, rows, columns);
        }
        else {
            outputData.push_back("no figures");
        }
    }

    bool isOutputComplieted = outputDataToFile(outputFilename, outputData, errors);
    if (!isOutputComplieted) {
        cerr << Error(outFileCreateFail, outputFilename).generateErrorMessage() << endl;
        return 1;
    }

    return 0;
}

void readDataFromFile(string filename, vector<string>& lines, set<Error>& errors)
{
    string line;
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        errors.insert(Error(inFileNotExist, filename));
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
        errors.insert(Error(incorrectDimensionsCount));
        return NULL;
    }
    else {
        bool isDigitOnly = true;
        for (const auto& dim : dimensions) {
            for (char symbol : dim) {
                if (!isdigit(symbol)) {
                    isDigitOnly = false;
                }
            }
        }

        if (!isDigitOnly) {
            errors.insert(Error(matrixSizeNotInt));
            return NULL;
        }

        if (!isDimensionInRange(dimensions[0])) {
            errors.insert(Error(rowCountError));
            if (!isDimensionInRange(dimensions[1])) {
                errors.insert(Error(columnCountError));
                return NULL;
            }
            return NULL;
        }
        if (!isDimensionInRange(dimensions[1])) {
            errors.insert(Error(columnCountError));
            return NULL;
        }

        *numberOfRows = stoi(dimensions[0]);
        *numberOfColumns = stoi(dimensions[1]);

        if (!errors.empty()) {
            *numberOfRows = 0;
            *numberOfColumns = 0;
            return NULL;
        }
    }

    int* matrix = new int[(*numberOfRows) * (*numberOfColumns)];
    *maxElementSize = 0;

    bool isErrorFound = false;

    if (lines.size() - 1 < *numberOfRows) {
        errors.insert(Error(tooFewRows, *numberOfRows, lines.size() - 1));
        isErrorFound = true;
    }
    else if (lines.size() - 1 > *numberOfRows) {
        errors.insert(Error(tooManyRows, *numberOfRows, lines.size() - 1));
        isErrorFound = true;
    }

    int currentRow = 0;
    for (auto it = lines.begin() + 1; it != lines.end(); ++it) {

        currentRow++;

        int currentColumn = 0;
        vector<string> splitElements;
        istringstream elements(*it);
        string element;

        while (elements >> element) {
            splitElements.push_back(element);
        }

        if (splitElements.size() > *numberOfColumns) {
            errors.insert(Error(tooManyElements, *numberOfColumns, splitElements.size(), currentRow));

            isErrorFound = true;
        }
        else if (splitElements.size() < *numberOfColumns) {
            errors.insert(Error(tooFewElements, *numberOfColumns, splitElements.size(), currentRow));

            isErrorFound = true;
        }

        for (auto& el : splitElements) {
            currentColumn++;
            bool isDigitOnly = true;
            int currentSymbol = 0;
            for (unsigned char symbol : el) {
                if (!isdigit(symbol)) {
                    if (currentSymbol != 0 || symbol != '-') {
                        errors.insert(Error(matrixElementNotInt, ElementPosition(currentRow, currentColumn), el));

                        isErrorFound = true;
                        isDigitOnly = false;
                    }
                }
                currentSymbol++;
            }

            if (isDigitOnly && !isInIntRange(el)) {
                errors.insert(Error(matrixElementNotInRange, ElementPosition(currentRow, currentColumn), el));

                isErrorFound = true;
            }

            if (!isErrorFound) {
                int newElement = stoi(el);
                if (to_string(newElement).size() > *maxElementSize) {
                    *maxElementSize = to_string(newElement).size();
                }

                matrix[(currentRow - 1) * (*numberOfColumns) + (currentColumn - 1)] = newElement;
            }
        }
    }


    if (!errors.empty()) {
        return NULL;
    }

    return matrix;
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
                newFigure.setElementValue(newElement);

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

    if ((column - 1 >= 0) && (*(passedMatrix + row * numberOfColumns + (column - 1)) == 0) && (*(matrix + row * numberOfColumns + (column - 1)) == element)) {
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
                    newString += string(currentElementValue.size(), '*') + " ";
                }
                else {
                    newString += currentElementValue + " ";
                }
            }
            output.push_back(newString);
        }

        if (next(it) != figures.end()) {
            output.push_back(separator);
        }
    }
}

bool isInIntRange(string number)
{
    if (number.size() > 11) {
        return false;
    }

    try {
        long long value = stoll(number);
        return (value >= -2147483648 && value <= 2147483647);
    }
    catch (const exception&) {
        return false;
    }
}

bool isDimensionInRange(string dimension) {
    if (isInIntRange(dimension)) {
        int size = stoi(dimension);
        if (size <= 1000 && size > 0) {
            return true;
        }
    }
    return false;
}