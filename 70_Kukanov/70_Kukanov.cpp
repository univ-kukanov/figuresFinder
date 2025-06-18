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

    readDataFromFile(inputFilename, inputData, errors);         //!##Считывание данных из файла##

    string errorString = composeErrorOutput(inputData, errors); //!##Создание строки вывода ошибки при ее наличии##

    if (!errorString.empty()) {         //!Если сообщение об ошибке не пустое
        cerr << errorString << endl;    //!Вывести ошибку
        return 1;
    }

    int rows = 0;
    int columns = 0;
    int maxElementSize = 0;

    int* matrix = parseMatrixData(inputData, errors, &rows, &columns, &maxElementSize); //!##Обработать считанные данные##

    if (errors.empty()) {       //!Если обработанные данные не содержат ошибок  
        extractLargestFiguresFromMatrix(matrix, rows, columns, figures);                //!##Найти наибольшие фигуры в матрице## 

        if (!figures.empty()) { //!Если была найдена хотя бы одна фигура 
            generateOutputMatrix(figures, outputData, maxElementSize, rows, columns);   //!##Сформировать данные для вывода в файл##
        }
        else {                  //!Иначе 
            outputData.push_back("no figures");                                         //!Добавить в вектор строк для вывода строку “no figure” 
        }
    }

    bool isOutputCompleted = outputDataToFile(outputFilename, outputData, errors);          //!##Вывести данные в файл##
    if (!isOutputCompleted) {       //!Если создание файла для вывода было неудачным 
        cerr << Error(outFileCreateFail, outputFilename).generateErrorMessage() << endl;    //!Вернуть ошибку о неудаче создания файла 
        return 1;
    }

    delete[] matrix;
    return 0;   //!Завершить работу программы 
}

/*! Функция считывания данных с файла
    \param[in] filename - файл, откуда будут считываться данные
    \param[out] lines - считанные с файла строки
    \param[out] errors - контейнер с ошибками
*/
void readDataFromFile(const string& filename, vector<string>& lines, set<Error>& errors)
{
    string line;
    ifstream inFile(filename);  //!Открыть файл 

    if (!inFile.is_open()) {    //!Если файл не был открыт 
        errors.insert(Error(inFileNotExist, filename)); //!Добавить ошибку о неудачном открытии файла 
    }
    else {                      //!Иначе 
        while (getline(inFile, line)) {     //!Пока не достигнут конец файла, cчитать строку 
            if (!line.empty()) {            //!Если строка не пустая 
                lines.push_back(line);      //!Добавить строку в вектор строк 
            }
        }

        inFile.close(); //!Закрыть файл 
    }
}

/*! Функция обработки считанных из матрицы строк в матрицу
    \param[in] lines - считанные с файла строки
    \param[out] errors - контейнер с ошибками
    \param[out] numberOfRows - количество строк матрицы
    \param[out] numberOfColumns - количество столбцов матрицы
    \param[out] maxElementSize - длина самого длинного элемента матрицы
    \return - указатель на созданную матрицу
*/
int* parseMatrixData(const vector<string>& lines, set<Error>& errors, int* numberOfRows, int* numberOfColumns, int* maxElementSize)
{
    //!Разбить первую строку с размерами матрицы по пробелам 
    istringstream dimensionsStream(lines[0]);
    vector<string> dimensions;
    string dimension;
    while (dimensionsStream >> dimension) {
        dimensions.push_back(dimension);
    }

    bool isErrorFound = parseMatrixDimensions(dimensions, numberOfRows, numberOfColumns, errors); //!##Обработка размеров матрицы##

    if (!isErrorFound) {    //!Если ошибок не было найдено
        int* matrix = new int[(*numberOfRows) * (*numberOfColumns)];    //!Инициализировать матрицу заданных размеров
        *maxElementSize = 0;

        if (lines.size() - 1 < *numberOfRows) { //!Если кол-во строк меньше заданного
            errors.insert(Error(tooFewRows, *numberOfRows, lines.size() - 1));  //!Добавить ошибку о слишком маленьком кол-ве строк
            isErrorFound = true;    //!Считать, что ошибка найдена
        }
        else if (lines.size() - 1 > *numberOfRows) {    //!Если кол-во строк больше заданного
            errors.insert(Error(tooManyRows, *numberOfRows, lines.size() - 1)); //!Добавить ошибку о слишком большом кол-ве строк
            isErrorFound = true;    //!Считать, что ошибка найдена
        }

        int currentRow = 0;
        for (auto it = lines.begin() + 1; it != lines.end(); ++it) {    //!Для каждой строки матрицы
            ++currentRow;
            parseMatrixRow(currentRow, *it, *numberOfColumns, maxElementSize, matrix, errors, &isErrorFound);   //!##Обработка строки матрицы##
        }

        if (!isErrorFound) {    //!Если ошибки не были найдены
            return matrix;      //!Вернуть указатель на матрицу
        }
    }


    return nullptr; //!Вернуть nullptr
}

/*! Функция вывода данных в файл
    \param[in] filename - файл, куда будут выводиться данные
    \param[in] output - вектор выводимых строк
    \param[in] errors - контейнер с ошибками
    \return - удачно ли создан файл
*/
bool outputDataToFile(const string& filename, vector<string>& output, const set<Error>& errors)
{
    ofstream outFile(filename); //!Создать и открыть файл для вывода 
    if (!outFile.is_open()) {   //!Если файл не был создан   
        return false;           //!Вернуть ложь (файл не был создан) 
    }

    if (!errors.empty()) {      //!Если ошибки были обнаружены 
        //!Вывести в файл данные об ошибках
        for (const auto& error : errors) {                      //!Для каждой ошибки в errors
            string errorString = error.generateErrorMessage();  //!Создать сообщение об ошибке
            outFile << errorString << "\n";                     //!Вывести сообщение об ошибке в файл
        }
    }
    else {                                  //!Иначе 
        for (const auto& line : output) {   //!Для каждой строки output 
            outFile << line << "\n";        //!Вывести строку в файл	
        }
    }

    outFile.close();    //!Закрыть файл 
    return true;        //!Вернуть истину (файл был создан) 
}

/*! Функия выделения наибольших фигур из матрицы
    \param[in] matrix - матрица с числами
    \param[in] numberOfRows - количество столбцов матрицы
    \param[in] numberOfColumns - количество строк матрицы
    \param[out] figures - найденные наибольшие фигуры
*/
void extractLargestFiguresFromMatrix(const int* matrix, const int numberOfRows, const int numberOfColumns, set<Figure>& figures)
{
    int largestFigureSize = 2;  //!...Считать, что размер наибольшей заданной фигуры равен 2
    int* passedMatrix = new int[numberOfRows * numberOfColumns];    //!...Считать, что ни один элемент матрицы не был пройден
    fill_n(passedMatrix, numberOfRows * numberOfColumns, 0);        //!Заполнение матрицы нулями

    //!Для каждого элемента матрицы 
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            if (*(passedMatrix + i * numberOfColumns + j) == 0) {   //!Если элемент матрицы не был пройден
                Figure newFigure;
                int newElement = *(matrix + i * numberOfColumns + j);
                newFigure.setElementValue(newElement);  //!Создание нового объекта класса фигуры

                findFigureInMatrixByGivenElement(matrix, passedMatrix, i, j, newElement, newFigure, numberOfRows, numberOfColumns); //!##Поиск фигуры с этим элементом (Рекурсия)## 

                int newFigureSize = newFigure.figureSize();
                if (newFigureSize > largestFigureSize) {        //!Если найденная фигура больше текущей наибольшей найденной или текущая наибольшая найденная отсутствует 
                    figures.clear();                            //!Очистить контейнер, содержащий прошлую наибольшую фигуру/фигуры 
                    figures.insert(newFigure);                  //!Добавить в контейнер новую фигуру

                    largestFigureSize = newFigureSize;          //!Обновить размер наибольшей найденной фигуры 
                }
                else if (newFigureSize == largestFigureSize) {  //!Иначе если найденная фигура равна текущей наибольшей найденной
                    figures.insert(newFigure);                  //!Добавить в контейнер дополнительную фигуру 
                }
            }
        }
    }

    delete[] passedMatrix;
}

/*! Рекурсивная функция поиска фигуры по заданному элементу
    \param[in] matrix - исходная матрица
    \param[in,out] passedMatrix - матрица с пройденными элементами
    \param[in] row - строка с текущим элементом
    \param[in] column - столбец с текущим элементом
    \param[in] element - значение текущего элемента
    \param[out] newFigure - новая фигура
    \param[in] numberOfRows - количество строк в матрице
    \param[in] numberOfColumns - количество столбцов в матрице
*/
void findFigureInMatrixByGivenElement(const int* matrix, int* passedMatrix, const int row, const int column, const int element, Figure& newFigure, const int numberOfRows, const int numberOfColumns)
{
    *(passedMatrix + row * numberOfColumns + column) = 1;   //!Считать, что элемент был пройден 
    newFigure.addElement(ElementPosition(row, column));     //!Добавить элемент в фигуру 

    //!Если позиция элемента снизу от текущего не выходит за пределы матрицы (позиция новой строки не больше, чем количество строк в матрице) и элемент не был пройден и элемент является частью фигуры 
    if ((row + 1 < numberOfRows) && (*(passedMatrix + (row + 1) * numberOfColumns + column) == 0) && (*(matrix + (row + 1) * numberOfColumns + column) == element)) {   
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row + 1, column, element, newFigure, numberOfRows, numberOfColumns); //!##Продолжение рекурсии с новым элементом## 
    }

    //!Если позиция элемента сверху от текущего не выходит за пределы матрицы (позиция новой строки не меньше нуля) и элемент не был пройден и элемент является частью фигуры
    if ((row - 1 >= 0) && (*(passedMatrix + (row - 1) * numberOfColumns + column) == 0) && (*(matrix + (row - 1) * numberOfColumns + column) == element)) {  
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row - 1, column, element, newFigure, numberOfRows, numberOfColumns); //!##Продолжение рекурсии с новым элементом## 
    }

    //!Если позиция элемента справа от текущего не выходит за пределы матрицы (позиция нового столбца не больше, чем количество столбцов в матрице) и элемент не был пройден и элемент является частью фигуры 
    if ((column + 1 < numberOfColumns) && (*(passedMatrix + row * numberOfColumns + (column + 1)) == 0) && (*(matrix + row * numberOfColumns + (column + 1)) == element)) { 
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column + 1, element, newFigure, numberOfRows, numberOfColumns); //!##Продолжение рекурсии с новым элементом## 
    }

    //!Если позиция элемента слева от текущего не выходит за пределы матрицы (позиция нового столбца не меньше нуля) и элемент не был пройден и элемент является частью фигуры
    if ((column - 1 >= 0) && (*(passedMatrix + row * numberOfColumns + (column - 1)) == 0) && (*(matrix + row * numberOfColumns + (column - 1)) == element)) {  
        findFigureInMatrixByGivenElement(matrix, passedMatrix, row, column - 1, element, newFigure, numberOfRows, numberOfColumns); //!##Продолжение рекурсии с новым элементом## 
    }
}

/*! Функция создания данных для вывода в файл
    \param[in] figures - найденные наибольшие фигуры
    \param[out] output - вектор строк для вывода в файл
    \param[in] maxElementSize - максимальная длина элемента матрицы
    \param[in] numberOfRows - количество строк матрицы
    \param[in] numberOfColumns - количество столбцов матрицы
*/
void generateOutputMatrix(const set<Figure>& figures, vector<string>& output, const int maxElementSize, const int numberOfRows, const int numberOfColumns)
{
    //!Рассчитать размер разделителя: длину самого длинного элемента матрицы (maxElementSize) умножить на количество столбцов матрицы и прибавить число, на 1 меньше количества столбцов 
    int separatorLength = maxElementSize * numberOfColumns + (numberOfColumns - 1); 
    int figureIndex = 1;
    string separator(separatorLength, '-'); //!Создать строку-разделитель из символов “-”, количество которых равно рассчитанному размеру разделителя 

    for (const auto& figure : figures) {    //!Для каждой фигуры 
        string elementValue = to_string(figure.getElementValue());

        for (int i = 0; i < numberOfRows; i++) {    //!Для каждой строки матрицы 
            string newString;   //!Создать пустую строку 

            for (int j = 0; j < numberOfColumns; j++) { //!Делать numberOfColumns раз 
                if (!figure.isElementInFigure(ElementPosition(i, j))) {     //!Если элемент не принадлежит фигуре 
                    newString += string(elementValue.size(), '*') + " ";    //!Добавить к строке “*” (количество которых равно длине элемента фигуры) и пробел 
                }
                else {                                  //!Иначе 
                    newString += elementValue + " ";    //!Добавить к строке значение элемента и пробел 
                }
            }
            output.push_back(newString);    //!Добавить строку к контейнеру строк на вывод 
        }

        if (figureIndex != figures.size()) {    //!Добавить в контейнер разделитель фигур, если это не последняя фигура 
            output.push_back(separator);
        }

        ++figureIndex;
    }
}

/*! Функция проверки элемента на принадлежность к разрешенному диапазону(int)
    \param[in] number - проверяющийся элемент
    \return - принадлежит ли элемент к разрешенному диапазону
*/
bool isInIntRange(const string& number)
{
    try {
        long long value = stoll(number); //!Перевести элемент из строки в long long
        return (value >= MIN_MATRIX_ELEMENT_VALUE && value <= MAX_MATRIX_ELEMENT_VALUE); //!Проверить на принадлежность разрешенному диапазону и вернуть результат проверки
    }
    catch (const exception&) { 
        return false; //!Вернуть ложь(элемент больше, чем long long)
    }
}

/*! Функция проверки размера матрицы на принадлежность к разрешенному диапазону
    \param[in] dimension - проверяющийся размер
    \return - принадлежит ли размер к разрешенному диапазону
*/
bool isDimensionInRange(const string& dimension) {
    if (isInIntRange(dimension)) { //!Если элемент в диапазоне int
        int size = stoi(dimension); //!Перевести размер из строки в int
        if (size <= MAX_MATRIX_DIMENSION && size > 0) { //!Вернуть истину, если размер матрицы корректный
            return true;
        }
    }
    return false;   //!Вернуть ложь
}

/*! Функция обработки размеров матрицы
    \param[in] dimensions - вектор с размерами матрицы
    \param[out] numberOfRows - количество строк
    \param[out] numberOfColumns - количество столбцов
    \param[out] errors - контейнер с ошибками
    \return - наличие ошибок
*/
bool parseMatrixDimensions(const vector<string>& dimensions, int* numberOfRows, int* numberOfColumns, set<Error>& errors) 
{
    bool isErrorFound = false; //!Флаг наличия ошибок

    if (dimensions.size() != 2) {                       //!Если в размерах матрицы больше двух чисел
        errors.insert(Error(incorrectDimensionsCount)); //!Добавить ошибку о некорректном кол-ве размеров матрицы
        isErrorFound = true;                            //!Считать, что ошибка найдена
    }
    else {                                              //!Иначе
        bool isDigitOnly = true;                        
        for (const auto& dim : dimensions) {            //!Для каждого из размеров (кол-ва строк и столбцов)
            for (const char symbol : dim) {             //!Для каждого символа в значении размера
                if (isDigitOnly && !isdigit(symbol)) {  //!Если до этого встречались только цифры и текущий символ не является цифрой
                    isDigitOnly = false;                //!Считать, что в размере есть отличные от цифр символы
                }
            }
        }

        if (!isDigitOnly) { //!Если в размерах матрицы присутствуют отличные от цифр символы
            errors.insert(Error(matrixSizeNotInt)); //!Добавить ошибку о том, что размеры матрицы не являются числами
            isErrorFound = true;    //!Считать, что ошибка найдена
        }

        if (isDigitOnly && !isDimensionInRange(dimensions[0])) {    //!Если размеры являются числами и кол-во строк не входит в разрешенный диапазон
            errors.insert(Error(rowCountError));    //!Добавить ошибку о некорректном кол-ве строк в матрице
            isErrorFound = true;    //!Считать, что ошибка найдена
        }
        if (isDigitOnly && !isDimensionInRange(dimensions[1])) {    //!Если размеры являются числами и кол-во столбцов не входит в разрешенный диапазон
            errors.insert(Error(columnCountError)); //!Добавить ошибку о некорректном кол-ве столбцов в матрице
            isErrorFound = true;    //!Считать, что ошибка найдена
        }

        if (!isErrorFound) {    //!Если ошибок не было найдено, присвоить значения кол-ва строк и столбцов переменным
            *numberOfRows = stoi(dimensions[0]);
            *numberOfColumns = stoi(dimensions[1]);
        }
    }

    return isErrorFound;    //!Вернуть наличие ошибок
}

/*! Функция обработки строки матрицы
    \param[in] currentRow - номер текущей строки(начинается с 1)
    \param[in] line - строка для обработки
    \param[in] numberOfColumns - количество столбцов
    \param[in,out] maxElementSize - текущая максимальная длина элемента матрицы
    \param[out] matrix - матрица
    \param[out] errors - контейнер с ошибками
    \param[in,out] isErrorFound - была ли найдена ошибка
*/
void parseMatrixRow(const int currentRow, const string& line, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound) 
{
    int currentColumn = 0;
    //!Разбить строку по элементам
    vector<string> splitElements;   
    istringstream elements(line);
    string element;
    while (elements >> element) {
        splitElements.push_back(element);
    }

    if (splitElements.size() > numberOfColumns) {   //!Если в строке больше элементов, чем кол-ва столбцов
        errors.insert(Error(tooManyElements, numberOfColumns, splitElements.size(), currentRow));   //!Добавить ошибку о слишком большом кол-ве элементов в строке
        *isErrorFound = true;   //!Считать, что ошибка найдена
    }
    else if (splitElements.size() < numberOfColumns) {  //!Если в строке меньше элементов, чем кол-ва столбцов
        errors.insert(Error(tooFewElements, numberOfColumns, splitElements.size(), currentRow));    //!Добавить ошибку о слишком маленьком кол-ве элементов в строке
        *isErrorFound = true;   //!Считать, что ошибка найдена
    }

    for (const auto& el : splitElements) {  //!Для каждого элемента в строке
        ++currentColumn;
        validateMatrixElement(el, currentRow, currentColumn, numberOfColumns, maxElementSize, matrix, errors, isErrorFound);    //!##Проверка элемента на наличие ошибок##
    }
}

/*! Функция проверки элемента матрицы на наличие ошибок
    \param[in] element - элемент для проверки
    \param[in] currentRow - номер текущей строки(начинается с 1)
    \param[in] currentColumn - номер текущего столбца(начинается с 1)
    \param[in] numberOfColumns - количество столбцов
    \param[in,out] maxElementSize - текущая максимальная длина элемента матрицы
    \param[out] matrix - матрица
    \param[out] errors - контейнер с ошибками
    \param[in,out] isErrorFound - была ли найдена ошибка
*/
void validateMatrixElement(const string& element, const int currentRow, const int currentColumn, const int numberOfColumns, int* maxElementSize, int* matrix, set<Error>& errors, bool* isErrorFound) 
{
    bool isDigitOnly = true;    
    int currentSymbol = 0;
    for (const char symbol : element) { //!Для каждого символа в значении элемента
        if (isDigitOnly && !isdigit(symbol) && !(currentSymbol == 0 && symbol == '-')) {    //!Если до этого встречались только цифры, текущий символ не является цифрой и это не знак "-" в начале числа
            errors.insert(Error(matrixElementNotInt, ElementPosition(currentRow, currentColumn), element)); //!Добавить ошибку о том, что элемент не является числом

            *isErrorFound = true;   //!Считать, что ошибка найдена
            isDigitOnly = false;    //!Считать, что в значении элемента есть отличные от цифр символы
        }
        ++currentSymbol;
    }

    if (isDigitOnly && !isInIntRange(element)) {    //!Если все элементы являются числами и элемент не входит в разрешенный диапазон значений элементов матрицы
        errors.insert(Error(matrixElementNotInRange, ElementPosition(currentRow, currentColumn), element)); //!Добавить ошибку о некорректном значении элемента матрицы

        *isErrorFound = true;   //!Считать, что ошибка найдена
    }

    if (!*isErrorFound) {   //!Если ошибок не было найдено
        int newElement = stoi(element); 
        if (to_string(newElement).size() > *maxElementSize) {   //!Если длина элемента больше найденной
            *maxElementSize = to_string(newElement).size(); //!Считать данный элемент самым длинным
        }

        matrix[(currentRow - 1) * (numberOfColumns) + (currentColumn - 1)] = newElement;    //!Записать значение элемента в матрицу
    }
}

/*! Функция создания сообщения об ошибке для вывода в консоль
    \param[in] inputData - вектор строк с данными из файла
    \param[in] errors - контейнер с ошибками
    \return - строка с сообщением об ошибке (при отсутствии ошибок возвращает пустую строку)
*/
string composeErrorOutput(const vector<string>& inputData, const set<Error>& errors) 
{
    string errorString = "";
    if (!errors.empty()) {  //!Если обнаружена ошибка открытия файла
        const auto& it = errors.begin();
        errorString = Error(*it).generateErrorMessage();    //!Создать сообщение об ошибке
    }
    else if (inputData.size() == 0) {   //!Если файл пустой
        errorString = Error(inFileIsEmpty).generateErrorMessage();  //!Создать сообщение об ошибке
    }

    return errorString; //!Вернуть сообщение об ошибке
}
