#pragma once

#include <iostream>

#include <spdlog/spdlog.h>

#include <sstream>
#include <vector>
#include <unordered_map>

typedef void (*BehaviouralFunction)(void* data);

class Behaviour
{
private:
	BehaviouralFunction mFunction;
	void* mDataParam;
public:
	static std::unordered_map<Behaviour*, bool> BehaviourInMemory;
	static std::vector<Behaviour*> BehaviourList;
	static void RemoveBehaviour(Behaviour* behaviour);
public:
	Behaviour();
	void SetBehaviouralFunction(BehaviouralFunction function);
	void AddBehaviouralFunctionParam(void* data_param);
	void CallBehaviouralFunction();
	bool IsBehaviouralFunctionAttached();
	static void CleanUpBehaviourList() {
		for (std::unordered_map<Behaviour*, bool>::iterator it = Behaviour::BehaviourInMemory.begin(); it != Behaviour::BehaviourInMemory.end(); it++) {
			if (it->second == false) {
				if (it->first != nullptr) {
					delete it->first;
					Behaviour::BehaviourInMemory.erase(it->first);
					spdlog::warn("Cleaning up behaviour");
				}
			}
		}
	}
	~Behaviour();
};

