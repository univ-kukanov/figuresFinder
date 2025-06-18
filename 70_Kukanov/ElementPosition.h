#pragma once

//!Класс, представляющий позицию элемента матрицы
class ElementPosition {
private:
	int row = -1;		//!<Строка
	int column = -1;	//!<Столбец
public:
	void setRow(int newRow);		//!<Сеттер строки
	void setColumn(int newColumn);	//!<Сеттер столбца

	int getRow() const;				//!<Геттер строки
	int getColumn() const;			//!<Геттер столбца

	bool operator==(const ElementPosition& other) const;	//!<Перегрузка оператора "=="
	bool operator<(const ElementPosition& other) const;		//!<Перегрузка оператора "<"

	ElementPosition();										//!<Стандартный конструктор
	ElementPosition(int newRow, int newColumn);				//!<Конструктор с заданной позицией элемента
};