#include <queue>
#include <memory>
#include <unordered_map>
#include <random>

class Scene
{
    class entityManager
    {
    };
    class IComponentArray
    {
    };
    std::unordered_map<uint32_t, IComponentArray> entities;
};
