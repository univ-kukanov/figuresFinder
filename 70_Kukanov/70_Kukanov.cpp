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
        cerr << "Error: Expected 3 arguments. Usage: program.exe ./input.txt ./output.txt" << endl;
        return 1;
    }

    string inputFilename = argv[1];     //Считать из параметров командной строки путь к входному файлу 
    string outputFilename = argv[2];    //Считать из параметров командной строки путь для выходных файлов 
    vector<string> inputData;
    vector<string> outputData;
    set<Error> errors;
    set<Figure> figures;

    readDataFromFile(inputFilename, inputData, errors);         //##Считывание данных из файла##

    string errorString = composeErrorOutput(inputData, errors); //##Создание строки вывода ошибки при ее наличии##

    if (!errorString.empty()) {         //Если сообщение об ошибке не пустое
        cerr << errorString << endl;    //Вывести ошибку
        return 1;
    }

    int rows = 0;
    int columns = 0;
    int maxElementSize = 0;

    int* matrix = parseMatrixData(inputData, errors, &rows, &columns, &maxElementSize); //##Обработать считанные данные##

    if (errors.empty()) {       //Если обработанные данные не содержат ошибок  
        extractLargestFiguresFromMatrix(matrix, rows, columns, figures);                //##Найти наибольшие фигуры в матрице## 

        if (!figures.empty()) { //Если была найдена хотя бы одна фигура 
            generateOutputMatrix(figures, outputData, maxElementSize, rows, columns);   //##Сформировать данные для вывода в файл##
        }
        else {                  //Иначе 
            outputData.push_back("no figures");                                         //Добавить в вектор строк для вывода строку “no figure” 
        }
    }

    bool isOutputCompleted = outputDataToFile(outputFilename, outputData, errors);          //##Вывести данные в файл##
    if (!isOutputCompleted) {       //Если создание файла для вывода было неудачным 
        cerr << Error(outFileCreateFail, outputFilename).generateErrorMessage() << endl;    //Вернуть ошибку о неудаче создания файла 
        return 1;
    }

    return 0;   //Завершить работу программы 
}

void readDataFromFile(const string& filename, vector<string>& lines, set<Error>& errors)
{
    string line;
    ifstream inFile(filename);  //Открыть файл 

    if (!inFile.is_open()) {    //Если файл не был открыт 
        errors.insert(Error(inFileNotExist, filename)); //Добавить ошибку о неудачном открытии файла 
    }
    else {                      //Иначе 
        while (getline(inFile, line)) {     //Пока не достигнут конец файла, cчитать строку 
            if (!line.empty()) {            //Если строка не пустая 
                lines.push_back(line);      //Добавить строку в вектор строк 
            }
        }

        inFile.close(); //Закрыть файл 
    }
}

int* parseMatrixData(const vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize)
{
    istringstream dimensionsStream(lines[0]);
    vector<string> dimensions;
    string dimension;

    while (dimensionsStream >> dimension) {
        dimensions.push_back(dimension);
    }

    bool isErrorFound = parseMatrixDimensions(dimensions, numberOfRows, numberOfColumns, errors);

    if (!isErrorFound) {
        int* matrix = new int[(*numberOfRows) * (*numberOfColumns)];
        *maxElementSize = 0;

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
            ++currentRow;
            parseMatrixRow(currentRow, *it, *numberOfColumns, maxElementSize, matrix, errors, &isErrorFound);
        }

        if (!isErrorFound) {
            return matrix;
        }
    }

    return nullptr;
}

bool outputDataToFile(const string& filename, vector<string>& output, const set<Error>& errors)
{
    ofstream outFile(filename); //Создать и открыть файл для вывода 
    if (!outFile.is_open()) {   //Если файл не был создан   
        return false;           //Вернуть ложь (файл не был создан) 
    }

    if (!errors.empty()) {      //Если ошибки были обнаружены 
        //Вывести в файл данные об ошибках
        for (const auto& error : errors) {                      //Для каждой ошибки в errors
            string errorString = error.generateErrorMessage();  //Создать сообщение об ошибке
            outFile << errorString << "\n";                     //Вывести сообщение об ошибке в файл
        }
    }
    else {                                  //Иначе 
        for (const auto& line : output) {   //Для каждой строки output 
            outFile << line << "\n";        //Вывести строку в файл	
        }
    }

    outFile.close();    //Закрыть файл 
    return true;        //Вернуть истину (файл был создан) 
}

void extractLargestFiguresFromMatrix(const int* matrix, const int numberOfRows, const int numberOfColumns, set<Figure>& figures)
{
    int largestFigureSize = 2;  //...Считать, что размер наибольшей заданной фигуры равен 2
    int* passedMatrix = new int[numberOfRows * numberOfColumns];    //...Считать, что ни один элемент матрицы не был пройден
    fill_n(passedMatrix, numberOfRows * numberOfColumns, 0);        //Заполнение матрицы нулями

    //Для каждого элемента матрицы 
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            if (*(passedMatrix + i * numberOfColumns + j) == 0) {   //Если элемент матрицы не был пройден
                Figure newFigure;
                int newElement = *(matrix + i * numberOfColumns + j);
                newFigure.setElementValue(newElement);  //Создание нового объекта класса фигуры

                findFigureInMatrixByGivenElement(matrix, passedMatrix, i, j, newElement, newFigure, numberOfRows, numberOfColumns); //##Поиск фигуры с этим элементом (Рекурсия)## 

                int newFigureSize = newFigure.figureSize();
                if (newFigureSize > largestFigureSize) {        //Если найденная фигура больше текущей наибольшей найденной или текущая наибольшая найденная отсутствует 
                    figures.clear();                            //Очистить контейнер, содержащий прошлую наибольшую фигуру/фигуры 
                    figures.insert(newFigure);                  //Добавить в контейнер новую фигуру

                    largestFigureSize = newFigureSize;          //Обновить размер наибольшей найденной фигуры 
                }
                else if (newFigureSize == largestFigureSize) {  //Иначе если найденная фигура равна текущей наибольшей найденной
                    figures.insert(newFigure);                  //Добавить в контейнер дополнительную фигуру 
                }
            }
        }
    }
}

void findFigureInMatrixByGivenElement(const int* matrix, int* passedMatrix, const int row, const int column, const int element, Figure& newFigure, const int numberOfRows, const int numberOfColumns)
{
    *(passedMatrix + row * numberOfColumns + column) = 1;   //Считать, что элемент был пройден 
    newFigure.addElement(ElementPosition(row, column));     //Добавить элемент в фигуру 

    //Если позиция элемента снизу от текущего не выходит за пределы матрицы (позиция новой строки не больше, чем количество строк в матрице) и элемент не был пройден и элемент является частью фигуры 
    if ((row + 1 < numberOfRows) && (*(passedMatrix + (row + 1) * numberOfColumns + column) == 0) && (*(matrix + (row + 1) * numberOfColumns + column) == element)) {   
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row + 1, column, element, newFigure, numberOfRows, numberOfColumns); //##Продолжение рекурсии с новым элементом## 
    }

    //Если позиция элемента сверху от текущего не выходит за пределы матрицы (позиция новой строки не меньше нуля) и элемент не был пройден и элемент является частью фигуры
    if ((row - 1 >= 0) && (*(passedMatrix + (row - 1) * numberOfColumns + column) == 0) && (*(matrix + (row - 1) * numberOfColumns + column) == element)) {  
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row - 1, column, element, newFigure, numberOfRows, numberOfColumns); //##Продолжение рекурсии с новым элементом## 
    }

    //Если позиция элемента справа от текущего не выходит за пределы матрицы (позиция нового столбца не больше, чем количество столбцов в матрице) и элемент не был пройден и элемент является частью фигуры 
    if ((column + 1 < numberOfColumns) && (*(passedMatrix + row * numberOfColumns + (column + 1)) == 0) && (*(matrix + row * numberOfColumns + (column + 1)) == element)) { 
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column + 1, element, newFigure, numberOfRows, numberOfColumns); //##Продолжение рекурсии с новым элементом## 
    }

    //Если позиция элемента слева от текущего не выходит за пределы матрицы (позиция нового столбца не меньше нуля) и элемент не был пройден и элемент является частью фигуры
    if ((column - 1 >= 0) && (*(passedMatrix + row * numberOfColumns + (column - 1)) == 0) && (*(matrix + row * numberOfColumns + (column - 1)) == element)) {  
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column - 1, element, newFigure, numberOfRows, numberOfColumns); //##Продолжение рекурсии с новым элементом## 
    }
}

void generateOutputMatrix(const set<Figure>& figures, vector<string>& output, const int maxElementSize, const int numberOfRows, const int numberOfColumns)
{
    //Рассчитать размер разделителя: длину самого длинного элемента матрицы (maxElementSize) умножить на количество столбцов матрицы и прибавить число, на 1 меньше количества столбцов 
    int separatorLength = maxElementSize * numberOfColumns + (numberOfColumns - 1); 
    int figureIndex = 1;
    string separator(separatorLength, '-'); //Создать строку-разделитель из символов “-”, количество которых равно рассчитанному размеру разделителя 

    for (const auto& figure : figures) {    //Для каждой фигуры 
        string elementValue = to_string(figure.getElementValue());

        for (int i = 0; i < numberOfRows; i++) {    //Для каждой строки матрицы 
            string newString;   //Создать пустую строку 

            for (int j = 0; j < numberOfColumns; j++) { //Делать numberOfColumns раз 
                if (!figure.isElementInFigure(ElementPosition(i, j))) {     //Если элемент не принадлежит фигуре 
                    newString += string(elementValue.size(), '*') + " ";    //Добавить к строке “*” (количество которых равно длине элемента фигуры) и пробел 
                }
                else {                                  //Иначе 
                    newString += elementValue + " ";    //Добавить к строке значение элемента и пробел 
                }
            }
            output.push_back(newString);    //Добавить строку к контейнеру строк на вывод 
        }

        if (figureIndex != figures.size()) {    //Добавить в контейнер разделитель фигур, если это не последняя фигура 
            output.push_back(separator);
        }

        ++figureIndex;
    }
}

bool isInIntRange(const string& number)
{
    try {
        long long value = stoll(number); //Перевести элемент из строки в long long
        return (value >= MIN_MATRIX_ELEMENT_VALUE && value <= MAX_MATRIX_ELEMENT_VALUE); //Проверить на принадлежность разрешенному диапазону и вернуть результат проверки
    }
    catch (const exception&) { 
        return false; //Вернуть ложь(элемент больше, чем long long)
    }
}

bool isDimensionInRange(const string& dimension) {
    if (isInIntRange(dimension)) { //Если элемент в диапазоне int
        int size = stoi(dimension); //Перевести размер из строки в int
        if (size <= MAX_MATRIX_DIMENSION && size > 0) { //Вернуть истину, если размер матрицы корректный
            return true;
        }
    }
    return false;   //Вернуть ложь
}

bool parseMatrixDimensions(const vector<string>& dimensions, int* numberOfRows, int* numberOfColumns, set<Error>& errors) 
{
    bool isErrorFound = false;

    if (dimensions.size() != 2) {
        errors.insert(Error(incorrectDimensionsCount));
        isErrorFound = true;
    }
    else {
        bool isDigitOnly = true;
        for (const auto& dim : dimensions) {
            for (const char symbol : dim) {
                if (isDigitOnly && !isdigit(symbol)) {
                    isDigitOnly = false;
                }
            }
        }

        if (!isDigitOnly) {
            errors.insert(Error(matrixSizeNotInt));
            isErrorFound = true;
        }

        if (isDigitOnly && !isDimensionInRange(dimensions[0])) {
            errors.insert(Error(rowCountError));
            isErrorFound = true;
        }
        if (isDigitOnly && !isDimensionInRange(dimensions[1])) {
            errors.insert(Error(columnCountError));
            isErrorFound = true;
        }

        if (!isErrorFound) {
            *numberOfRows = stoi(dimensions[0]);
            *numberOfColumns = stoi(dimensions[1]);
        }
    }

    return isErrorFound;
}

void parseMatrixRow(const int currentRow, const string& line, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound) 
{
    int currentColumn = 0;
    vector<string> splitElements;
    istringstream elements(line);
    string element;

    while (elements >> element) {
        splitElements.push_back(element);
    }

    if (splitElements.size() > numberOfColumns) {
        errors.insert(Error(tooManyElements, numberOfColumns, splitElements.size(), currentRow));
        *isErrorFound = true;
    }
    else if (splitElements.size() < numberOfColumns) {
        errors.insert(Error(tooFewElements, numberOfColumns, splitElements.size(), currentRow));
        *isErrorFound = true;
    }

    for (const auto& el : splitElements) {
        ++currentColumn;
        validateMatrixElement(el, currentRow, currentColumn, numberOfColumns, maxElementSize, matrix, errors, isErrorFound);
    }
}

void validateMatrixElement(const string& element, const int currentRow, const int currentColumn, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound) 
{
    bool isDigitOnly = true;
    int currentSymbol = 0;
    for (const char symbol : element) {
        if (isDigitOnly && !isdigit(symbol) && !(currentSymbol == 0 && symbol == '-')) {
            errors.insert(Error(matrixElementNotInt, ElementPosition(currentRow, currentColumn), element));

            *isErrorFound = true;
            isDigitOnly = false;
        }
        ++currentSymbol;
    }

    if (isDigitOnly && !isInIntRange(element)) {
        errors.insert(Error(matrixElementNotInRange, ElementPosition(currentRow, currentColumn), element));

        *isErrorFound = true;
    }

    if (!*isErrorFound) {
        int newElement = stoi(element);
        if (to_string(newElement).size() > *maxElementSize) {
            *maxElementSize = to_string(newElement).size();
        }

        matrix[(currentRow - 1) * (numberOfColumns) + (currentColumn - 1)] = newElement;
    }
}

string composeErrorOutput(const vector<string>& inputData, const set<Error>& errors) 
{
    string errorString = "";
    if (!errors.empty()) {  //Если обнаружена ошибка
        const auto& it = errors.begin();
        errorString = Error(*it).generateErrorMessage();    //Создать сообщение об ошибке
    }
    else if (inputData.size() == 0) {   //Если файл пустой
        errorString = Error(inFileIsEmpty).generateErrorMessage();  //Создать сообщение об ошибке
    }

    return errorString; //Вернуть сообщение об ошибке
}

// 4. написать комментарии