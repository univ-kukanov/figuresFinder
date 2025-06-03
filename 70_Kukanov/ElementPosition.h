#pragma once

class ElementPosition {
private:
	int row = -1;
	int column = -1;
public:
	void setRow(int newRow);
	void setColumn(int newColumn);

	int getRow() const;
	int getColumn() const;

	bool operator==(const ElementPosition& other) const;
	bool operator<(const ElementPosition& other) const;

	ElementPosition();
	ElementPosition(int newRow, int newColumn);
};