#include <sstream>
#include "pch.h"
#include "CppUnitTest.h"
#include "../70_Kukanov/ElementPosition.h"
#include "../70_Kukanov/Error.h"
#include "../70_Kukanov/Figure.h"
#include "../70_Kukanov/functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// Функция форматирования набора позиций
wstring formatPositions(const set<ElementPosition>& positions) {
    wstringstream wss;
    for (const auto& pos : positions) {
        wss << "(" << pos.getRow() << "," << pos.getColumn() << ") ";
    }
    return wss.str();
}

// Функция поиска фигуры по позиции
const Figure* findFigureByPosition(const set<Figure>& figures, const ElementPosition& pos) {
    for (const auto& fig : figures) {
        if (fig.isElementInFigure(pos)) {
            return &fig;
        }
    }
    return NULL;
}

// Сравнения фигур
void assertFiguresEqual(const set<Figure>& exp_figures, const set<Figure>& figures) {
    wstringstream wss;
    // Проверка количества фигур
    if (exp_figures.size() != figures.size()) {
        wss << "Different number of figures. Expected: " << exp_figures.size() << ", Actual: " << figures.size() << "\n";

        // Поиск отсутствующих фигур
        for (const auto& expFig : exp_figures) {
            bool found = false;
            for (const auto& actFig : figures) {
                if (expFig == actFig) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                wss << "Missing figure with value " << expFig.getElementValue()
                    << " and positions: " << formatPositions(expFig.getPositions()) << "\n";
            }
        }

        // Поиск лишних фигур
        for (const auto& actFig : figures) {
            bool found = false;
            for (const auto& expFig : exp_figures) {
                if (actFig == expFig) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                wss << "Extra figure with value " << actFig.getElementValue()
                    << " and positions: " << formatPositions(actFig.getPositions()) << "\n";
            }
        }

        Assert::Fail(wss.str().c_str());
    }

    bool isErrorFound = false;

    // Проверка значений элементов и позиций
    for (const auto& expFig : exp_figures) {
        bool equal = false;
        for (const auto& actFig : figures) {
            if (expFig.getPositions() == actFig.getPositions()) {
                equal = true;

                // Проверка значения элемента
                if (expFig.getElementValue() != actFig.getElementValue()) {
                    wss << "Figure at positions " << formatPositions(expFig.getPositions())
                        << "has wrong element value. Expected: " << expFig.getElementValue()
                        << ", Actual: " << actFig.getElementValue() << "\n";
                    isErrorFound = true;
                }
                break;
            }
        }

        if (!equal) {
            isErrorFound = true;

            // Проверка, какие именно позиции не совпали
            wss << "Could not find exact match for figure with value " << expFig.getElementValue()
                << " and positions: " << formatPositions(expFig.getPositions()) << "\n";

            // Проверка, есть ли позиции этой фигуры в других фигурах
            for (const auto& pos : expFig.getPositions()) {
                const Figure* actFig = findFigureByPosition(figures, pos);
                if (actFig) {
                    wss << "Position (" << pos.getRow() << "," << pos.getColumn()
                        << ") was found in figure with value " << actFig->getElementValue()
                        << " and positions: " << formatPositions(actFig->getPositions()) << "\n";
                }
                else {
                    wss << "Position (" << pos.getRow() << "," << pos.getColumn()
                        << ") was not found in any figure\n";
                }
            }
        }
    }

    if (isErrorFound) {
        Assert::Fail(wss.str().c_str());
    }
}





namespace Test_extractLargestFiguresFromMatrix
{
    TEST_CLASS(Test_extractLargestFiguresFromMatrix)
    {
    public:

        // 1. Одна фигура в середине, квадратная матриц
        TEST_METHOD(baseTest)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 5;
            int matrix[numberOfRows * numberOfColumns] = { 4, 2, 5, 1, 6,
                                                           1, 9, 9, 9, 5,
                                                           4, 9, 9, 9, 0,
                                                           7, 9, 9, 9, 2,
                                                           6, 3, 4, 5, 1 };

            set<ElementPosition> f1 = { ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3),
                                        ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3),
                                        ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3) };
            set<Figure> exp_figures = { Figure(f1, 9) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 2. Одна фигура в середине 3x5, прямоугольная матрица 5x7 
        TEST_METHOD(oneFigureInCenterRectangleMatrix3x5)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 7;
            int matrix[numberOfRows * numberOfColumns] = {
                8, 2, 6, 7, 3, 4, 1,
                2, 5, 5, 5, 5, 5, 2,
                3, 5, 5, 5, 5, 5, 6,
                4, 5, 5, 5, 5, 5, 7,
                1, 0, 8, 6, 2, 3, 9
            };


            set<ElementPosition> f1 = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4), ElementPosition(1, 5),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4), ElementPosition(2, 5),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4), ElementPosition(3, 5)
            };
            set<Figure> exp_figures = { Figure(f1, 5) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }


        // 3. Одна фигура в середине 5x3, прямоугольная матрица 7x5
        TEST_METHOD(oneFigureInCenterRectangleMatrix5x3)
        {
            const int numberOfRows = 7;
            const int numberOfColumns = 5;
            int matrix[numberOfRows * numberOfColumns] = {
                3, 7, 8, 6, 2,
                1, 4, 4, 4, 5,
                2, 4, 4, 4, 3,
                9, 4, 4, 4, 7,
                5, 4, 4, 4, 6,
                8, 4, 4, 4, 1,
                0, 3, 2, 9, 4
            };


            set<ElementPosition> f1 = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3),
                ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3),
                ElementPosition(4, 1), ElementPosition(4, 2), ElementPosition(4, 3),
                ElementPosition(5, 1), ElementPosition(5, 2), ElementPosition(5, 3)
            };
            set<Figure> exp_figures = { Figure(f1, 4) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 4. Несколько одинаковых по размеру фигур 
        TEST_METHOD(multipleSameSizeFigures)
        {
            const int numberOfRows = 6;
            const int numberOfColumns = 6;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 5, 2, 9, 9,
                9, 9, 1, 2, 9, 9,
                1, 5, 9, 9, 5, 1,
                8, 3, 9, 9, 1, 0,
                9, 9, 7, 8, 9, 9,
                9, 9, 1, 2, 9, 9
            };

            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(1, 0), ElementPosition(1, 1)
            };
            set<ElementPosition> f2 = {
                ElementPosition(0, 4), ElementPosition(0, 5),
                ElementPosition(1, 4), ElementPosition(1, 5)
            };
            set<ElementPosition> f3 = {
                ElementPosition(2, 2), ElementPosition(2, 3),
                ElementPosition(3, 2), ElementPosition(3, 3)
            };
            set<ElementPosition> f4 = {
                ElementPosition(4, 0), ElementPosition(4, 1),
                ElementPosition(5, 0), ElementPosition(5, 1)
            };
            set<ElementPosition> f5 = {
                ElementPosition(4, 4), ElementPosition(4, 5),
                ElementPosition(5, 4), ElementPosition(5, 5)
            };
            set<Figure> exp_figures = {
                Figure(f1, 9), Figure(f2, 9), Figure(f3, 9), Figure(f4, 9), Figure(f5, 9)
            };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 5. Три фигуры: сначала встречается меньшая, потом наибольшая, потом меньше
        TEST_METHOD(threeFiguresSmallBigSmall)
        {
            const int numberOfRows = 5;
            const int numberOfColumns = 5;
            int matrix[numberOfRows * numberOfColumns] = {
                2, 2, 8, 1, 9,
                3, 3, 0, 9, 9,
                3, 5, 9, 9, 9,
                3, 9, 9, 9, 9,
                3, 9, 9, 1, 0
            };

            set<ElementPosition> f1 = { ElementPosition(0, 4),
                                        ElementPosition(1, 3), ElementPosition(1, 4),
                                        ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4),
                                        ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4),
                                        ElementPosition(4, 1), ElementPosition(4, 2) };
            set<Figure> exp_figures = { Figure(f1, 9) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 6. Матрица минимального размера 1x1
        TEST_METHOD(matrixSize1x1O)
        {
            const int numberOfRows = 1;
            const int numberOfColumns = 1;
            int matrix[numberOfRows * numberOfColumns] = { 9 };

            set<Figure> exp_figures = { };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 7. Матрица полностью из минимальных фигур (2 элемента) с разными значениями
        TEST_METHOD(matrixOfMinimalFiguresWithVariousValues)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 4;
            int matrix[numberOfRows * numberOfColumns] = {
                1,          1,         -1,                 -1,
                2147483647, 412,       -1111,              -1111,
                2147483647, 412,       (-2147483647 - 1), (-2147483647 - 1)  // Корректное представление INT_MIN
            };

            set<ElementPosition> f1 = { ElementPosition(2, 2), ElementPosition(2, 3) };
            set<ElementPosition> f2 = { ElementPosition(1, 2), ElementPosition(1, 3) };
            set<ElementPosition> f3 = { ElementPosition(0, 2), ElementPosition(0, 3) };
            set<ElementPosition> f4 = { ElementPosition(0, 0), ElementPosition(0, 1) };
            set<ElementPosition> f5 = { ElementPosition(1, 1), ElementPosition(2, 1) };
            set<ElementPosition> f6 = { ElementPosition(1, 0), ElementPosition(2, 0) };

            set<Figure> exp_figures = {
                Figure(f1, (-2147483647 - 1)),
                Figure(f2, -1111),
                Figure(f3, -1),
                Figure(f4, 1),
                Figure(f5, 412),
                Figure(f6, 2147483647)
            };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 8. Вся матрица - одна фигура
        TEST_METHOD(wholeMatrixIsOneFigure)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 3;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 9,
                9, 9, 9,
                9, 9, 9
            };

            set<ElementPosition> f1 = { ElementPosition(0, 0), ElementPosition(0, 1), ElementPosition(0, 2),
                                        ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2),
                                        ElementPosition(2, 0), ElementPosition(2, 1), ElementPosition(2, 2) };
            set<Figure> exp_figures = { Figure(f1, 9) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 9. Фигура начинается в левом верхнем углу
        TEST_METHOD(figureStartsInTopLeftCorner)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 3;
            int matrix[numberOfRows * numberOfColumns] = {
                9, 9, 1,
                9, 3, 2,
                6, 5, 4
            };

            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(1, 0)
            };
            set<Figure> exp_figures = { Figure(f1, 9) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 10. Фигура начинается в правом верхнем углу
        TEST_METHOD(figureStartsInTopRightCorner)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 3;
            int matrix[numberOfRows * numberOfColumns] = {
                8, 7, 9,
                0, 9, 9,
                6, 5, 4
            };

            set<ElementPosition> f1 = {
                ElementPosition(0, 2),
                ElementPosition(1, 1), ElementPosition(1, 2)
            };
            set<Figure> exp_figures = { Figure(f1, 9) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 11. Фигура начинается в левом нижнем углу
        TEST_METHOD(figureStartsInBottomLeftCorner)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 3;
            int matrix[numberOfRows * numberOfColumns] = {
                8, 7, 1,
                0, 3, 2,
                9, 9, 9
            };

            set<ElementPosition> f1 = {
                ElementPosition(2, 0), ElementPosition(2, 1), ElementPosition(2, 2)
            };
            set<Figure> exp_figures = { Figure(f1, 9) };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 12. Фигур нет (все элементы уникальны)
        TEST_METHOD(noFiguresInMatrix)
        {
            const int numberOfRows = 3;
            const int numberOfColumns = 3;
            int matrix[numberOfRows * numberOfColumns] = {
                1, 2, 3,
                4, 5, 6,
                7, 8, 9
            };

            set<Figure> exp_figures = {};

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }

        // 13. Комплексный тест с разными фигурами
        TEST_METHOD(complexTest)
        {
            const int numberOfRows = 12;
            const int numberOfColumns = 28;
            int matrix[numberOfRows * numberOfColumns] = {
                0,           0,           0,           0,           0,           0, 0,      0, 9, 234,  234,  234,  234,   234,   0,     0,     0,     -432,  0,     5678, 0,    -2,   -2,   124356,     0,          0,          2147483647, 2147483647,
                0,           9,           9,           9,           9,           9, 0,      9, 9, 9,    9,    0,    -9321, -9321, -9321, -9321, -9321, -9321, -9321, -2,   -2,   0,    0,    2147483647, 2147483647, 2147483647, 2147483647, 52,
                1234,        1243,        4231,        -534,        9,           9, 0,      0, 9, 0,    0,    0,    0,     0,     0,     0,     0,     -2,    -9321, 0,    0,    0,    0,    1,          0,          0,          0,          0,
                0,           0,           0,           9,           9,           0, 0,      9, 9, 9,    0,    0,    0,     0,     -9321, -9321, -2,    -2,    -9321, 33,   -2,   0,    0,    0,          1,          1,          0,          0,
                0,           0,           9,           9,           0,           0, 9,      9, 9, 9,    9,    9,    9,     0,     -9321, 0,     0,     0,     -9321, -477, -477, -477, -1,   -477,       -477,       1,          -477,       -477,
                0,           0,           0,           9,           9,           0, 0,      9, 9, 9,    0,    0,    0,     0,     -9321, 0,     0,     0,     -9321, 0,    0,    -2,   0,    20,         0,          0,          -2,         0,
                0,           0,           0,           0,           9,           9, 569427, 0, 9, 4823, 4823, 4823, 23456, -2,    -9321, -9321, -9321, -9321, -9321, 0,    0,    0,    0,    -2,         0,          0,          0,          0,
                0,           9,           9,           9,           9,           9, 0,      0, 0, 0,    0,    0,    23456, 0,     0,     -2,    -2,    0,     0,     0,    0,    -2,   -477, -477,       -477,       -477,       -477,       -477,
                (-2147483647 - 1), 0,           0,           4823,        0,           0, 9,      9, 9, 9,    9,    0,    23456, 0,     23456, 23456, 23456, 23456, 23456, 0,    0,    -2,   0,    0,          0,          0,          0,          0,
                (-2147483647 - 1), 0,           0,           0,           0,           0, 9,      9, 9, 9,    9,    0,    23456, 0,     23456, 0,     0,     0,     23456, 0,    -2,   0,    0,    0,          0,          1987,       1987,       1987,
                (-2147483647 - 1), (-2147483647 - 1), 0,           0,           0,           0, 9,      9, 9, 9,    9,    0,    23456, 0,     0,     0,     0,     0,     23456, -2,   0,    0,    0,    0,          1987,       1987,       1987,       1987,
                (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), (-2147483647 - 1), 0, 9,      9, 9, 9,    9,    0,    23456, 23456, 23456, 23456, 23456, 23456, 23456, 1,    2,    3,    4,    5,          6,          7,          8,          9
            };

            set<ElementPosition> f1 = {
                ElementPosition(1, 12), ElementPosition(1, 13), ElementPosition(1, 14), ElementPosition(1, 15), ElementPosition(1, 16), ElementPosition(1, 17), ElementPosition(1, 18),
                ElementPosition(2, 18),
                ElementPosition(3, 14), ElementPosition(3, 15), ElementPosition(3, 18),
                ElementPosition(4, 14), ElementPosition(4, 18),
                ElementPosition(5, 14), ElementPosition(5, 18),
                ElementPosition(6, 14), ElementPosition(6, 15), ElementPosition(6, 16), ElementPosition(6, 17), ElementPosition(6, 18)
            };
            set<ElementPosition> f2 = {
                ElementPosition(0, 8),
                ElementPosition(1, 7), ElementPosition(1, 8), ElementPosition(1, 9), ElementPosition(1, 10),
                ElementPosition(2, 8),
                ElementPosition(3, 7), ElementPosition(3, 8), ElementPosition(3, 9),
                ElementPosition(4, 6), ElementPosition(4, 7), ElementPosition(4, 8), ElementPosition(4, 9), ElementPosition(4, 10), ElementPosition(4, 11), ElementPosition(4, 12),
                ElementPosition(5, 7), ElementPosition(5, 8), ElementPosition(5, 9),
                ElementPosition(6, 8)
            };
            set<ElementPosition> f3 = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4), ElementPosition(1, 5),
                ElementPosition(2, 4), ElementPosition(2, 5),
                ElementPosition(3, 3), ElementPosition(3, 4),
                ElementPosition(4, 2), ElementPosition(4, 3),
                ElementPosition(5, 3), ElementPosition(5, 4),
                ElementPosition(6, 4), ElementPosition(6, 5),
                ElementPosition(7, 1), ElementPosition(7, 2), ElementPosition(7, 3), ElementPosition(7, 4), ElementPosition(7, 5)
            };
            set<ElementPosition> f4 = {
                ElementPosition(8, 6), ElementPosition(8, 7), ElementPosition(8, 8), ElementPosition(8, 9), ElementPosition(8, 10),
                ElementPosition(9, 6), ElementPosition(9, 7), ElementPosition(9, 8), ElementPosition(9, 9), ElementPosition(9, 10),
                ElementPosition(10, 6), ElementPosition(10, 7), ElementPosition(10, 8), ElementPosition(10, 9), ElementPosition(10, 10),
                ElementPosition(11, 6), ElementPosition(11, 7), ElementPosition(11, 8), ElementPosition(11, 9), ElementPosition(11, 10)
            };
            set<ElementPosition> f5 = {
                ElementPosition(6, 12),
                ElementPosition(7, 12),
                ElementPosition(8, 12), ElementPosition(8, 14), ElementPosition(8, 15), ElementPosition(8, 16), ElementPosition(8, 17), ElementPosition(8, 18),
                ElementPosition(9, 12), ElementPosition(9, 14), ElementPosition(9, 18),
                ElementPosition(10, 12), ElementPosition(10, 18),
                ElementPosition(11, 12), ElementPosition(11, 13), ElementPosition(11, 14), ElementPosition(11, 15), ElementPosition(11, 16), ElementPosition(11, 17), ElementPosition(11, 18)
            };

            set<Figure> exp_figures = {
                Figure(f1, -9321),
                Figure(f2, 9),
                Figure(f3, 9),
                Figure(f4, 9),
                Figure(f5, 23456)
            };

            set<Figure> figures;
            extractLargestFiguresFromMatrix(matrix, numberOfRows, numberOfColumns, figures);

            assertFiguresEqual(exp_figures, figures);
        }
    };
}
