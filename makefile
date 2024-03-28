cache : 
	gcc -c cache.c -o cache.o
	
simulate :
	gcc startSimulation.c -o simu
	./simu

si:
	gcc func.c -o a
	./a

clean :
	rm -f simu.exe cache.o
