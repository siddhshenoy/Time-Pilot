#pragma once

#include <string>

#include <Game/Globals.h>

typedef void (*TextOnClickFunction)(std::vector<void*>);

class Text2D
{
public:
	static std::vector<Text2D*> gTextList;
	static void TextHoverChecks(double mX, double mY);
public:
	glm::vec2 mPosition;
	glm::vec2 rPosition;
	glm::vec2 rDimensions;
	glm::vec3 mColor;
	glm::vec3 mOriginalColor;
	std::string mText;
	bool mHasDrawn;
	float mScale;

	bool IsHoverable;
	glm::vec3 mHoverColor;

	bool IsSetVisible;

	TextOnClickFunction OnClickFunction;
	std::vector<void*> mOnClickParams;
public:
	Text2D();
	Text2D(const std::string& text);
	void SetText(const std::string& text);
	void SetTranslation(glm::vec2 position);
	glm::vec2 GetTranslation();
	void SetColor(glm::vec3 color);
	glm::vec3 GetColor();
	glm::vec2 GetTextDimensions();
	void SetScale(float scale);
	float GetScale();
	void Draw();
	void SetHasDrawn(bool set);
	bool HasDrawn();
	
	
	void SetHoverable(bool hoverable);
	void SetHoverColor(glm::vec3 hovercolor);

	void SetVisible(bool set);

	bool HoveringCheck(float mouseX, float mouseY, bool print_debug = false);
	bool MouseClick(float mouseX, float mouseY);
	void SetOnClickFunction(TextOnClickFunction function);
	void AddOnClickParam(void* param);
	void CallOnClickFunction(std::vector<void*> params);
	~Text2D();
};

