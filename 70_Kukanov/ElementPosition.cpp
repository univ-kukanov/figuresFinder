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
	return (row == other.row && column == other.column);
}

bool ElementPosition::operator<(const ElementPosition& other) const {
	if (row != other.row) {
		return row < other.row;
	}
	return column < other.column;
}

ElementPosition::ElementPosition() {

}

ElementPosition::ElementPosition(int newRow, int newColumn) {
	row = newRow;
	column = newColumn;
}