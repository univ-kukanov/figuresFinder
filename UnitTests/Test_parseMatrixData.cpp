
#include "pch.h"
#include "CppUnitTest.h"
#include "../70_Kukanov/ElementPosition.h"
#include "../70_Kukanov/Error.h"
#include "../70_Kukanov/Figure.h"
#include "../70_Kukanov/functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

string getErrorDetails(const Error& error)
{
    const string errorTypeToString[] = {
    "noError",
    "rowCountError",
    "columnCountError",
    "incorrectDimensionsCount",
    "tooFewElements",
    "tooFewRows",
    "tooManyElements",
    "tooManyRows",
    "matrixSizeNotInt",
    "matrixElementNotInt",
    "matrixElementNotInRange",
    "inFileNotExist",
    "outFileCreateFail",
    "inFileIsEmpty"
    };

    string errorDetails = "Type = " + errorTypeToString[error.getErrorType()] + "; expRowCount = " + to_string(error.getExpRowCount()) + "; expColumnCount = " + to_string(error.getExpColumnCount())
        + "; rowCount = " + to_string(error.getRowCount()) + "; columnCount = " + to_string(error.getColumnCount()) + "; currentRow = " + to_string(error.getCurrentRow())
        + "; matrixElement = " + error.getMatrixElement() + "; pos = (" + to_string(error.getPos().getRow()) + ", " + to_string(error.getPos().getColumn())
        + "); errorInputFilePath = " + error.getErrorInputFilePath() + "; errorOutputFilePath = " + error.getErrorOutputFilePath() + ";";
    return errorDetails;
}

void assertMatrixData(const int* matrix, const set<Error>& errors, int numberOfRows, int numberOfColumns, int maxElementSize, const int* exp_matrix, const set<Error>& exp_errors, int exp_numberOfRows, int exp_numberOfColumns, int exp_maxElementSize)
{
    wstringstream wss;
    bool isErrorFound = false;

    // Проверка соответствия кол-ва строк
    if (exp_numberOfRows != numberOfRows) {
        wss << L"Incorrect number of rows. Expected: " << exp_numberOfRows
            << L", Actual: " << numberOfRows << L"\n";
        isErrorFound = true;
    }

    // Проверка соответствия кол-ва столбцов
    if (exp_numberOfColumns != numberOfColumns) {
        wss << L"Incorrect number of columns. Expected: " << exp_numberOfColumns
            << L", Actual: " << numberOfColumns << L"\n";
        isErrorFound = true;
    }

    // Проверка соответствия максимального размера элемента
    if (exp_maxElementSize != maxElementSize) {
        wss << L"Incorrect maximum element size. Expected: " << exp_maxElementSize
            << L", Actual: " << maxElementSize << L"\n";
        isErrorFound = true;
    }

    // Проверка ошибок
    if (errors.size() != exp_errors.size()) {
        wss << L"Incorrect number of errors. Expected: " << exp_errors.size()
            << L", Actual: " << errors.size() << L"\n";

        // Поиск отсутствующих ошибок
        if (!errors.empty()) {
            wss << L"Missing expected errors:\n";
            for (const auto& exp_error : exp_errors) {
                if (errors.find(exp_error) == errors.end()) {
                    wss << L"- " << getErrorDetails(exp_error).c_str() << L"\n";
                }
            }
        }

        // Поиск лишних ошибок
        if (!errors.empty()) {
            wss << L"Unexpected errors found:\n";
            for (const auto& error : errors) {
                if (exp_errors.find(error) == exp_errors.end()) {
                    wss << L"- " << getErrorDetails(error).c_str() << L"\n";
                }
            }
        }

        Assert::Fail(wss.str().c_str());
        return;
    }

    // Сравнение ошибок
    if (!equal(exp_errors.begin(), exp_errors.end(), errors.begin())) {
        wss << L"Errors don't match:\n";

        // Поиск ожидаемых, но ненайденных ошибок
        for (const auto& exp_error : exp_errors) {
            if (errors.find(exp_error) == errors.end()) {
                wss << L"Expected but not found: " << getErrorDetails(exp_error).c_str() << L"\n";
            }
        }

        // Поиск найденных ошибок, которых не было в ожидаемых
        for (const auto& act_error : errors) {
            if (exp_errors.find(act_error) == exp_errors.end()) {
                wss << L"Unexpected error found: " << getErrorDetails(act_error).c_str() << L"\n";
            }
        }
        Assert::Fail(wss.str().c_str());
        return;
    }

    if (isErrorFound) {
        Assert::Fail(wss.str().c_str());
    }

    // Сравнение матрицы
    if (exp_matrix != nullptr && matrix != nullptr) {
        for (int i = 0; i < exp_numberOfRows; ++i) {
            for (int j = 0; j < exp_numberOfColumns; ++j) {
                const int index = i * exp_numberOfColumns + j;
                if (exp_matrix[index] != matrix[index]) {
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
    else if (exp_matrix != matrix) {
        Assert::Fail(L"Matrix pointer mismatch");
    }
}



namespace Test_parseMatrixData
{
    TEST_CLASS(Test_parseMatrixData)
    {
    public:
        // 1. Все данные корректны, матрица квадратного размера 5x5 с однозначными положительными числами
        TEST_METHOD(baseTest)
        {
            vector<string> lines = {
                "5 5",
                "2 2 3 4 5",
                "2 2 5 2 2",
                "4 2 2 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            int exp_maxElementSize = 1;
            int exp_matrix[exp_numberOfColumns * exp_numberOfRows] = {
                2, 2, 3, 4, 5,
                2, 2, 5, 2, 2,
                4, 2, 2, 2, 2,
                1, 0, 2, 2, 1,
                7, 8, 4, 6, 5,
            };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 2. Все данные корректны, матрица прямоугольного размера 3x7 с однозначными положительными числами
        TEST_METHOD(correctRectangleMatrix3x7)
        {
            vector<string> lines = {
                "3 7",
                "2 2 3 4 5 2 2",
                "5 2 2 4 2 2 2",
                "2 1 0 2 2 1 7"
            };

            const int exp_numberOfRows = 3;
            const int exp_numberOfColumns = 7;
            const int exp_maxElementSize = 1;
            const int exp_matrix[exp_numberOfRows * exp_numberOfColumns] = {
                2, 2, 3, 4, 5, 2, 2,
                5, 2, 2, 4, 2, 2, 2,
                2, 1, 0, 2, 2, 1, 7
            };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 3. Все данные корректны, матрица прямоугольного размера 7x3 с однозначными положительными числами
        TEST_METHOD(correctRectangleMatrix7x3)
        {
            vector<string> lines = {
                "7 3",
                "2 2 3",
                "4 5 2",
                "2 5 2",
                "2 4 2",
                "2 2 2",
                "1 0 2",
                "2 1 7"
            };

            const int exp_numberOfRows = 7;
            const int exp_numberOfColumns = 3;
            const int exp_maxElementSize = 1;
            const int exp_matrix[exp_numberOfRows * exp_numberOfColumns] = {
                2, 2, 3,
                4, 5, 2,
                2, 5, 2,
                2, 4, 2,
                2, 2, 2,
                1, 0, 2,
                2, 1, 7
            };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 4. Все данные корректны, матрица 5x5 с многозначными, отрицательными элементами и граничными значениями
        TEST_METHOD(matrixWithDifferentNumbers)
        {
            vector<string> lines = {
                "5 5",
                "10 -12 4321 1278635 32",
                "1 333 77777 -77777 -333",
                "-2147483648 2147483647 45367 3214 43",
                "2147483647  -12354 543344 -2 0",
                "2147483647 2147483647 2147483647 234 -1231"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 11;
            const int exp_matrix[exp_numberOfRows * exp_numberOfColumns] = {
                10, -12, 4321, 1278635, 32,
                1, 333, 77777, -77777, -333,
                (-2147483647 - 1), 2147483647, 45367, 3214, 43,
                2147483647, -12354, 543344, -2, 0,
                2147483647, 2147483647, 2147483647, 234, -1231
            };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 5. В строке с размерами только одно число
        TEST_METHOD(sizeLineOneNumber)
        {
            vector<string> lines = {
                "5",
                "2 2 3 4 5",
                "2 2 5 2 2",
                "4 2 2 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(incorrectDimensionsCount) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 6. В строке с размерами 5 чисел
        TEST_METHOD(sizeLineFiveNumbers)
        {
            vector<string> lines = {
                "5 10 432 100 20",
                "2 2 3 4 5",
                "2 2 5 2 2",
                "4 2 2 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(incorrectDimensionsCount) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 7. В строке с размерами кол-во строк не является числом
        TEST_METHOD(rowsNotNumber)
        {
            vector<string> lines = {
                "e 5",
                "2 2 3 4 5",
                "2 2 5 2 2",
                "4 2 2 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(matrixSizeNotInt) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 8. В строке с размерами кол-во столбцов не является числом
        TEST_METHOD(colsNotNumber)
        {
            vector<string> lines = {
                "5 e",
                "2 2 3 4 5",
                "2 2 5 2 2",
                "4 2 2 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(matrixSizeNotInt) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 9. Кол-во строк в матрице больше/меньше разрешенного диапазона
        TEST_METHOD(rowCountOutOfRange)
        {
            vector<string> lines = {
                "1001 5",
                "1 2 3 4 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(rowCountError) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 10. Кол-во столбцов в матрице больше/меньше разрешенного диапазона
        TEST_METHOD(colCountOutOfRange)
        {
            vector<string> lines = {
                "5 1001",
                "1 2 3 4 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(columnCountError) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 11. Кол-во строк и столбцов в матрице больше/меньше разрешенного диапазона
        TEST_METHOD(bothSizesOutOfRange)
        {
            vector<string> lines = {
                "1001 1001",
                "1 2 3 4 5"
            };

            const int exp_numberOfRows = 0;
            const int exp_numberOfColumns = 0;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = {
                Error(rowCountError),
                Error(columnCountError)
            };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 12. Матрица минимального размера 1x1
        TEST_METHOD(minimalMatrix)
        {
            vector<string> lines = {
                "1 1",
                "1"
            };

            const int exp_numberOfRows = 1;
            const int exp_numberOfColumns = 1;
            const int exp_maxElementSize = 1;
            const int exp_matrix[1] = { 1 };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 13. Матрица с 1 строкой 1x5
        TEST_METHOD(oneRowMatrix)
        {
            vector<string> lines = {
                "1 5",
                "1 2 3 4 5"
            };

            const int exp_numberOfRows = 1;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 1;
            const int exp_matrix[exp_numberOfRows * exp_numberOfColumns] = { 1, 2, 3, 4, 5 };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 14. Матрица с 1 столбцом 5x1
        TEST_METHOD(oneColMatrix)
        {
            vector<string> lines = {
                "5 1",
                "1",
                "2",
                "3",
                "4",
                "5"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 1;
            const int exp_maxElementSize = 1;
            const int exp_matrix[exp_numberOfRows * exp_numberOfColumns] = { 1, 2, 3, 4, 5 };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 15. Некоторые элементы матрицы вне диапазона или не являются числом, несколько ошибок
        TEST_METHOD(matrixElementsOutOfRangeOrNotInt)
        {
            vector<string> lines = {
                "5 5",
                "2 2- dsf 4 5",
                "2 -2147483649 5 -99999999999 2",
                "4 2147483648 2 2 2",
                "1 e-0 2 2 1",
                "7 8 4 6 99999999999"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 1;
            set<Error> exp_errors = {
                Error(matrixElementNotInt, ElementPosition(1, 2), "2-"),
                Error(matrixElementNotInt, ElementPosition(1, 3), "dsf"),
                Error(matrixElementNotInt, ElementPosition(4, 2), "e-0"),
                Error(matrixElementNotInRange, ElementPosition(2, 2), "-2147483649"),
                Error(matrixElementNotInRange, ElementPosition(2, 4), "-99999999999"),
                Error(matrixElementNotInRange, ElementPosition(3, 2), "2147483648"),
                Error(matrixElementNotInRange, ElementPosition(5, 5), "99999999999")
            };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 16. Недостающее/избыточное кол-во элементов в строках, пустая строка
        TEST_METHOD(tooFewAndTooManyElementsInRows)
        {
            vector<string> lines = {
                "5 5",
                "2 2 3",
                "2 2 5 2 2 4 5",
                " ",
                "1 0 2 2 2",
                "7 8 4 6 5 5"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = {
                Error(tooFewElements, 5, 3, 1),
                Error(tooFewElements, 5, 0, 3),
                Error(tooManyElements, 5, 7, 2),
                Error(tooManyElements, 5, 6, 5)
            };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 17. Недостающее количество строк
        TEST_METHOD(tooFewRowsInMatrix)
        {
            vector<string> lines = {
                "5 5",
                "2 2 3 4 5",
                "2 2 5 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(tooFewRows, 5, 4) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 18. Наличие в строке букв других языков
        TEST_METHOD(lettersFromOtherLanguagesInMatrix)
        {
            vector<string> lines = {
                "5 5",
                "2 2 3 4 5",
                "2 2 テスト 2 2",
                "1 0 2 2 1",
                "7 8 4 6 5",
                "5 5 5 5 5"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 1;
            set<Error> exp_errors = {
                Error(matrixElementNotInt, ElementPosition(2, 3), "テスト")
            };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 19. В матрице слишком много строк
        TEST_METHOD(tooManyRowsInMatrix)
        {
            vector<string> lines = {
                "2 2",
                "1 2",
                "2 1",
                "1 2",
                "2 1"
            };

            const int exp_numberOfRows = 2;
            const int exp_numberOfColumns = 2;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = { Error(tooManyRows, 2, 4) };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }

        // 20. Размеры матрицы и элементы имеют ведущие нули
        TEST_METHOD(matrixSizeAndElementsWithLeadingZeros)
        {
            vector<string> lines = {
                "05 05",
                "02 2 3 4 5",
                "2 2 05 -2 -02",
                "4 -2 -2 -02 -02",
                "1 0 -2 -2 1",
                "7 8 4 6 5"
            };

            const int exp_numberOfRows = 5;
            const int exp_numberOfColumns = 5;
            const int exp_maxElementSize = 2;
            const int exp_matrix[exp_numberOfRows * exp_numberOfColumns] = {
                2, 2, 3, 4, 5,
                2, 2, 5, -2, -2,
                4, -2, -2, -2, -2,
                1, 0, -2, -2, 1,
                7, 8, 4, 6, 5
            };
            set<Error> exp_errors = {};

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, exp_matrix, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);

            delete[] matrix;
        }

        // 21. Комплексный тест с различными ошибками
        TEST_METHOD(complexTest)
        {
            vector<string> lines = {
                "2 2",
                "-10 айу",
                "-9999999999",
                "-32- 3301112201 22"
            };

            const int exp_numberOfRows = 2;
            const int exp_numberOfColumns = 2;
            const int exp_maxElementSize = 0;
            set<Error> exp_errors = {
                Error(tooFewElements, 2, 1, 2),
                Error(tooManyElements, 2, 3, 3),
                Error(tooManyRows, 2, 3),
                Error(matrixElementNotInt, ElementPosition(1, 2), "айу"),
                Error(matrixElementNotInt, ElementPosition(3, 1), "-32-"),
                Error(matrixElementNotInRange, ElementPosition(2, 1), "-9999999999"),
                Error(matrixElementNotInRange, ElementPosition(3, 2), "3301112201")
            };

            set<Error> errors;
            int numberOfRows = 0;
            int numberOfColumns = 0;
            int maxElementSize = 0;

            int* matrix = parseMatrixData(lines, errors, &numberOfRows, &numberOfColumns, &maxElementSize);

            assertMatrixData(matrix, errors, numberOfRows, numberOfColumns, maxElementSize, nullptr, exp_errors, exp_numberOfRows, exp_numberOfColumns, exp_maxElementSize);
        }
    };
}
