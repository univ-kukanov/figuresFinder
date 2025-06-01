#pragma once
#include <set>
#include "ElementPosition.h"

using namespace std;

class Figure {
private:
	set<ElementPosition> positions;
	int element;
public:
	void addElement(ElementPosition& pos);
	int figureSize();
	bool isElementInFigure(ElementPosition& pos);
	void setElementValue(int newElement);
	int getElementValue();
};