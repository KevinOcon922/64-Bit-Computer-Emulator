cpu: main.o cpu.o ram.o
	g++ -o cpu main.o cpu.o ram.o

main.o: main.cpp cpu.h
	g++ -c main.cpp

cpu.o: cpu.cpp cpu.h ram.h
	g++ -c cpu.cpp

ram.o: ram.cpp ram.h storage.h
	g++ -c ram.cpp

clean:
	rm *.o cpu