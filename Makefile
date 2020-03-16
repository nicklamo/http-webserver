web-server: web-server.o handle-request.o
	gcc $^ -o $@

handle-request.o: handle-request.c handle-request.h
	gcc -c handle-request.c
	
web-server.o: web-server.c web-server.h
	gcc -c web-server.c


clean:
	rm *.o
	rm web-server
