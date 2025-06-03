#include "Figure.h"

Figure::Figure(set<ElementPosition> pos, int el) {
	positions = pos;
	element = el;
}

Figure::Figure() {

}

void Figure::addElement(ElementPosition pos) {
	positions.insert(pos);
}

int Figure::figureSize() const {
	return positions.size();
}

bool Figure::isElementInFigure(ElementPosition pos) const {
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

int Figure::getElementValue() const {
	return element;
}

bool Figure::operator<(const Figure& other) const {
	if (positions.size() != other.positions.size()) {
		return positions.size() < other.positions.size();
	}
	if (element != other.element) {
		return element < other.element;
	}
	return lexicographical_compare(positions.begin(), positions.end(), other.positions.begin(), other.positions.end());
}

bool Figure::operator==(const Figure& other) const {
	return (positions == other.positions && element == other.element);
}

const set<ElementPosition>& Figure::getPositions() const {
	return positions;
}