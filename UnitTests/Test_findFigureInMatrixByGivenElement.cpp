
#include "pch.h"
#include "CppUnitTest.h"
#include "../70_Kukanov/ElementPosition.h"
#include "../70_Kukanov/Error.h"
#include "../70_Kukanov/Figure.h"
#include "../70_Kukanov/functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


void assertFigureAndMatrix(const Figure& newFigure, const Figure& exp_newFigure, const int* matrix, const int* exp_matrix, int numberOfRows, int numberOfColumns) {
    wstringstream wss;
    bool isErrorFound = false;

    // Проверка размеров фигуры
    if (newFigure.figureSize() != exp_newFigure.figureSize()) {
        wss << L"Figure size mismatch. Expected: " << exp_newFigure.figureSize()
            << L", Actual: " << newFigure.figureSize() << L"\n";

        // Поиск отсутствующих элементов
        for (const auto& pos : exp_newFigure.getPositions()) {
            if (!newFigure.isElementInFigure(pos)) {
                wss << L"Missing element: (" << pos.getRow() << L"," << pos.getColumn() << L")\n";
            }
        }

        // Поиск лишних элементов
        for (const auto& pos : newFigure.getPositions()) {
            if (!exp_newFigure.isElementInFigure(pos)) {
                wss << L"Extra element: (" << pos.getRow() << L"," << pos.getColumn() << L")\n";
            }
        }

        isErrorFound = true;
    }

    if (!isErrorFound) {
        // Проверка фигуры по элементам
        // Поиск ожидаемых, но отсутствующих в найденной фигуре элементов
        for (const auto& expPos : exp_newFigure.getPositions()) {
            if (!newFigure.isElementInFigure(expPos)) {
                wss << L"Expected element not found: (" << expPos.getRow()
                    << L"," << expPos.getColumn() << L")\n";
                isErrorFound = true;
            }
        }

        // Поиск элементов, которые не должны быть в найденной фигуре
        for (const auto& actPos : newFigure.getPositions()) {
            if (!exp_newFigure.isElementInFigure(actPos)) {
                wss << L"Unexpected element found: (" << actPos.getRow()
                    << L"," << actPos.getColumn() << L")\n";
                isErrorFound = true;
            }
        }
    }

    // Проверка пройденной матрицы
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < numberOfColumns; ++j) {
            int index = i * numberOfColumns + j;
            if (matrix[index] != exp_matrix[index]) {
                wss << L"Matrix mismatch at position (" << i << L"," << j << L")\n"
                    << L"Expected: " << exp_matrix[index] << L"\n"
                    << L"Actual: " << matrix[index] << L"\n";
                isErrorFound = true;
            }
        }
    }

    if (isErrorFound) {
        Assert::Fail(wss.str().c_str());
        return;
    }
}


namespace Test_findFigureInMatrixByGivenElement
{
    TEST_CLASS(Test_findFigureInMatrixByGivenElement)
    {
    public:
        // 1. Квадратная фигура 3x3 в середине матрицы, входной элемент - верхний левый угол 
        TEST_METHOD(baseTest)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 5;
            int row = 1;
            int column = 1;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = { 4, 2, 5, 1, 6,
                                                           1, 9, 9, 9, 5,
                                                           4, 9, 9, 9, 0,
                                                           7, 9, 9, 9, 2,
                                                           6, 3, 4, 5, 1 };
            set<ElementPosition> positions = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3)
            };
            Figure exp_newFigure(positions, element);
            int exp_passedMatrix[numberOfRows * numberOfColumns] = { 0, 0, 0, 0, 0,
                                                                     0, 1, 1, 1, 0,
                                                                     0, 1, 1, 1, 0,
                                                                     0, 1, 1, 1, 0,
                                                                     0, 0, 0, 0, 0 };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 2. Прямоугольная фигура 3x5 в середине матрицы
        TEST_METHOD(rectangleFigure3x5InCenter)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 7;
            int row = 1;
            int column = 1;
            int element = 5;
            int matrix[numberOfRows * numberOfColumns] = {
                8, 2, 6, 7, 3, 4, 1,
                2, 5, 5, 5, 5, 5, 2,
                3, 5, 5, 5, 5, 5, 6,
                4, 5, 5, 5, 5, 5, 7,
                1, 0, 8, 6, 2, 3, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4), ElementPosition(1, 5),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4), ElementPosition(2, 5),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 5)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 0,
                0, 1, 1, 1, 1, 1, 0,
                0, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 3. Прямоугольная фигура 5x3 в середине матрицы
        TEST_METHOD(rectangleFigure5x3InCenter)
        {
            const int numberOfRows = 7;
            const int numberOfColumns = 5;
            int row = 1;
            int column = 1;
            int element = 4;
            int matrix[numberOfRows * numberOfColumns] = {
                3, 7, 8, 6, 2,
                1, 4, 4, 4, 5,
                2, 4, 4, 4, 3,
                9, 4, 4, 4, 7,
                5, 4, 4, 4, 6,
                8, 4, 4, 4, 1,
                0, 3, 2, 9, 4
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3),
                ElementPosition(4, 1), ElementPosition(4, 2), ElementPosition(4, 3),
                ElementPosition(5, 1), ElementPosition(5, 2), ElementPosition(5, 3)
            };

            Figure exp_newFigure(positions, element);
            exp_newFigure.setElementValue(element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0,
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0,
                0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 4. Фигура в форме "+" в середине матрицы
        TEST_METHOD(plusShapedFigureInCenterStartAtCenter)
        {
            const int numberOfRows = 7;
            const int numberOfColumns = 7;
            int row = 3;
            int column = 3;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                7, 8, 0, 1, 3, 4, 1,
                2, 4, 2, 9, 1, 6, 2,
                3, 1, 3, 9, 4, 5, 3,
                4, 9, 9, 9, 9, 9, 4,
                5, 7, 2, 9, 1, 2, 5,
                7, 6, 3, 9, 5, 1, 6,
                1, 2, 4, 6, 2, 1, 4
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 3),
                ElementPosition(2, 3),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 5),
                ElementPosition(4, 3),
                ElementPosition(5, 3)
            };

            Figure exp_newFigure(positions, 9);
            exp_newFigure.setElementValue(element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 1, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 5. Фигура в форме ромба в середине матрицы
        TEST_METHOD(diamondShapedFigureInCenter)
        {
            const int numberOfRows = 7;
            const int numberOfColumns = 7;
            int row = 1;
            int column = 3;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                7, 8, 0, 1, 3, 4, 1,
                2, 4, 2, 9, 1, 6, 2,
                3, 1, 9, 9, 9, 5, 3,
                4, 9, 9, 9, 9, 9, 4,
                5, 7, 9, 9, 9, 2, 5,
                7, 6, 3, 9, 5, 1, 6,
                1, 2, 4, 6, 2, 1, 4
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 3),
                ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 5),
                ElementPosition(4, 2), ElementPosition(4, 3), ElementPosition(4, 4),
                ElementPosition(5, 3)
            };

            Figure exp_newFigure(positions, element);
            exp_newFigure.setElementValue(element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1, 0, 0, 0,
                0, 0, 1, 1, 1, 0, 0,
                0, 1, 1, 1, 1, 1, 0,
                0, 0, 1, 1, 1, 0, 0,
                0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 6. Фигура-спираль по часовой стрелке
        TEST_METHOD(spiralFigureClockwiseFromTopLeft)
        {
            const int numberOfRows = 10;
            const int numberOfColumns = 10;
            int row = 1;
            int column = 1;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                7, 8, 0, 1, 3, 4, 1, 2, 4, 1,
                2, 9, 9, 9, 9, 9, 9, 9, 9, 0,
                3, 1, 4, 5, 1, 5, 3, 1, 9, 4,
                4, 9, 9, 9, 9, 9, 9, 2, 9, 5,
                5, 9, 6, 2, 1, 2, 9, 0, 9, 1,
                7, 9, 3, 9, 5, 1, 9, 4, 9, 3,
                1, 9, 4, 9, 9, 9, 9, 8, 9, 1,
                1, 9, 3, 4, 5, 6, 7, 4, 9, 2,
                4, 9, 9, 9, 9, 9, 9, 9, 9, 4,
                5, 8, 5, 1, 5, 2, 4, 3, 1, 0
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4), ElementPosition(1, 5), ElementPosition(1, 6), ElementPosition(1, 7), ElementPosition(1, 8),
                ElementPosition(2, 8),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 5), ElementPosition(3, 6),  ElementPosition(3, 8),
                ElementPosition(4, 1), ElementPosition(4, 6), ElementPosition(4, 8),
                ElementPosition(5, 1), ElementPosition(5, 3), ElementPosition(5, 6), ElementPosition(5, 8),
                ElementPosition(6, 1), ElementPosition(6, 3), ElementPosition(6, 4), ElementPosition(6, 5), ElementPosition(6, 6), ElementPosition(6, 8),
                ElementPosition(7, 1), ElementPosition(7, 8),
                ElementPosition(8, 1), ElementPosition(8, 2), ElementPosition(8, 3), ElementPosition(8, 4), ElementPosition(8, 5), ElementPosition(8, 6), ElementPosition(8, 7), ElementPosition(8, 8),
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                0, 1, 1, 1, 1, 1, 1, 0, 1, 0,
                0, 1, 0, 0, 0, 0, 1, 0, 1, 0,
                0, 1, 0, 1, 0, 0, 1, 0, 1, 0,
                0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
                0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 7. Фигура-спираль против часовой стрелки
        TEST_METHOD(spiralFigureCounterClockwiseFromBottomRight)
        {
            const int numberOfRows = 10;
            const int numberOfColumns = 10;
            int row = 8;
            int column = 8;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                7, 8, 0, 1, 3, 4, 1, 2, 4, 1,
                2, 9, 9, 9, 9, 9, 9, 9, 9, 0,
                3, 9, 4, 5, 1, 5, 3, 1, 9, 4,
                4, 9, 2, 9, 9, 9, 9, 2, 9, 5,
                5, 9, 6, 9, 1, 2, 9, 0, 9, 1,
                7, 9, 3, 9, 5, 1, 5, 4, 9, 3,
                1, 9, 4, 9, 9, 9, 9, 9, 9, 1,
                1, 9, 3, 4, 5, 6, 7, 4, 5, 2,
                4, 9, 9, 9, 9, 9, 9, 9, 9, 4,
                5, 8, 5, 1, 5, 2, 4, 3, 1, 0
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4), ElementPosition(1, 5), ElementPosition(1, 6), ElementPosition(1, 7), ElementPosition(1, 8),
                ElementPosition(2, 1), ElementPosition(2, 8),
                ElementPosition(3, 1), ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 5), ElementPosition(3, 6), ElementPosition(3, 8),
                ElementPosition(4, 1), ElementPosition(4, 3), ElementPosition(4, 6), ElementPosition(4, 8),
                ElementPosition(5, 1), ElementPosition(5, 3), ElementPosition(5, 8),
                ElementPosition(6, 1), ElementPosition(6, 3), ElementPosition(6, 4), ElementPosition(6, 5), ElementPosition(6, 6), ElementPosition(6, 7), ElementPosition(6, 8),
                ElementPosition(7, 1),
                ElementPosition(8, 1), ElementPosition(8, 2), ElementPosition(8, 3), ElementPosition(8, 4), ElementPosition(8, 5), ElementPosition(8, 6), ElementPosition(8, 7), ElementPosition(8, 8)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
                0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
                0, 1, 0, 1, 0, 0, 1, 0, 1, 0,
                0, 1, 0, 1, 0, 0, 0, 0, 1, 0,
                0, 1, 0, 1, 1, 1, 1, 1, 1, 0,
                0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 8. Фигура на правой границе матрицы
        TEST_METHOD(figureOnRightEdgeStartTopRight3x5Matrix)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 5;
            int row = 0;
            int column = 4;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                1, 5, 2, 4, 9,
                7, 2, 5, 8, 9,
                0, 3, 7, 3, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 4),
                ElementPosition(1, 4),
                ElementPosition(2, 4)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 1,
                0, 0, 0, 0, 1,
                0, 0, 0, 0, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 9. Фигура на левой границе матрицы
        TEST_METHOD(figureOnLeftEdgeStartBottomLeft5x3Matrix)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 3;
            int row = 4;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 5, 2,
                9, 2, 5,
                9, 3, 7,
                9, 1, 0,
                9, 5, 3
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0),
                ElementPosition(1, 0),
                ElementPosition(2, 0),
                ElementPosition(3, 0),
                ElementPosition(4, 0)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 0, 0,
                1, 0, 0,
                1, 0, 0,
                1, 0, 0,
                1, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 10. Фигура на верхней границе матрицы
        TEST_METHOD(figureOnTopEdgeStartTopLeft3x5Matrix)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 5;
            int row = 0;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 9, 9, 9,
                7, 2, 5, 8, 0,
                0, 3, 7, 3, 1
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(0, 2), ElementPosition(0, 3),
                ElementPosition(0, 4)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 1, 1, 1, 1,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 11. Фигура на нижней границе матрицы
        TEST_METHOD(figureOnBottomEdgeStartBottomRight5x3Matrix)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 3;
            int row = 4;
            int column = 2;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                1, 5, 2,
                4, 2, 5,
                6, 3, 7,
                8, 1, 0,
                9, 9, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(4, 0), ElementPosition(4, 1),
                ElementPosition(4, 2)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0,
                0, 0, 0,
                0, 0, 0,
                0, 0, 0,
                1, 1, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 12. Вся матрица - фигура
        TEST_METHOD(wholeMatrixIsFigure)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 3;
            int row = 2;
            int column = 1;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 9,
                9, 9, 9,
                9, 9, 9,
                9, 9, 9,
                9, 9, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0), ElementPosition(0, 1), ElementPosition(0, 2),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2),
                ElementPosition(2, 0), ElementPosition(2, 1), ElementPosition(2, 2),
                ElementPosition(3, 0), ElementPosition(3, 1), ElementPosition(3, 2),
                ElementPosition(4, 0), ElementPosition(4, 1), ElementPosition(4, 2)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 1, 1,
                1, 1, 1,
                1, 1, 1,
                1, 1, 1,
                1, 1, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 13. Фигура с диагональным спуском (Sigma-образная)
        TEST_METHOD(sigmaShapedFigure)
        {
            const int numberOfRows = 10;
            const int numberOfColumns = 8;
            int row = 0;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 9, 9, 9, 9, 9, 9,
                1, 2, 3, 4, 5, 9, 9, 9,
                4, 5, 2, 2, 9, 9, 9, 0,
                0, 4, 1, 9, 9, 9, 2, 3,
                3, 2, 9, 9, 9, 4, 5, 6,
                2, 1, 9, 9, 9, 7, 8, 0,
                1, 4, 5, 9, 9, 9, 3, 2,
                5, 6, 7, 8, 9, 9, 9, 1,
                1, 2, 3, 4, 5, 9, 9, 9,
                9, 9, 9, 9, 9, 9, 9, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0),ElementPosition(0, 1),ElementPosition(0, 2), ElementPosition(0, 3),ElementPosition(0, 4),ElementPosition(0, 5), ElementPosition(0, 6),ElementPosition(0, 7),
                ElementPosition(1, 5),ElementPosition(1, 6),ElementPosition(1, 7),
                ElementPosition(2, 4),ElementPosition(2, 5),ElementPosition(2, 6),
                ElementPosition(3, 3),ElementPosition(3, 4),ElementPosition(3, 5),
                ElementPosition(4, 2),ElementPosition(4, 3),ElementPosition(4, 4),
                ElementPosition(5, 2),ElementPosition(5, 3),ElementPosition(5, 4),
                ElementPosition(6, 3),ElementPosition(6, 4),ElementPosition(6, 5),
                ElementPosition(7, 4),ElementPosition(7, 5),ElementPosition(7, 6),
                ElementPosition(8, 5),ElementPosition(8, 6),ElementPosition(8, 7),
                ElementPosition(9, 0),ElementPosition(9, 1),ElementPosition(9, 2), ElementPosition(9, 3),ElementPosition(9, 4),ElementPosition(9, 5), ElementPosition(9, 6),ElementPosition(9, 7)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 1, 1, 1, 1, 1, 1, 1,
                0, 0, 0, 0, 0, 1, 1, 1,
                0, 0, 0, 0, 1, 1, 1, 0,
                0, 0, 0, 1, 1, 1, 0, 0,
                0, 0, 1, 1, 1, 0, 0, 0,
                0, 0, 1, 1, 1, 0, 0, 0,
                0, 0, 0, 1, 1, 1, 0, 0,
                0, 0, 0, 0, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 14. Фигура с соседними элементами через один
        TEST_METHOD(horizontalAndVerticalSeparatedFigures)
        {
            const int numberOfRows = 8;
            const int numberOfColumns = 8;
            int row = 3;
            int column = 3;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                1, 6, 2, 9, 9, 3, 2, 1,
                4, 3, 4, 9, 9, 1, 7, 3,
                6, 2, 8, 1, 2, 0, 6, 5,
                9, 9, 1, 9, 9, 5, 9, 9,
                9, 9, 3, 9, 9, 1, 9, 9,
                1, 4, 5, 7, 8, 0, 4, 1,
                5, 2, 4, 9, 9, 1, 3, 8,
                8, 1, 0, 9, 9, 4, 6, 0
            };

            set<ElementPosition> positions = {
                ElementPosition(3, 3),ElementPosition(3, 4),
                ElementPosition(4, 3),ElementPosition(4, 4)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1, 1, 0, 0, 0,
                0, 0, 0, 1, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 15. Фигура с диагональными соседями через один
        TEST_METHOD(diagonalSeparatedFigures)
        {
            const int numberOfRows = 6;
            const int numberOfColumns = 6;
            int row = 2;
            int column = 2;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 5, 2, 9, 9,
                9, 9, 1, 2, 9, 9,
                1, 5, 9, 9, 5, 1,
                8, 3, 9, 9, 1, 0,
                9, 9, 7, 8, 9, 9,
                9, 9, 1, 2, 9, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(2, 2),ElementPosition(2, 3),
                ElementPosition(3, 2),ElementPosition(3, 3)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0,
                0, 0, 1, 1, 0, 0,
                0, 0, 1, 1, 0, 0,
                0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 16. Одиночный элемент (фигуры нет)
        TEST_METHOD(singleElementNoFigure)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 3;
            int row = 1;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                1, 2, 4,
                9, 5, 8,
                1, 3, 0
            };

            set<ElementPosition> positions = {
                ElementPosition(1, 0)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0,
                1, 0, 0,
                0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 17. Диагональный крест без фигуры (один элемент)
        TEST_METHOD(diagonalCrossNoFigureStartAtCenter)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 5;
            int row = 2;
            int column = 2;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 2, 3, 4, 9,
                1, 9, 5, 9, 1,
                2, 3, 9, 3, 2,
                1, 9, 0, 9, 5,
                9, 2, 4, 5, 9
            };

            set<ElementPosition> positions = {
                ElementPosition(2, 2)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 1, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 18. Матрица 1x1
        TEST_METHOD(matrix1x1)
        {
            const int numberOfRows = 1;
            const int numberOfColumns = 1;
            int row = 0;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = { 9 };

            set<ElementPosition> positions = {
                ElementPosition(0, 0)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = { 1 };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 19. Матрица 1x5 (вся матрица - фигура)
        TEST_METHOD(matrix1x5FullFigure)
        {
            const int numberOfRows = 1;
            const int numberOfColumns = 5;
            int row = 0;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = { 9, 9, 9, 9, 9 };

            set<ElementPosition> positions = {
                ElementPosition(0, 0), ElementPosition(0, 1), ElementPosition(0, 2), ElementPosition(0, 3), ElementPosition(0, 4)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = { 1, 1, 1, 1, 1 };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 20. Матрица 5x1 (вся матрица - фигура)
        TEST_METHOD(matrix5x1FullFigure)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 1;
            int row = 0;
            int column = 0;
            int element = 9;
            int matrix[numberOfRows * numberOfColumns] = {
                9,
                9,
                9,
                9,
                9
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0),
                ElementPosition(1, 0),
                ElementPosition(2, 0),
                ElementPosition(3, 0),
                ElementPosition(4, 0)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1,
                1,
                1,
                1,
                1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 21. Максимальное значение элемента (2147483647)
        TEST_METHOD(figureWithMaxValueElement)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 5;
            int row = 0;
            int column = 0;
            int element = 2147483647;
            int matrix[numberOfRows * numberOfColumns] = {
                2147483647, 1145552, 2147483647, 1320, 2147483647,
                2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
                -2, 2147483647, 8, 2147483647, 2147483647,
                50, 2147483647, -4311, 2147483647, 1234,
                2147483647, 2147483647, 2147483647, 2147483647, 2147483647
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0), ElementPosition(0, 2), ElementPosition(0, 4),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4),
                ElementPosition(2, 1), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 1), ElementPosition(3, 3),
                ElementPosition(4, 0), ElementPosition(4, 1), ElementPosition(4, 2), ElementPosition(4, 3), ElementPosition(4, 4)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 0, 1, 0, 1,
                1, 1, 1, 1, 1,
                0, 1, 0, 1, 1,
                0, 1, 0, 1, 0,
                1, 1, 1, 1, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 22. Минимальное значение элемента (-2147483648)
        TEST_METHOD(figureWithMinNegativeValueElement)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 5;
            int row = 0;
            int column = 0;
            int element = (-2147483647 - 1);
            int matrix[numberOfRows * numberOfColumns] = {
                (-2147483647 - 1), 1145552, (-2147483647 - 1), 1320, (-2147483647 - 1),
                (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1),
                -2, (-2147483647 - 1), 8, (-2147483647 - 1), (-2147483647 - 1),
                50, (-2147483647 - 1), -4311, (-2147483647 - 1), 1234,
                (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1)
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0), ElementPosition(0, 2), ElementPosition(0, 4),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2),
                ElementPosition(1, 3), ElementPosition(1, 4),
                ElementPosition(2, 1), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 1), ElementPosition(3, 3),
                ElementPosition(4, 0), ElementPosition(4, 1), ElementPosition(4, 2),
                ElementPosition(4, 3), ElementPosition(4, 4)
            };

            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 0, 1, 0, 1,
                1, 1, 1, 1, 1,
                0, 1, 0, 1, 1,
                0, 1, 0, 1, 0,
                1, 1, 1, 1, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }

        // 23. Большая прямоугольная матрица со сложной фигурой
        TEST_METHOD(complexTest)
        {
            const int numberOfRows = 10;
            const int numberOfColumns = 20;
            int row = 4;
            int column = 7;
            int element = -123;
            int matrix[numberOfRows * numberOfColumns] = {
                -123, -123, -123, 1000, 1000, 1000, 1000, -123, -123, -123,   52,   52, -123, 1000, 1000, 1000, -123, -123, -123, -123,
                -123, -123, -123, 1000, 1000, -123, -123, 1000, -123,   52,   52,   52, -123, -123, 1000, 1000, -123, 1000, 1000, -123,
                -123, 1000, -123, -123, 1000, -123, -123, 1000, -123,   52,   52,   52, -123, -123, -123, -123, -123, -123, 1000, -123,
                1000, 1000, 1000, -123, -123, 1000, 1000, 1000, 1000, -123, 1000, 2434, 1000, -123, -123, 1000, 1000, -123, 1000, -123,
                -123, -123, 1000, 1000, -123, -123, -123, -123, -123, -123, -123,  -274, -123, 1230, -123, -123, 1000, 1000, 1000, 1000,
                1000, -123, 1000, -123, -123, 6573, 6573, -123, -123, -123, -123, 6534, -123, 1230, -123, 1000, 1000, -123, -123, -123,
                -123, -123, 1000, -123, 6573, 6573, -123, -123, -123, 4623, -123,   23, -123, 1230, -123, -123, -123, -123, -123, 1000,
                -123, 1000, -123, -123, 6573, 1000, -123,  846,  -21, 4623, -123, -123, -123, -123, -123, -123, 1000, -123, 1000, 1000,
                -123, -123, -123, -432, 6573, -123, -123, 5234, -123, 4623, -123, -123, -123, -123, 1000, 1000, 1000, -123, -123, -123,
                -123, -123,  465, 6573, 6573, -123,    1,   -1, -123, 4623, 4623, -123, 4623, -123, 1000, 1000, -123, -123, -123, -123
            };

            set<ElementPosition> positions = {
                ElementPosition(0, 0), ElementPosition(0, 1), ElementPosition(0, 2), ElementPosition(0, 12), ElementPosition(0, 16), ElementPosition(0, 17), ElementPosition(0, 18), ElementPosition(0, 19),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 12), ElementPosition(1, 13), ElementPosition(1, 16), ElementPosition(1, 19),
                ElementPosition(2, 0), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 12), ElementPosition(2, 13), ElementPosition(2, 14), ElementPosition(2, 15), ElementPosition(2, 16), ElementPosition(2, 17), ElementPosition(2, 19),
                ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 9), ElementPosition(3, 13), ElementPosition(3, 14), ElementPosition(3, 17), ElementPosition(3, 19),
                ElementPosition(4, 0), ElementPosition(4, 1), ElementPosition(4, 4), ElementPosition(4, 5), ElementPosition(4, 6), ElementPosition(4, 7), ElementPosition(4, 8), ElementPosition(4, 9), ElementPosition(4, 10), ElementPosition(4, 12), ElementPosition(4, 14), ElementPosition(4, 15),
                ElementPosition(5, 1), ElementPosition(5, 3), ElementPosition(5, 4), ElementPosition(5, 7), ElementPosition(5, 8), ElementPosition(5, 9), ElementPosition(5, 10), ElementPosition(5, 12), ElementPosition(5, 14),  ElementPosition(5, 17), ElementPosition(5, 18), ElementPosition(5, 19),
                ElementPosition(6, 0), ElementPosition(6, 1), ElementPosition(6, 3), ElementPosition(6, 6), ElementPosition(6, 7), ElementPosition(6, 8), ElementPosition(6, 10), ElementPosition(6, 12), ElementPosition(6, 14), ElementPosition(6, 15), ElementPosition(6, 16), ElementPosition(6, 17), ElementPosition(6, 18),
                ElementPosition(7, 0), ElementPosition(7, 2), ElementPosition(7, 3), ElementPosition(7, 6), ElementPosition(7, 10), ElementPosition(7, 11), ElementPosition(7, 12), ElementPosition(7, 13), ElementPosition(7, 14), ElementPosition(7, 15), ElementPosition(7, 17),
                ElementPosition(8, 0), ElementPosition(8, 1), ElementPosition(8, 2), ElementPosition(8, 5), ElementPosition(8, 6), ElementPosition(8, 10), ElementPosition(8, 11), ElementPosition(8, 12), ElementPosition(8, 13), ElementPosition(8, 17), ElementPosition(8, 18), ElementPosition(8, 19),
                ElementPosition(9, 0), ElementPosition(9, 1), ElementPosition(9, 5), ElementPosition(9, 11), ElementPosition(9, 13), ElementPosition(9, 16), ElementPosition(9, 17), ElementPosition(9, 18), ElementPosition(9, 19)
            };
            Figure exp_newFigure(positions, element);

            int exp_passedMatrix[numberOfRows * numberOfColumns] = {
                1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1,
                1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1,
                0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1,
                1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0,
                0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1,
                1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0,
                1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0,
                1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
                1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1
            };

            int passedMatrix[numberOfRows * numberOfColumns] = { 0 };
            Figure newFigure;
            newFigure.setElementValue(element);
            findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column, element, newFigure, numberOfRows, numberOfColumns);

            assertFigureAndMatrix(newFigure, exp_newFigure, passedMatrix, exp_passedMatrix, numberOfRows, numberOfColumns);
        }
    };
}
