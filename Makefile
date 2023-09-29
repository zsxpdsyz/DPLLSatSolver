CC := g++
CXXFLAG := -g

TARGET := main
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

all : $(TARGET)

$(TARGET) : $(OBJ) 
	$(CC) $(CXXFLAG) $^ -o $@

%.o : %.cpp
	$(CC) $(CXXFLAG) -c $< -o $@

clean :
	rm main main.o dpll.o

.PHONY : all clean