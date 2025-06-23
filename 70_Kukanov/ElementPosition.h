#pragma once

//! \brief Класс, представляющий позицию элемента матрицы
class ElementPosition {
private:
	int row = -1;		//!<Строка
	int column = -1;	//!<Столбец
public:
	/*! \brief Установить значение строки
		\param[in] newRow - новое значение строки
	*/
	void setRow(int newRow);
	/*! \brief Установить значение столбца
		\param[in] newColumn - новое значение столбца
	*/
	void setColumn(int newColumn);

	/*! \brief Получить значение строки
		\return значение строки
	*/
	int getRow() const;
	/*! \brief Получить значение столбца
		\return значение столбца
	*/
	int getColumn() const;

	/*! \brief Перегрузка оператора "=="
		\param[in] other - объект для сравнения
		\return - равны ли объекты
	*/
	bool operator==(const ElementPosition& other) const;
	/*! \brief Перегрузка оператора "<"
		\param[in] other - объект для сравнения
		\return - меньше ли левый объект
	*/
	bool operator<(const ElementPosition& other) const;

	/*! \brief Стандартный конструктор */
	ElementPosition();										
	/*! \brief Конструктор с заданной позицией элемента
		\param[in] newRow - значение строки
		\param[in] newColumn - значение столбца
	*/
	ElementPosition(int newRow, int newColumn);
};