cache : 
	gcc -c cache.c -o cache.o
	
simulate :
	gcc startSimulation.c -o simu
	./simu

clean :
	rm -f simu.exe cache.o
