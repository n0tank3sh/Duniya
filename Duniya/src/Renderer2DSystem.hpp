#pragma once
#include <ECS/ECS.hpp>
#include <Graphics/Renderer.hpp>
#include <TexturePacker.hpp>
#include <cstdint>
#include <unordered_map>

namespace ComponentTypes {
enum : uint32_t {
    PANEL = 20,
    TEXTBOX,
    TEXTPANEL,
};
};

struct Panel {
    Vect4 dimension;
    Vect4 color;
    float sideDist;
};

struct TextPanel {
    Vect4 dimension;
};

struct Sprite {
    Vect2 uv[4];
    uint32_t data;
};

struct Text {
    std::string str;
    uint32_t scale;
    Vect3 color;
};

struct Button {
    uint32_t id;
};

struct Font {
    uint32_t width, height, data, fontSize;
    Vect2 umap[128][4];
};

class Renderer2DSystem : public System {
    struct AddMessage : public Message {
	uint32_t entity;
	AddMessage(uint32_t entity);
    };

   public:
    void LoadScene(Scene* scene) override;
    void Update(float deltaTime) override;
    friend class RendererSystem;
    static Renderer2DSystem* Init();
    static Renderer2DSystem* GetSingleton();

   private:
    Renderer2DSystem();

   public:
    static Renderer2DSystem* singleton;
    Renderer* renderer;

   private:
    FontDict defaultFont;
    std::vector<Font> fonts;
    std::vector<uint32_t> texts;
    std::unique_ptr<ShaderStageHandler> shaderStageHandler;
    std::unique_ptr<ShaderStageHandler> fontShaderStageHandler;
    Scene* scene;
    std::vector<uint32_t> panels;
    uint32_t layout;
    void LoadFontFile(std::string fontFile);
    void LoadPanels();
    void LoadFontGlyph();
    void ProcessMessages();
    void Scan();
    void Add(uint32_t);
};
