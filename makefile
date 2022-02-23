programa: prueba_aristas.o 
	g++ -o programa prueba_aristas.o 


prueba_aristas.o: prueba_aristas.cc nodo.cc arista.cc
	g++ -c prueba_aristas.cc

nodo.o: nodo.cc nodo.h	
	g++ -c nodo.cc

arista.o: arista.cc arista.h
	g++ -c arista.cc	

