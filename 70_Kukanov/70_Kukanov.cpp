#include <iostream>
#include <set>
#include <vector>
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
            if ((passedMatrix + i * numberOfColumns + j) == 0) {
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

}

void generateOutputMatrix(set<Figure>& figures, vector<string>& output, int maxElementSize, int numberOfRows, int numberOfColumns)
{

}