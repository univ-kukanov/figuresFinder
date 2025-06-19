#pragma once

//!Класс, представляющий позицию элемента матрицы
class ElementPosition {
private:
	int row = -1;		//!<Строка
	int column = -1;	//!<Столбец
public:
	/*! Установить значение строки
		\param[in] newRow - новое значение строки
	*/
	void setRow(int newRow);
	/*! Установить значение столбца
		\param[in] newColumn - новое значение столбца
	*/
	void setColumn(int newColumn);

	/*! Получить значение строки
		\return значение строки
	*/
	int getRow() const;
	/*! Получить значение столбца
		\return значение столбца
	*/
	int getColumn() const;

	/*! Перегрузка оператора "=="
		\param[in] other - объект для сравнения
		\return - равны ли объекты
	*/
	bool operator==(const ElementPosition& other) const;
	/*! Перегрузка оператора "<"
		\param[in] other - объект для сравнения
		\return - меньше ли левый объект
	*/
	bool operator<(const ElementPosition& other) const;

	/*! Стандартный конструктор */
	ElementPosition();										
	/*! Конструктор с заданной позицией элемента
		\param[in] newRow - значение строки
		\param[in] newColumn - значение столбца
	*/
	ElementPosition(int newRow, int newColumn);
};