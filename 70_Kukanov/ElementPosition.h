#pragma once

class ElementPosition {
private:
	int row;
	int column;
public:
	void setRow(int newRow);
	void setColumn(int newColumn);

	int getRow() const;
	int getColumn() const;

	bool operator==(const ElementPosition& other) const;

	ElementPosition();
	ElementPosition(int newRow, int newColumn);
};