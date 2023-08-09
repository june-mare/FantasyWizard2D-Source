#include "Component.h"
#include "GameObject.h"

void Component::Reload() noexcept
{
	ThisStart();
	for (auto itr : gameObject->componentList)
	{
		if (itr->isStart == true)
		{
			continue;
		}
		itr->Start();
		itr->ThisStart();
	}
}
void Component::ThisStart()noexcept { isStart = true; }