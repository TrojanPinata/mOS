GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-leading-underscore -fno-stack-protector -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	obj/gdt.o \
	obj/port.o \
	obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mOS.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mOS.bin
	sudo cp $< /boot/mOS.bin

mOS.iso: mOS.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mOS.bin iso/boot/mOS.bin
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "mOS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mOS.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mOS.iso iso
	rm -rf iso

	qemu-img create -f qcow2 Image.img 1G
	dd if=mOS.iso of=Image.img

run: mOS.iso
	qemu-system-x86_64 -enable-kvm \
		-boot menu=on \
		-drive id=disk,file=Image.img,format=raw,if=none \
		-device piix4-ide,id=piix4 -device ide-hd,drive=disk,bus=piix4.0 \
		-cpu 486 -smp 1 -m 1G \
		-vga virtio 

.PHONY: clean
clean:
	rm -rf obj mOS.bin mOS.iso

# use if making iso fails for some reason
isodelete:
	rm -rf iso

# use if image is a issue
imgdelete:
	rm Image.img
