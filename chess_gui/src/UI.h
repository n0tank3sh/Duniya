#include <ECS/ECS.h>
#include <array>
#include <unordered_map>
#include <Math/Vect2.h>

namespace ComponentTypes
{
enum : uint32_t 
	{
		BUTTON=127
	};
};

struct PressDown
{
};

struct PressUp
{
	float sec;
};

struct ButtonDown : public Message
{
	uint32_t entity;
};

class UISystem : public System
{
	Scene* scene;
	std::array<std::list<int>, 100> buttons;
	Vect2 screenResoution;
private:
	uint32_t Hash(const Vect2& a);
	uint32_t Hash(const float& x, const float& y);
public:
	UISystem();
	void LoadScene(Scene* scene) override;
	void Update(float deltaTime) override;
	void Scan();
	~UISystem();
};
