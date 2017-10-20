#include "main.h"
#include <string>

class UIDropdown {
protected:
	int CoordX;
	int CoordY;

	bool xOnce;

	int selectedValue;

	int iAdd = 10;

	const char** items;

	int iAmount;

	bool open;

	Color itemColor;
	Color outlineColor;

	char* text;
public:

	int GetCoordX()
	{
		return this->CoordX;
	}

	int GetCoordY()
	{
		return this->CoordY;
	}

	int getSelectedItem() {
		return this->selectedValue;
	}

	char* getText() {
		return this->text;
	}

	Color GetColor() {
		return this->itemColor;
	}

	void setPos(int x, int y)
	{
		this->CoordX = x; this->CoordY = y;
	}


	void SetText(char* Value) {
		this->text = Value;
	}

	void setItems(const char** itemz, int ammount) {
		this->items = itemz;
		this->iAmount = ammount;
	}

	void initColor() {
		this->itemColor = Color(32, 32, 32, 255);
		this->outlineColor = Color(40, 40, 40, 255);
	}

	void MouseMove() {

	}

	void onMouseHold() {

	}

	void onMouseClick() {
		if (CursorInArea(this->CoordX - 50, this->CoordY, 100, this->iAdd))  {
			this->open = !this->open;
		}
	}

};