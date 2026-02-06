CXX = g++
CXXFLAGS = -std=c++17 -I$(HOME)/vcpkg/installed/x64-linux/include
LDFLAGS = -L$(HOME)/vcpkg/installed/x64-linux/lib -lcurl

SRC = main.cpp
TARGET = phyphox

all: $(TARGET)

$(TARGET): $(SRC)
    $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
    rm -f $(TARGET)
