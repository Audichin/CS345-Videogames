CXX = clang++

VCPKG_ROOT = C:/Users/glimp/vcpkg
TRIPLET_STAT = x64-mingw-static
TRIPLET_WIN = x64-windows
TRIPLET_DYNAM = x64-mingw-dynamic

INCLUDES = -I$(VCPKG_ROOT)/installed/$(TRIPLET_win)/include \
           -I$(VCPKG_ROOT)/installed/$(TRIPLET_WIN)/include \
		   -I$(VCPKG_ROOT)/installed/$(TRIPLET_DYNAM)/include

LIBDIR = -L$(VCPKG_ROOT)/installed/$(TRIPLET)/lib \
         -L$(VCPKG_ROOT)/installed/$(TRIPLET_WIN)/lib \
		 -L$(VCPKG_ROOT)/installed/$(TRIPLET_DYNAM)/lib

LIBS = -lcurl -lzlib \
       -lws2_32 -lcrypt32 -lwldap32 -lbcrypt -liphlpapi -lsecur32 -pthread

CXXFLAGS = -std=c++17 -Wall

PROJ = main

$(PROJ).exe: $(PROJ).cpp
	$(CXX) $(PROJ).cpp $(CXXFLAGS) $(INCLUDES) -o $@ $(LIBDIR) $(LIBS)

run: $(PROJ).exe
	./$(PROJ).exe
