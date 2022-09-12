
GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/drivers/driver.o \
		  obj/hardwarecommunication/port.o \
	      obj/hardwarecommunication/interruptstubs.o \
	      obj/hardwarecommunication/interrupts.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
          obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0'                  > iso/boot/grub/grub.cfg
	echo 'set default=0'                 >> iso/boot/grub/grub.cfg
	echo ''                              >> iso/boot/grub/grub.cfg
	echo 'menuentry "My OS" {'           >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo ' boot'                         >> iso/boot/grub/grub.cfg
	echo '}'                             >> iso/boot/grub/grub.cfg
	grub2-mkrescue --output=$@ iso
	rm -rf iso/

run: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "MyOS" &

#install: mykernel.bin
#	suco cp $< /boot/mykernel.bin

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso
