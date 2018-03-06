CXX = g++
CXXFLAGS = -Wall -g

main: main.o lexar.o
	$(CXX) $(CXXFLAGS) -o main main.o lexar.o

main.o: main.cpp lexar.h 
	$(CXX) $(CXXFLAGS) -c main.cpp

lexar.o: lexar.h 
	$(CXX) $(CXXFLAGS) -c lexar.cpp

clean:
	rm *.o ./main
	


