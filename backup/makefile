main:main.o tcp_link.o process.o message.o
	g++ main.o tcp_link.o process.o message.o  -o main
main.o:main.cpp
	g++ -c main.cpp -o main.o
tcp_link.o:tcp_link.cpp tcp_link.h
	g++ -c tcp_link.cpp -o tcp_link.o
process.o:process.cpp process.h
	g++ -c process.cpp -o process.o
message.o:message.cpp message.h
	g++ -c message.cpp -o message.o
	
clean:
	rm -f main main.o process.o tcp_link.o 
