CXX = clang++

VCPKG_ROOT = C:/Users/glimp/vcpkg
TRIPLET = x64-mingw-static

INCLUDES = -I$(VCPKG_ROOT)/installed/$(TRIPLET)/include
LIBDIR = -L$(VCPKG_ROOT)/installed/$(TRIPLET)/lib

LIBS = -lcurl -lzlib -lws2_32 -lcrypt32 -lwldap32 -lbcrypt -liphlpapi -lsecur32
CXXFLAGS = -std=c++17

PROJ = phyphox

$(PROJ).exe: phyphox.cpp
	$(CXX) phyphox.cpp $(CXXFLAGS) $(INCLUDES) -o $@ $(LIBDIR) $(LIBS)

run: $(PROJ).exe
	./$(PROJ).exe
