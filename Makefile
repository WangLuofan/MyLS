myls: main.cpp dirhandler.o parsecmd.o fileinfo.h
	g++ main.cpp dirhandler.o parsecmd.o -o myls
dirhandler.o: dirhandler.h dirhandler.cpp fileinfo.h
	g++ -c dirhandler.cpp fileinfo.h
parsecmd.o: parsecmd.h parsecmd.cpp
	g++ -c parsecmd.cpp

clean:
	rm dirhandler.o parsecmd.o fileinfo.h.gch
