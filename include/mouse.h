#ifndef __OS__MOUSE_H
#define __OS__MOUSE_H

#include <types.h>
#include <port.h>
#include <interrupts.h>
#include <driver.h>

namespace os {
    class MouseEventHandler {
        public:
            MouseEventHandler();
            virtual void OnActivate();
            virtual void OnMouseDown(uint8_t button);
            virtual void OnMouseUp(uint8_t button);
            virtual void OnMouseMove(int x, int y);
    };

    class MouseDriver : public InterruptHandler, public Driver {
        Port8Bit dataport;
        Port8Bit commandport;
        uint8_t buffer[3];
        uint8_t offset;
        uint8_t buttons;
        MouseEventHandler* handler;

        public:
            MouseDriver(InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual uint32_t HandleInterrupt(uint32_t esp);
            virtual void Activate();
    };
}
#endif