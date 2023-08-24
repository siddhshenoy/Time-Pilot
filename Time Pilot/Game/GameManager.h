#pragma once

#include <iostream>
#include <vector>


#include <Game/Behaviour.h>
#include <Game/Text2D.h>

#include <Core/Window.h>

class GameManager
{
public:
	static void CallBehaviouralFunctions();
	static void RemoveBehaviourFromList(Behaviour* bObject);
public:
	static void PollTextClickEvents(Window* window);
};

