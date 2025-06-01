#include "Figure.h"

void Figure::addElement(ElementPosition& pos) {
	positions.insert(pos);
}

int Figure::figureSize() {
	return positions.size();
}

bool Figure::isElementInFigure(ElementPosition& pos) {
	for (auto& currentPos : positions) {
		if (pos == currentPos) {
			return true;
		}
	}

	return false;
}

void Figure::setElementValue(int newElement) {
	element = newElement;
}

int Figure::getElementValue() {
	return element;
}