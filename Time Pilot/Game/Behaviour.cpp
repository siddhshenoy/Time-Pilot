#include "Behaviour.h"

std::vector<Behaviour*> Behaviour::BehaviourList;
std::unordered_map<Behaviour*, bool> Behaviour::BehaviourInMemory;
Behaviour::Behaviour()
{
	mFunction = nullptr;
	mDataParam = nullptr;
	Behaviour::BehaviourList.push_back(this);
	if (Behaviour::BehaviourInMemory.find(this) != Behaviour::BehaviourInMemory.end()) {
		spdlog::info("Already existing behaviour, setting it to true");
		Behaviour::BehaviourInMemory[this] = true;
	}
	else {
		Behaviour::BehaviourInMemory.insert(std::pair<Behaviour*, bool>(this, true));
	}
}

void Behaviour::SetBehaviouralFunction(BehaviouralFunction function)
{
	mFunction = function;
}

void Behaviour::AddBehaviouralFunctionParam(void* data_param)
{
	mDataParam = data_param;
}

void Behaviour::CallBehaviouralFunction()
{
	if (Behaviour::BehaviourInMemory.find(this) != Behaviour::BehaviourInMemory.end()) {
		if (Behaviour::BehaviourInMemory[this] != false) {
			if (mFunction != nullptr && mDataParam != nullptr) {
				mFunction(mDataParam);
			}
			else {
				spdlog::error("Tried accessing a nullptr functional behaviour with nullptr parameter");
			}
		}
		else {
			std::stringstream ss;
			ss << "Tried to call behavioural function but found the memory was deallocated! Trying to remove behaviour from list" << this;
			spdlog::error(ss.str());
		}
	}
}
bool Behaviour::IsBehaviouralFunctionAttached() {
	return (mFunction != nullptr);

}
void Behaviour::RemoveBehaviour(Behaviour* behaviour) {
	std::vector<Behaviour*>::iterator it = std::find(Behaviour::BehaviourList.begin(), Behaviour::BehaviourList.end(), behaviour);
	if(it != Behaviour::BehaviourList.end())
		Behaviour::BehaviourList.erase(it);

}
Behaviour::~Behaviour()
{
	std::stringstream ss;
	/*ss << "Destroying behaviour " << this;
	spdlog::info(ss.str());*/
	Behaviour::RemoveBehaviour(this);
	if (Behaviour::BehaviourInMemory.find(this) != Behaviour::BehaviourInMemory.end()) {
		Behaviour::BehaviourInMemory[this] = false;
	}
}
