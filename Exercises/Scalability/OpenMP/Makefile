CXX=icpc

EXEC=omp_example

CXXFLAGS=-std=c++0x -openmp -O3 -xHost

INC=-I./inc -I/usr/global/intel/mkl/11.0.0.079/include -I/usr/global/intel-13/boost/1.54.0/include
LIB=-L/usr/global/intel/mkl/11.0.0.079/lib/intel64 -lmkl_rt -lm


SRC:=$(wildcard src/*.cpp)
SRCC:=$(wildcard src/*.c)
OBJ:=$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
OBJC:=$(addprefix obj/,$(notdir $(SRCC:.c=.o)))

all: $(EXEC)
	mv $< bin/

$(EXEC): $(OBJ) $(OBJC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ 

obj/%.o: src/%.c
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ 

clean:
	rm  $(OBJ) $(OBJC)
	rm bin/$(EXEC) 
	
