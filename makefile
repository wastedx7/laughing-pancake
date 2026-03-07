CXX     = g++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET  = main.exe
SRC     = src/main.cxx

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	del $(TARGET)