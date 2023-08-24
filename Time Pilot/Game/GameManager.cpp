#include "GameManager.h"


void GameManager::CallBehaviouralFunctions()
{
	int i = 0;
	for (Behaviour* Object : Behaviour::BehaviourList) {
		if (Object != nullptr && Behaviour::BehaviourInMemory[Object] != false) {
			if (Object->IsBehaviouralFunctionAttached()) {
				Object->CallBehaviouralFunction();
				i++;
			}
		}
	}
}
void GameManager::RemoveBehaviourFromList(Behaviour* bObject) {
	Behaviour::RemoveBehaviour(bObject);
}
//std::vector<CelestialRock*> GameManager::GetCelestialRockList() {
//	return CelestialRock::CelestialRockList;
//}
//void GameManager::RemoveCelestialRockFromList(CelestialRock* rock) {
//	std::vector<CelestialRock*>::iterator it = std::find(CelestialRock::CelestialRockList.begin(), CelestialRock::CelestialRockList.end(), rock);
//	if (it != CelestialRock::CelestialRockList.end()) {
//		CelestialRock::CelestialRockList.erase(it);
//	}
//}

void GameManager::PollTextClickEvents(Window* window) {
	double mouseX, mouseY;
	window->GetCursorPosition(&mouseX, &mouseY);
	for (Text2D* text : Text2D::gTextList) {
		if (text->MouseClick(mouseX, mouseY)) break;
	}

}