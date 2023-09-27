CC := g++

TARGET := main
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

all : $(TARGET)

$(TARGET) : $(OBJ) 
	$(CC) $^ -o $@

%.o : %.cpp
	$(CC) -c $< -o $@

clean :
	rm main main.o

.PHONY : all clean