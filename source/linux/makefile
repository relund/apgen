all: generator

apmat.o: ../code/apmat.cpp ../code/apmat.hpp ../code/basicdt.hpp
	g++ -O -c ../code/apmat.cpp -I /usr/include/libxml2

apmatgen.o: ../code/apmatgen.cpp ../code/apmatgen.hpp ../code/basicdt.hpp
	g++ -O -c ../code/apmatgen.cpp -I /usr/include/libxml2

apsolution.o: ../code/apsolution.cpp ../code/apsolution.hpp ../code/basicdt.hpp
	g++ -O -c ../code/apsolution.cpp -I /usr/include/libxml2

cell.o: ../code/cell.cpp ../code/cell.hpp ../code/basicdt.hpp
	g++ -O -c ../code/cell.cpp -I /usr/include/libxml2

centry.o: ../code/centry.cpp ../code/centry.hpp ../code/basicdt.hpp  ../code/point.hpp
	g++ -O -c ../code/centry.cpp -I /usr/include/libxml2

rand.o: ../code/rand.cpp ../code/rand.hpp
	g++ -O -c ../code/rand.cpp -I /usr/include/libxml2

generator: ../code/main_gen.cpp apmat.o apmatgen.o apsolution.o cell.o centry.o rand.o
	g++ -I /usr/include/libxml2 apmat.o apmatgen.o apsolution.o cell.o centry.o rand.o ../code/main_gen.cpp -O -o apgen -lxml2

clean:
	/bin/rm -rf *.o *~
	/bin/rm -rf $(EX)
