#include <ecs/ecs.h>
#include <SDL2/SDL.h>



class Editor
{
public:
	Editor(uint32_t width = 1200, uint32_t height= 700);
	void Import(std::string filePath);
	void Export(std::string filePath);
	void Run();
};
