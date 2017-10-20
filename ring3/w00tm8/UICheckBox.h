#include "main.h" 

class UICheckBox {
protected:
	int CoordX;
	int CoordY;
	int Width;
	int Height;
	bool Active;

	bool checkValue;

	Color itemColor;
	Color outlineColor;

	bool xOnce;

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

	int GetWidth()
	{
		return this->Width;
	}

	int GetHeight()
	{
		return this->Height;
	}

	bool GetActive()
	{
		return this->Active;
	}

	char* getText() {
		return this->text;
	}

	Color GetColor() {
		return this->itemColor;
	}

	bool getValve() {
		return this->checkValue;
	}

	void SetCoord(int x, int y) {
		this->CoordX = x;
		this->CoordY = y;
	}

	void SetWidth(int Value)
	{
		this->Width = Value;
	}

	void SetHeight(int Value)
	{
		this->Height = Value;
	}

	void SetActive(bool Value)
	{
		this->Active = Value;
	}

	void SetText(char* Value) {
		this->text = Value;
	}

	void setValue(bool Value) {
		this->checkValue = Value;
	}
	void initColor() {
		this->itemColor = Color(24, 24, 24, 255);
		this->outlineColor = Color(60, 60, 60, 255);
	}

	void onMouseMove()
	{
		if (CursorInArea(this->CoordX, this->CoordY, this->Width, this->Height)) {
			this->outlineColor = Color(130, 130, 130, 255);
		}
		else
		{
			this->outlineColor = Color(60, 60, 60, 255);
		}
	}

	void onMouseClick() {
		if (CursorInArea(this->CoordX, this->CoordY, this->Width, this->Height)) {
			this->checkValue = !this->checkValue;
		}
	}

	void render() {
		if (this->xOnce == false) {
			this->Width = 20;
			this->Height = 20;
			this->Active = true;
			this->itemColor = Color(24, 24, 24, 255);
			this->outlineColor = Color(60, 60, 60, 255);
			this->xOnce = true;
		}
		if (this->Active) {
			FillRGB(this->CoordX + 1, this->CoordY + 1, this->Width - 1, this->Height - 1, this->itemColor);
			if (this->checkValue) {
				FillRGB(this->CoordX + 1, this->CoordY + 1, this->Width - 1, this->Height - 1, themeColor);
			}
			DrawBox(this->CoordX, this->CoordY, this->Width, this->Height, this->outlineColor);
			DrawString(this->CoordX + this->Width + 20, this->CoordY, Color(255, 255, 255, 255), pFont, this->text);
		}
	}
};