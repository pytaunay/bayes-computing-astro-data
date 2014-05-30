NVCC=nvcc
CXX=g++

EXEC=cuda_example

CXXFLAGS=-std=c++0x -fopenmp -O3
CFLAGS=-std=c99 -fopenmp -O3
CUDAFLAGS=-arch sm_35 -O3 

INC=-I./inc -I/usr/global/intel/mkl/11.0.0.079/include -I/usr/global/intel-13/boost/1.54.0/include
LIB=-L/usr/global/intel/mkl/11.0.0.079/lib/intel64 -lmkl_rt -lm
LIBCUDA=-L/usr/global/cuda/5.5/lib64 -lcublas

SRC:=$(wildcard src/*.cpp)
SRCC:=$(wildcard src/*.c)
SRCCU:=$(wildcard src/*.cu)

OBJ:=$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
OBJC:=$(addprefix obj/,$(notdir $(SRCC:.c=.o)))
OBJCU:=$(addprefix obj/,$(notdir $(SRCCU:.cu=.o)))

all: $(EXEC)
	mv $< bin/

$(EXEC): $(OBJ) $(OBJC) $(OBJCU)
	$(NVCC) -Xcompiler -fopenmp $(CUDAFLAGS) $^ -o $@ $(LIB) $(LIBCUDA)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ 

obj/%.o: src/%.c
	$(CXX) $(CFLAGS) $(INC) -c $< -o $@ 

obj/%.o: src/%.cu
	$(NVCC) -Xcompiler -fopenmp $(CUDAFLAGS) $(INC) -c $< -o $@ 

clean:
	rm  $(OBJ) $(OBJC) $(OBJCU)
	rm bin/$(EXEC) 
	
