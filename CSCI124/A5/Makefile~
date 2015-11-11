# ass1.make -- makefile for Ass5
# to run: make
# to clean: make clean

ass5 : main.o ass5.o list.o btree.o 
	g++ -o ass5 main.o ass5.o list.o btree.o
# above, we are saying ass5 depends on main.o, ass5.o list.o btree.o 
# and to create ass5 we give the g++ command as shown on the next line

# which starts with a TAB (although you cannot see that).
# note that the command : g++ -o ass5 main.o ass5.o list.o btree.o 
# creates an executable file named ass5 from the 2 object files respectively.
main.o: main.cpp ass5.h
	g++ -c main.cpp
# above we are saying main.o depends on main.cpp and ass5.h
# and to compile only main.cpp if and only if main.cpp or ass5.h
# have changed since the last creation of main.o
ass5.o: ass5.cpp ass5.h list.h btree.h
	g++ -c ass5.cpp
# above we are saying ass5.o depends on ass5.cpp, ass5.h, list.h and btree.h
# so if either ass5.cpp, ass5.h, list.h & btree.h CHANGED since creating ass5.o
# compile ass5.cpp again
list.o: list.h
	g++ -c list.cpp
# above we are saying list.o depends on list.cpp and list.h
# so if either list.cpp or list.h CHANGED since creating list.o
# compile list.cpp again
btree.o: btree.cpp  btree.h
	g++ -c btree.cpp
# above we are saying btree.o depends on btree.cpp and btree.h
# so if either btree.cpp or btree.h CHANGED since creating btree.o
# compile btree.cpp again

clean:
	rm *.o ass5
# above we are stating how to run the rule for clean, no dependencies,
# what we want is when we ask to do a "make -f ass5.make clean"
# that will not do anything except remove executable and object files
# so we can "clean out" our directory of unneeded large files.
# we only do a make clean when we want to clean up the files.

# END OF MAKE FILE
