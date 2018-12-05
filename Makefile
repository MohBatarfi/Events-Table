all: completedemo.cgi parser mailer

completedemo.cgi: cgicc/completedemo.o database.o ccevent.o student.o
	g++ -pthread -Wall -o index.cgi cgicc/completedemo.o database.o ccevent.o student.o -ldl -lcgicc -lsqlite3

completedemo.o: cgicc/completedemo.cpp database.h ccevent.h student.h
	g++ -Wall -c cgicc/completedemo.cpp

student.o: student.cpp database.h
	g++ -Wall -c student.cpp

ccevent.o: ccevent.cpp database.h
	g++ -Wall -c ccevent.cpp

# Use this syntax to compile main that use database, order matters at least for some of it
# g++ -pthread -o main main.o sqlite3.o database.o -ldl 

database.o: database.h database.cpp student.h ccevent.h
	g++ -c database.cpp 

# RSS parser files
parser: rssdaemon/parser.o database.o ccevent.o student.o
	g++ -pthread -Wall -g -o rssdaemon/parser rssdaemon/parser.o database.o ccevent.o student.o -lpugixml -lsqlite3 

parser.o: rssdaemon/parser.cpp database.h ccevent.h
	g++ -Wall -g -c rssdaemon/parser.cpp

# email daemon files
mailer: emaildaemon/mailer.o ccevent.o database.o student.o emaildaemon/makeemail.o
	g++ -Wall -g -o emaildaemon/mailer ccevent.o database.o student.o emaildaemon/makeemail.o emaildaemon/mailer.o -lcurl -lsqlite3

mailer.o: emaildaemon/mailer.cpp ccevent.h database.h student.h emaildaemon/makeemail.h
	g++ -Wall -g -c emaildaemon/mailer.cpp ccevent.h database.h student.h emaildaemon/makeemail.h

makeemail.o: emaildaemon/makeemail.h emaildaemon/makeemail.cpp student.h ccevent.h
	g++ -Wall -g -c emaildaemon/makeemail.cpp student.cpp ccevent.cpp

# command that deletes .o files and binary files
.PHONY: clean
clean:
	rm -f *.o rssdaemon/*.o rssdaemon/parser cgicc/*.o emaildaemon/*.o emaildaemon/main
