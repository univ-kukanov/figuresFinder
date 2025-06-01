#pragma once

class ElementPosition {
private:
	int row;
	int column;
public:
	void setRow(int newRow);
	void setColumn(int newColumn);

	int getRow();
	int getColumn();

	ElementPosition();
	ElementPosition(int newRow, int newColumn);
};