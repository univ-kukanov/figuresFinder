
#include "pch.h"
#include "CppUnitTest.h"
#include "../70_Kukanov/ElementPosition.h"
#include "../70_Kukanov/Error.h"
#include "../70_Kukanov/Figure.h"
#include "../70_Kukanov/functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


void assertOutputEqual(const vector<string>& exp_output, const vector<string>& output) {
    // Проверка кол-ва строк
    if (exp_output.size() != output.size()) {
        wstringstream wss;
        wss << L"Different number of lines! Expected: " << exp_output.size()
            << L", Actual: " << output.size();
        Assert::Fail(wss.str().c_str());
        return;
    }

    // Построчная проверка
    for (int i = 0; i < exp_output.size(); ++i) {
        if (exp_output[i] != output[i]) {
            wstringstream wss;
            wss << L"Error in line " << i << ":\n"
                << L"Expected: \"" << exp_output[i].c_str() << L"\"\n"
                << L"Actual:   \"" << output[i].c_str() << L"\"";
            Assert::Fail(wss.str().c_str());
            return;
        }
    }
}



namespace Test_generateOutputMatrix
{
    TEST_CLASS(Test_generateOutputMatrix)
    {
    public:

        // 1. Одна фигура в квадратной матрице, граничит со всеми краями матрицы
        TEST_METHOD(baseTest)
        {
            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 2), ElementPosition(3, 3),
                ElementPosition(4, 2), ElementPosition(4, 3),
            };
            set<Figure> figures = { Figure(f1, 2) };

            int maxElementSize = 1;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "2 2 * * * ",
                "2 2 2 2 2 ",
                "* 2 2 2 2 ",
                "* * 2 2 * ",
                "* * 2 2 * "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 2. Одна фигура, не граничит с краями матрицы
        TEST_METHOD(oneFigureNotTouchingEdges)
        {
            set<ElementPosition> f1 = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3),
                ElementPosition(2, 1),                     ElementPosition(2, 3),
            };
            set<Figure> figures = { Figure(f1, 2) };

            int maxElementSize = 1;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "* * * * * ",
                "* 2 2 2 * ",
                "* 2 * 2 * ",
                "* * * * * ",
                "* * * * * "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 3. Одна фигура, занимает всю матрицу
        TEST_METHOD(oneFigureFillsWholeMatrix)
        {
            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1), ElementPosition(0, 2), ElementPosition(0, 3), ElementPosition(0, 4),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4),
                ElementPosition(2, 0), ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 0), ElementPosition(3, 1), ElementPosition(3, 2), ElementPosition(3, 3), ElementPosition(3, 4),
                ElementPosition(4, 0), ElementPosition(4, 1), ElementPosition(4, 2), ElementPosition(4, 3), ElementPosition(4, 4),
            };
            set<Figure> figures = { Figure(f1, 2) };

            int maxElementSize = 1;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "2 2 2 2 2 ",
                "2 2 2 2 2 ",
                "2 2 2 2 2 ",
                "2 2 2 2 2 ",
                "2 2 2 2 2 "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 4. Одна фигура, элемент многозначный положительный
        TEST_METHOD(oneFigureWithMultiDigitPositiveElement)
        {
            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 2), ElementPosition(3, 3),
                ElementPosition(4, 2), ElementPosition(4, 3),
            };
            set<Figure> figures = { Figure(f1, 7777) };

            int maxElementSize = 4;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "7777 7777 **** **** **** ",
                "7777 7777 7777 7777 7777 ",
                "**** 7777 7777 7777 7777 ",
                "**** **** 7777 7777 **** ",
                "**** **** 7777 7777 **** "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 5. Одна фигура, элемент многозначный отрицательный
        TEST_METHOD(oneFigureWithMultiDigitNegativeElement)
        {
            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(1, 0), ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3), ElementPosition(1, 4),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4),
                ElementPosition(3, 2), ElementPosition(3, 3),
                ElementPosition(4, 2), ElementPosition(4, 3),
            };
            set<Figure> figures = { Figure(f1, -7777) };

            int maxElementSize = 5;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "-7777 -7777 ***** ***** ***** ",
                "-7777 -7777 -7777 -7777 -7777 ",
                "***** -7777 -7777 -7777 -7777 ",
                "***** ***** -7777 -7777 ***** ",
                "***** ***** -7777 -7777 ***** "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 6. Две фигуры, разная длина элементов, maxElementSize больше длины элементов фигур
        TEST_METHOD(twoFiguresDifferentLengthElementsMatrixHasLongerValues)
        {
            set<ElementPosition> fig1 = {
                ElementPosition(2, 2),
                ElementPosition(3, 2), ElementPosition(3, 3),
                ElementPosition(4, 2), ElementPosition(4, 3),
            };

            set<ElementPosition> fig2 = {
                ElementPosition(0, 1), ElementPosition(0, 2),
                ElementPosition(1, 1), ElementPosition(1, 2),
                ElementPosition(2, 1),
            };

            set<Figure> figures = {
                Figure(fig1, -332),
                Figure(fig2, 2)
            };

            int maxElementSize = 7;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "**** **** **** **** **** ",
                "**** **** **** **** **** ",
                "**** **** -332 **** **** ",
                "**** **** -332 -332 **** ",
                "**** **** -332 -332 **** ",
                "",
                "---------------------------------------",
                "",
                "* 2 2 * * ",
                "* 2 2 * * ",
                "* 2 * * * ",
                "* * * * * ",
                "* * * * * "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 7. Пять фигур, maxElementSize равен длине самого длинного элемента
        TEST_METHOD(fiveFiguresWithEqualMaxElementLength)
        {
            set<ElementPosition> f1 = {
                ElementPosition(1, 4), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4), ElementPosition(3, 2)
            };

            set<ElementPosition> f2 = {
                ElementPosition(1, 1), ElementPosition(1, 2), ElementPosition(1, 3),
                ElementPosition(2, 1),
                ElementPosition(3, 1),
            };

            set<ElementPosition> f3 = {
                ElementPosition(0, 0), ElementPosition(0, 1), ElementPosition(0, 2), ElementPosition(0, 3), ElementPosition(0, 4)
            };

            set<ElementPosition> f4 = {
                ElementPosition(3, 3), ElementPosition(3, 4),
                ElementPosition(4, 2), ElementPosition(4, 3), ElementPosition(4, 4)
            };

            set<ElementPosition> f5 = {
                ElementPosition(1, 0),
                ElementPosition(2, 0),
                ElementPosition(3, 0),
                ElementPosition(4, 0), ElementPosition(4, 1)
            };

            set<Figure> figures = {
                Figure(f1, -22),
                Figure(f2, -3),
                Figure(f3, 5),
                Figure(f4, 10),
                Figure(f5, 444)
            };

            int maxElementSize = 3;
            int numberOfRows = 5;
            int numberOfColumns = 5;

            vector<string> exp_output = {
                "*** *** *** *** *** ",
                "*** *** *** *** -22 ",
                "*** *** -22 -22 -22 ",
                "*** *** -22 *** *** ",
                "*** *** *** *** *** ",
                "",
                "-------------------",
                "",
                "** ** ** ** ** ",
                "** -3 -3 -3 ** ",
                "** -3 ** ** ** ",
                "** -3 ** ** ** ",
                "** ** ** ** ** ",
                "",
                "-------------------",
                "",
                "5 5 5 5 5 ",
                "* * * * * ",
                "* * * * * ",
                "* * * * * ",
                "* * * * * ",
                "",
                "-------------------",
                "",
                "** ** ** ** ** ",
                "** ** ** ** ** ",
                "** ** ** ** ** ",
                "** ** ** 10 10 ",
                "** ** 10 10 10 ",
                "",
                "-------------------",
                "",
                "*** *** *** *** *** ",
                "444 *** *** *** *** ",
                "444 *** *** *** *** ",
                "444 *** *** *** *** ",
                "444 444 *** *** *** "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }

        // 8. Две фигуры в прямоугольной матрице 3x7
        TEST_METHOD(twoFiguresInRectangleMatrix3x7)
        {
            set<ElementPosition> f1 = {
                ElementPosition(0, 0),
                ElementPosition(1, 0), ElementPosition(1, 1),
                ElementPosition(2, 1), ElementPosition(2, 2), ElementPosition(2, 3), ElementPosition(2, 4)
            };

            set<ElementPosition> f2 = {
                ElementPosition(0, 2), ElementPosition(0, 3), ElementPosition(0, 4), ElementPosition(0, 5), ElementPosition(0, 6),
                ElementPosition(1, 6),
                ElementPosition(2, 6),
            };

            set<Figure> figures = {
                Figure(f1, 10),
                Figure(f2, 1234567)
            };

            int maxElementSize = 7;
            int numberOfRows = 3;
            int numberOfColumns = 7;

            vector<string> exp_output = {
                "10 ** ** ** ** ** ** ",
                "10 10 ** ** ** ** ** ",
                "** 10 10 10 10 ** ** ",
                "",
                "-------------------------------------------------------",
                "",
                "******* ******* 1234567 1234567 1234567 1234567 1234567 ",
                "******* ******* ******* ******* ******* ******* 1234567 ",
                "******* ******* ******* ******* ******* ******* 1234567 "
            };

            vector<string> output;
            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }


        // 9. Две фигуры в прямоугольной матрице 7x3
        TEST_METHOD(twoFiguresInRectangleMatrix7x3)
        {
            set<ElementPosition> f1 = {
                ElementPosition(0, 0), ElementPosition(0, 1),
                ElementPosition(1, 0),
                ElementPosition(2, 0),
                ElementPosition(3, 0),
                ElementPosition(4, 0),
                ElementPosition(5, 0)
            };

            set<ElementPosition> f2 = {
                ElementPosition(1, 1),
                ElementPosition(2, 1),
                ElementPosition(3, 1),
                ElementPosition(4, 1), ElementPosition(4, 2),
                ElementPosition(5, 1), ElementPosition(5, 2)
            };

            set<Figure> figures = {
                Figure(f1, 1),
                Figure(f2, 2)
            };

            int maxElementSize = 1;
            int numberOfRows = 7;
            int numberOfColumns = 3;

            vector<string> exp_output = {
                "1 1 * ",
                "1 * * ",
                "1 * * ",
                "1 * * ",
                "1 * * ",
                "1 * * ",
                "* * * ",
                "",
                "-----",
                "",
                "* * * ",
                "* 2 * ",
                "* 2 * ",
                "* 2 * ",
                "* 2 2 ",
                "* 2 2 ",
                "* * * "
            };

            vector<string> output;

            generateOutputMatrix(figures, output, maxElementSize, numberOfRows, numberOfColumns);

            assertOutputEqual(exp_output, output);
        }


    };
}
