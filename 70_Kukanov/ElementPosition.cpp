#include "ElementPosition.h"

void ElementPosition::setRow(int newRow) {
	row = newRow;
}

void ElementPosition::setColumn(int newColumn) {
	column = newColumn;
}

int ElementPosition::getRow() {
	return row;
}

int ElementPosition::getColumn() {
	return column;
}

ElementPosition::ElementPosition() {

}

ElementPosition::ElementPosition(int newRow, int newColumn) {
	row = newRow;
	column = newColumn;
}