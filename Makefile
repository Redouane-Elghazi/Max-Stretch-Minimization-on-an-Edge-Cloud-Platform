PROG		= main
PRODNAME	= schedule_sim
CXX			= g++
CXXFLAGS	= -Wall -O3 -std=c++11
LIBRARY		= task processor scheduler srpt greedy taskcreator ssfedf2 bender kanginstance

OBJECTS		= $(LIBRARY:=.o) $(PROG:=.o)

default : $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(STD) -o $(PRODNAME) $^

all: default
	
main.o : main.cpp task.hpp processor.hpp scheduler.hpp srpt.hpp greedy.hpp bender.hpp kanginstance.hpp bender.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

task.o : task.cpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

taskcreator.o : taskcreator.cpp taskcreator.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

kanginstance.o : kanginstance.cpp kanginstance.hpp scheduler.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

processor.o : processor.cpp processor.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

scheduler.o : scheduler.cpp scheduler.hpp processor.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

srpt.o : srpt.cpp srpt.hpp scheduler.hpp processor.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

greedy.o : greedy.cpp greedy.hpp scheduler.hpp processor.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

ssfedf2.o : ssfedf2.cpp ssfedf2.hpp scheduler.hpp processor.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@

bender.o : bender.cpp bender.hpp scheduler.hpp processor.hpp task.hpp
	$(CXX) -c $(CXXFLAGS) $(STD) $< -o $@
	
cleanall : clean
	rm -f $(PRODNAME)

clean :
	rm -f $(OBJECTS) $(PROG:=.o)