#ifndef __OS__GRAPHICS__DESKTOP_H
#define __OS__GRAPHICS__DESKTOP_H

#include <graphics/widget.h>
#include <mouse.h>

namespace os {
    namespace graphics {
        class Desktop : public CompositeWidget, public MouseEventHandler {
            public:
                uint32_t MouseX;
                uint32_t MouseY;

            public:
                Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b);
                ~Desktop();

                void Draw(GraphicsContext* gc);
                void OnMouseDown(uint8_t button);
                void OnMouseUp(uint8_t button);
                void OnMouseMove(int x, int y);
        };
    }
}

#endif