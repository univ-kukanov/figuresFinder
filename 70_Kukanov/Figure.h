#pragma once
#include <set>
#include "ElementPosition.h"

using namespace std;

//! \brief Класс, представляющий фигуру в матрице
class Figure {
private:
	set<ElementPosition> positions;	//!<Позиции элементов фигуры
	int element;					//!<Значение элемента фигуры
public:
	/*! \brief Добавление элемента в фигуру
		\param[in] pos - позиция элемента
	*/
	void addElement(ElementPosition pos);

	/*! \brief Получение размера фигуры
		\return - размер фигуры
	*/
	int figureSize() const;

	/*! \brief Проверка принадлежности элемента к фигуре
		\param[in] pos - позиция элемента
		\return - принадлежит ли элемент фигуре
	*/
	bool isElementInFigure(ElementPosition pos) const;

	/*! \brief Установить значение элемента
		\param[in] newElement - новое значение элемента
	*/
	void setElementValue(int newElement);

	/*! \brief Получить значение элемента
		\return - значение элемента
	*/
	int getElementValue() const;
	/*! \brief Получить контейнер со всеми позициями элементов фигуры
		\return - контейнер со всеми позициями элементов фигуры
	*/
	const set<ElementPosition>& getPositions() const;

	/*! \brief Стандартный конструктор*/
	Figure();
	/*! \brief Конструктор с заданными позициями элементов и значением элемента
		\param[in] pos - контейнер со всеми позициями элементов фигуры
		\param[in] el - значение элемента
	*/
	Figure(set<ElementPosition> pos, int el);

	/*! \brief Перегрузка оператора "<"
		\param[in] other - объект для сравнения
		\return - меньше ли левый объект
	*/
	bool operator<(const Figure& other) const;
	/*! \brief Перегрузка оператора "=="
		\param[in] other - объект для сравнения
		\return - равны ли объекты
	*/
	bool operator==(const Figure& other) const;
};