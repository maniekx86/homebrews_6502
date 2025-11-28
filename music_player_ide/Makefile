CC65PATH=
CC65=$(CC65PATH)cc65
CA65=$(CC65PATH)ca65
AR65=$(CC65PATH)ar65
LD65=$(CC65PATH)ld65


all:
	cp supervision.lib sbc.lib
	$(CA65) crt0.s
	$(AR65) a sbc.lib crt0.o
	$(CC65) --cpu 6502 -t none -Osir -Cl main.c
	$(CA65) main.s
	$(CA65) isr.s
	$(CA65) vectors.s
	$(LD65) -C sbc.cfg -m main.map isr.o vectors.o main.o sbc.lib -o rom.bin 

clean:
	rm -f crt0.o interrupt.o vectors.o main.o sbc.lib *.o *.bin main.map
