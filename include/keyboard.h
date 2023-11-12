#ifndef __OS__KEYBOARD_H
#define __OS__KEYBOARD_H

#include <types.h>
#include <interrupts.h>
#include <port.h>
#include <driver.h>

namespace os {
    class KeyboardEventHandler {
        public:
            KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
    };

    class KeyboardDriver : public InterruptHandler, public Driver {
        Port8Bit dataport;
        Port8Bit commandport;
        KeyboardEventHandler* handler;
        
    public:
        KeyboardDriver(InterruptManager* manager, KeyboardEventHandler *handler);
        ~KeyboardDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
        virtual void Activate();
    };
}

#endif
