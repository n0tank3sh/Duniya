#include <SDL2/SDL.h>

#include <ECS/ECS.hpp>

class Editor {
   public:
    Editor(uint32_t width = 1200, uint32_t height = 700);
    void Import(std::string filePath);
    void Export(std::string filePath);
    void Run();
};
