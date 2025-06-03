#pragma once
#include <set>
#include "ElementPosition.h"

using namespace std;

class Figure {
private:
	set<ElementPosition> positions;
	int element;
public:
	void addElement(ElementPosition pos);
	int figureSize() const;
	bool isElementInFigure(ElementPosition pos) const;
	void setElementValue(int newElement);
	int getElementValue() const;
	const set<ElementPosition>& getPositions() const;

	Figure(set<ElementPosition> pos, int el);
	Figure();

	bool operator<(const Figure& other) const;
	bool operator==(const Figure& other) const;
};