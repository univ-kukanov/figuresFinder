#include "ElementPosition.h"

void ElementPosition::setRow(int newRow) {
	row = newRow;
}

void ElementPosition::setColumn(int newColumn) {
	column = newColumn;
}

int ElementPosition::getRow() const {
	return row;
}

int ElementPosition::getColumn() const {
	return column;
}

bool ElementPosition::operator==(const ElementPosition& other) const {
	return (row == other.getRow() && column == other.getColumn());
}

bool ElementPosition::operator<(const ElementPosition& other) const {
	if (row < other.getRow()) {
		return true;
	}
	else if (row == other.getRow() && column < other.getColumn()) {
		return true;
	}
	return false;
}

ElementPosition::ElementPosition() {

}

ElementPosition::ElementPosition(int newRow, int newColumn) {
	row = newRow;
	column = newColumn;
}