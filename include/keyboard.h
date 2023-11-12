#ifndef __OS__KEYBOARD_H
#define __OS__KEYBOARD_H

#include <types.h>
#include <interrupts.h>
#include <port.h>

namespace os {
    class KeyboardDriver : public InterruptHandler {
        Port8Bit dataport;
        Port8Bit commandport;
    public:
        KeyboardDriver(InterruptManager* manager);
        ~KeyboardDriver();
        virtual os::uint32_t HandleInterrupt(os::uint32_t esp);
    };
}

#endif
