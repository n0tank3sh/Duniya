#include "UI.hpp"
#include "ECS/ECS.hpp"
#include "Renderer2DSystem.hpp"
#include "SDLUtiliy.hpp"
#include <algorithm>
#include <cmath>


uint32_t Transform(float x)
{
	return std::floor(std::clamp((x * 10), 0.f, 9.f));
}

uint32_t UISystem::Hash(const float& x, const float& y)
{
	return  Transform(x) * 10 + Transform(y);
}

uint32_t UISystem::Hash(const Vect2& a)
{
	return Hash(a.x, a.y);
}


void UISystem::Scan()
{
}

void UISystem::LoadScene(Scene* scene)
{
	this->scene = scene;
}

template<typename T>
bool InRange(const T& a, const T& b, const T& x)
{
	return a <= x && x <= b;
}

void UISystem::Update(float deltaTime)
{
	for(auto& e: (*messagingSystem)[0x1])
	{
		if(e.first == EVENTS::MOUSE_EVENTS)
		{
			auto mouseEvent = dynamic_cast<MouseButtonEvent*>(e.second.get());
			auto hash = Hash(mouseEvent->event.x, mouseEvent->event.y);
			for(auto& entityID: this->buttons[hash])
			{
				auto entity = scene->GetEntity(entityID); 
				if(entity != nullptr)
				{
					auto button = entity->Get(ComponentTypes::BUTTON);
					auto panel = entity->Get<Panel>(ComponentTypes::PANEL);
					if(button != nullptr)
					{
						Vect2 ans((float)mouseEvent->event.x/this->screenResoution.x, (float)mouseEvent->event.y/this->screenResoution.y);
						ans = (ans - .5) * 2;
						if(InRange(panel->dimension.x, panel->dimension.z * panel->dimension.x, ans.x)
							&&InRange(panel->dimension.y, panel->dimension.w * panel->dimension.y, ans.y)
						)
						{
						}
					}
				}
			}
		}
	}
}
