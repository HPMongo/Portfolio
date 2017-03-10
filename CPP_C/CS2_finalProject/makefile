CXX = g++
CXXFLAGS = -std=c++0x           #current c compiler 
CXXFLAGS += -Wall               #turn on all warnings
CXXFLAGS += -pedantic-errors    #enforce the standards and generate errors if violated 
CXXFLAGS += -g                  #debug
LDFLAGS = -lboost_date_time

OBJS = final_main.o function.o

SRCS = final_main.cpp function.cpp

HEADERS = function.h

#target: dependencies
#       rule to build
#      
all: final
final: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o final
        
$(OBJS): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)
