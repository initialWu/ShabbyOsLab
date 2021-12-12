# commands/Makefile

#ENTRYPOINT	= 0x1000
HD		= ../80m.img

ASM		= nasm
DASM		= objdump
CC		= gcc
LD		= ld
ASMFLAGS	= -I ../include/ -f elf
CFLAGS		= -I ../include/ -c -fno-builtin -fno-stack-protector -Wall
LDFLAGS		= -Ttext 0x1000
DASMFLAGS	= -D
LIB		= ../lib/orangescrt.a
#BIN		= ls echo pwd sum procA procB ps queue stest
BIN		= echo pwd pwd1 calendar guess attack ls bufof parity_check allocfree

# All Phony Targets
.PHONY : everything final clean realclean disasm all install

# Default starting position
everything : $(BIN)

install : all clean
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN)
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

all : realclean everything

final : all clean

clean :
	rm -f *.o

realclean :
	rm -f $(BIN) *.o

kernel.bin :
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o: echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB) $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? 

pwd.o: pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

pwd1.o: pwd1.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd1 : pwd1.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

calendar.o: calendar.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

calendar : calendar.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

guess.o: guess.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

guess : guess.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

attack.o: attack.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

attack : attack.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

bufof.o: bufof.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

bufof: bufof.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

parity_check.o: parity_check.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

parity_check: parity_check.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

allocfree.o: allocfree.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

allocfree : allocfree.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

# sum.o: sum.c ../include/type.h ../include/stdio.h
# 	$(CC) $(CFLAGS) -o $@ $<

# sum : sum.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $?

# procA.o: procA.c ../include/type.h ../include/stdio.h
# 	$(CC) $(CFLAGS) -o $@ $<

# procA : procA.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $?

# procB.o: procB.c ../include/type.h ../include/stdio.h
# 	$(CC) $(CFLAGS) -o $@ $<

# procB : procB.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $?

# ps.o: ps.c
# 	$(CC) $(CFLAGS) -o $@ $<

# ps : ps.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $?

# queue.o: queue.c
# 	$(CC) $(CFLAGS) -o $@ $<

# queue : queue.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $?

# stest.o: stest.c
# 	$(CC) $(CFLAGS) -o $@ $<

# stest : stest.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $?

# stest.o: stest.c
# 	$(CC) $(CFLAGS) -o $@ $<
ls.o: ls.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

ls : ls.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? $(LIB)

