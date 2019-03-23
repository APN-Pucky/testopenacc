C_SRC := $(wildcard *.c)
C_OBJ := $(patsubst %.c, obj-c/%.o,$(C_SRC))

CXX_SRC := $(wildcard *.cpp)
CXX_OBJ := $(patsubst %.cpp, obj-cxx/%.o,$(CXX_SRC))


OACC_FLAGS := -fopenacc
FLAGS := -O2

default: c cpp

c: $(C_OBJ)

cpp: $(CXX_OBJ) 

obj-c/%.o: %.c  obj-c/.stamp
	$(CC) $(FLAGS) $(OACC_FLAGS) -o $@ -c $<
	$(CC) $(OACC_FLAGS) $@ -o $(patsubst %.c, %.openacc,$<)
	$(CC) $(FLAGS) -o $(patsubst %.c, %.noopenacc,$<) $<

obj-cxx/%.o: %.cpp  obj-cxx/.stamp
	$(CXX) $(FLAGS) $(OACC_FLAGS) -o $@ -c $<
	$(CXX) $(OACC_FLAGS) $@ -o $(patsubst %.cpp, %.openacc,$<)
	$(CXX) $(FLAGS) -o $(patsubst %.cpp, %.noopenacc,$<) $<

obj-c/.stamp:
	mkdir -p obj-c
	touch obj-c/stamp

obj-cxx/.stamp:
	mkdir -p obj-cxx
	touch obj-cxx/stamp


