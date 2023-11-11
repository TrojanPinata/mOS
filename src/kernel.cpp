#include <types.h>
#include <gdt.h>

using namespace os;

void printf(char* str) {

    static uint16_t* vidmem = (uint16_t*)0xb8000;
    for(int i = 0; str[i] != '\0'; ++i) {
        vidmem[i] = (vidmem[i] & 0xFF00) | str[i];
    }

    /*static uint8_t x=0, y=0;
    static bool cliCursor = false
    uint16_t attrib = 0x07;
    volatile uint16_t* vidmem;

    for (int i = 0; str[i] != '\0'; i++) {
        vidmem = (volatile uint16_t*)0xb8000 + (80*y+x);
    }*/
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for(constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t magicnumber) {
    printf("monkaS");

    GlobalDescriptorTable* gdt;

    while(1);
}
