#include "Text2D.h"

std::vector<Text2D*> Text2D::gTextList;

Text2D::Text2D()
{
	Text2D::gTextList.push_back(this);
	this->mOnClickParams.push_back(static_cast<void*>(this));
	this->OnClickFunction = nullptr;
	this->IsSetVisible = true;
	this->mHasDrawn = false;
}

Text2D::Text2D(const std::string& text)
{
	Text2D::gTextList.push_back(this);
	this->mOnClickParams.push_back(static_cast<void*>(this));
	this->OnClickFunction = nullptr;
	this->IsSetVisible = true;
	this->mHasDrawn = false;
	mText = text;
}
void Text2D::SetText(const std::string& text) {
	mText = text;
}
void Text2D::SetOnClickFunction(TextOnClickFunction function)
{
	this->OnClickFunction = function;
}
void Text2D::SetTranslation(glm::vec2 position) {
	mPosition = position;
}
glm::vec2 Text2D::GetTranslation() {
	return mPosition;
}
void Text2D::SetScale(float scale) {
	mScale = scale;
}
float Text2D::GetScale() {
	return mScale;
}
void Text2D::SetVisible(bool set) {
	this->IsSetVisible = set;
}
void Text2D::SetColor(glm::vec3 color) {
	mColor = color;
	mOriginalColor = color;
}
glm::vec3 Text2D::GetColor() {
	return mColor;
}
void Text2D::Draw() {
	if (Globals::FontRenderer != nullptr) {
		if (this->IsSetVisible) {
			Globals::FontRenderer->RenderText(
				Globals::gScreenWidth,
				Globals::gScreenHeight,
				mText,
				mPosition.x,
				mPosition.y,
				mScale,
				mColor,
				&rPosition,
				&rDimensions
			);
		}
	}
	else {
		std::cout << "Could not draw.." << std::endl;
	}
}
void Text2D::SetHoverable(bool hoverable) {
	IsHoverable = hoverable;
}
void Text2D::SetHoverColor(glm::vec3 hovercolor) {
	mHoverColor = hovercolor;
}
void Text2D::SetHasDrawn(bool set) {
	this->mHasDrawn = set;
}
bool Text2D::HasDrawn() {
	return mHasDrawn;
}
bool Text2D::HoveringCheck(float mouseX, float mouseY, bool print_debug) {
	bool result = false;
	if(print_debug)
		std::cout << "mX : " << mouseX << " mY : " << Globals::gScreenHeight - mouseY << " sX: " << rPosition.x << " eX: " << (rPosition.x + rDimensions.x) << " sY: " << rPosition.y << " eY: " << (rPosition.y + rDimensions.y) << std::endl;
	if (
		(mouseX > (rPosition.x) && mouseX < (rPosition.x + rDimensions.x)) &&
		((Globals::gScreenHeight - mouseY) > (rPosition.y) && (Globals::gScreenHeight - mouseY) < (rPosition.y + rDimensions.y))
		&& IsHoverable && this->mHasDrawn) {
		mColor = mHoverColor;
		result = true;
	}
	else {
		mColor = mOriginalColor;
		result = false;
	}
	return result;
}
glm::vec2 Text2D::GetTextDimensions() {
	return this->rDimensions;
}
bool Text2D::MouseClick(float mouseX, float mouseY) {
	//std::cout << "Checking mouse click: " << mouseX << ", " << mouseY << std::endl;
	if (HoveringCheck(mouseX,mouseY, false)) {
		if (this->OnClickFunction != nullptr) {
			this->CallOnClickFunction(this->mOnClickParams);
		}
		return true;
	}
	return false;
}
void Text2D::AddOnClickParam(void* param)
{
	this->mOnClickParams.push_back(param);
}
void Text2D::TextHoverChecks(double mX, double mY) {
	for (Text2D* text : Text2D::gTextList) {
		text->HoveringCheck(mX, mY);
	}
}
void Text2D::CallOnClickFunction(std::vector<void*> params)
{
	this->OnClickFunction(params);
}

Text2D::~Text2D()
{
	std::vector<Text2D*>::iterator loc = std::find(Text2D::gTextList.begin(), Text2D::gTextList.end(), this);
	Text2D::gTextList.erase(loc);
}
