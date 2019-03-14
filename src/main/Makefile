OBJECTS = ../../tmp/file_io.o
TARGETS = ../../bin/file_io.out
SOURCES = file_io.c

$(TARGETS) : $(OBJECTS)
	gcc $(OBJECTS) -o $(TARGETS) 

$(OBJECTS) :
	gcc $(SOURCES) -c -o $(OBJECTS)

.PHONY : run clean
clean :
	cd ../../tmp/ && rm *.o

run :
	$(TARGETS)
