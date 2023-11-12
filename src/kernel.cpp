#include <types.h>
#include <gdt.h>
#include <driver.h>
#include <port.h>
#include <interrupts.h>
#include <keyboard.h>
#include <mouse.h>

using namespace os;

void printf(char* str) {
	static uint8_t x = 0, y = 0;
	static bool cliCursor = false;
	uint16_t attrib = 0x07;
	volatile uint16_t* vidmem;

	for (int i = 0; str[i] != '\0'; i++) {
		vidmem = (volatile uint16_t*)0xb8000 + (80*y+x);
		
		switch (str[i]) {
			case '\b':
				vidmem = (volatile uint16_t*)0xb8000 + (80*y+x);
				*vidmem = ' ' | (attrib << 8);
				vidmem--; *vidmem = '_' | (attrib << 8);
				x--;

				break;

			case '\n':
				*vidmem = ' ' | (attrib << 8);
				y++;
				x = 0;
				break;

			case '\t': //$: shell interface		
				if (!i) {
					cliCursor = true;
					if (x < 3) { x = 3; }
					
					vidmem = (volatile uint16_t*)0xb8000 + (80*y);
					*vidmem = '$' | 0xc00;
					vidmem++; *vidmem = ':' | 0xf00;
					vidmem++; *vidmem = ' ';
				} 
                else { // check this because it may be an issue later
					*vidmem = '_' | (attrib << 8);
				}
				break;
				
			case '\v': //clear screen
				for (y = 0; y < 25; y++) {
					for (x = 0; x < 80; x++) {
					
						vidmem = (volatile uint16_t*)0xb8000 + (80*y+x);
						*vidmem = 0x00;
					}
				}
				x = 0;
				y = 0;
				break;

			default:
				*vidmem = str[i] | (attrib << 8);
				x++;
				break;
		}
	
		if (x >= 80) {
			y++;
			x = 0;
		}

		if (y >= 25) { // this is a good implementation.
			uint16_t scroll_temp;
			for (y = 1; y < 25; y++) {	
				for (x = 0; x < 80; x++) {
					vidmem = (volatile uint16_t*)0xb8000 + (80*y+x);
					scroll_temp = *vidmem;
						
					vidmem -= 80;
					*vidmem = scroll_temp;
					
					if (y == 24) {
						vidmem = (volatile uint16_t*)0xb8000 + (1920+x);
						*vidmem = ' ' | (attrib << 8);
					}
				}
			}
			x = 0;
			y = 24;
		}
	}
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler {
	public:
    	void OnKeyDown(char c) {
        	char* foo = " ";
        	foo[0] = c;
        	printf(foo);
    	}
};

class MouseToConsole : public MouseEventHandler {
    int8_t x, y;
	public:

    MouseToConsole() {}

    virtual void OnActivate() {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }

    virtual void OnMouseMove(int xoffset, int yoffset) {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
};



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for(constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t magicnumber) {
    printf("\v"); // clear screen
    printf("   (\\(\\\n   (-.-)\n  o_(\")(\")\n"); // bunny

    GlobalDescriptorTable* gdt;
    InterruptManager interrupts(0x20, gdt);
    
	printf("Initializing Hardware, Stage 1\n");

    DriverManager drvManager;

        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        drvManager.AddDriver(&keyboard);

        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
        drvManager.AddDriver(&mouse);


    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");

    interrupts.Activate();

    while(1);
}

