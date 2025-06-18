#pragma once
#include <set>
#include "ElementPosition.h"

using namespace std;

//!Фигура
class Figure {
private:
	set<ElementPosition> positions;	//!<Позиции элементов фигуры
	int element;					//!<Значение элемента фигуры
public:
	/*! Добавление элемента в фигуру
		\param[in] pos - позиция элемента
	*/
	void addElement(ElementPosition pos);	

	/*! Получение размера фигуры
		\return - размер фигуры
	*/
	int figureSize() const;		

	/*! Проверка принадлежности элемента к фигуре
		\param[in] pos - позиция элемента
		\return - принадлежит ли элемент фигуре
	*/
	bool isElementInFigure(ElementPosition pos) const;

	void setElementValue(int newElement);				//!<Сеттер значения элемента

	int getElementValue() const;						//!<Геттер значения элемента
	const set<ElementPosition>& getPositions() const;	//!<Геттер всех позиций элементов фигуры

	Figure(set<ElementPosition> pos, int el);			//!<Конструктор с заданными позициями элементов и значением элемента
	Figure();											//!<Стандартный конструктор

	bool operator<(const Figure& other) const;			//!<Перегрузка оператора "<"
	bool operator==(const Figure& other) const;			//!<Перегрузка оператора "=="
};