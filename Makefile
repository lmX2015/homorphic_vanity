default: generate_keys generate_vanity test_homomorphic

keccak.o: keccak.cpp
	g++ -c -Wall -O3 -std=c++11 keccak.cpp -o keccak.o
encryption.o: encryption.cpp keccak.o
	g++ -c -Wall -O3 -std=c++11  encryption.cpp -lgmp -o encryption.o

generate_keys.o: generate_keys.cpp
	g++ -c -Wall -O3 -std=c++11  generate_keys.cpp -lgmp -o generate_keys.o

generate_keys: generate_keys.o encryption.o 
	g++ generate_keys.o -lgmp -o generate_keys
generate_vanity.o: vanity.cpp
	g++ -c -Wall -O3 -std=c++11  vanity.cpp -lgmp -o generate_vanity.o

generate_keys: generate_vanity.o encryption.o
	g++ generate_vanity.o -lgmp -o generate_vanity

test_homomorphic.o: test_homomorphic.cpp
	g++ -c -Wall -O3 -std=c++11 test_homomorphic.cpp -lgmp -o test_homomorphic.o


test_homomorphic: test_homomorphic.o encryption.o
	g++ test_homomorphic.o -lgmp -o test_homomorphic


clean:
	rm *.o


debug: encryption_debug
	gdb encryption_debug

encryption_debug: encryption_debug.o
	g++ encryption_debug.o -lgmp -o encryption_debug

encryption_debug.o: encryption.cpp
	g++ -c -g -Wall -O3 -std=c++11  encryption.cpp -lgmp -o encryption_debug.o

