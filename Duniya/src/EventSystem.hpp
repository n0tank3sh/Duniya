#pragma once
#include <cstdint>
struct Event {
    enum class EventType {
	KEYBOARD,
	MOUSE,
    };
    EventType type;
};

struct KeyBoardEvent : public Event {
    KeyBoardEvent();
    bool specialKey;
    enum class SpecialKey { SHIFT, CTRL, ALT, CAPSLOCK, SPACE };
    int8_t key;
};

struct MouseEvent : public Event {
    MouseEvent();
    enum class MouseButton { LMOUSEBUTTON, RMOUSEBUTTON, MIDMOUSEBUTTON };
};
