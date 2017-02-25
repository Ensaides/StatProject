CC=g++-6
CFLAGS=-std=c++17 
INCUDES=-I./include -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxl -I/usr/include/curlpp
SRC=./src/StatProject.cpp ./src/Connection.cpp ./src/HTMLParser.cpp ./src/JSONParser.cpp ./src/Config.cpp ./src/IO.cpp ./src/TSVParser.cpp ./src/ExcelParser.cpp ./src/Dataset.cpp
LUASRC=./src/StatProjectLua.cpp ./src/Test.cpp # ./src/Database.cpp
LIBS= -lcurl -lxml2 -ljsoncpp -lxl -pthread #-x86_64-linux-gnu/libOpenCL.so.1 -lodbc

StatProjectExecutable: $(SRC) $(INCLUDES)
	$(CC) $(CFLAGS) -DSTATPROJECT_EXECUTABLE $(INCUDES) -o./bin/debug/statproject $(SRC) ./src/main.cpp $(LIBS)

StatProjectModule: $(SRC) $(INCLUDES)
	$(CC) $(CFLAGS) -fPIC -c -DSTATPROJECT_MODULE $(INCUDES) -I/usr/include/lua5.3 $(SRC) $(LUASRC) ./src/modulemain.cpp $(LIBS)
	$(CC) -O -shared -fPIC -o./bin/debug/statproject.so *.o $(LIBS)
	mv *.o obj