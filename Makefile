run: lab1.c
	g++ lab1.c -lglut -lGLU -lGL -lGLEW -g
	
clean: 
	rm -f *.out *~ run
	
