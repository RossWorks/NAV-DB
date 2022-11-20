GTK_CLAUSE = `pkg-config gtkmm-4.0 --cflags --libs`
OBJ_FILES = ./obj/main.o ./obj/database.o
CC = g++
FLAGS = -g

OBJECTS = ./obj/main.o ./obj/StdDb.o ./obj/CommonUtils.o
EXECUTABLE = ./bin/NavDbBuilder

$(EXECUTABLE): ./obj/main.o ./obj/StdDb.o ./obj/CommonUtils.o ./obj/Presentation.o ./obj/MainWindow.o
	@echo Linking executable
	@$(CC) ./obj/main.o ./obj/StdDb.o ./obj/CommonUtils.o ./obj/Presentation.o ./obj/MainWindow.o -o $(EXECUTABLE) $(GTK_CLAUSE)

./obj/main.o: ./src/main.cpp
	@echo compiling main.cpp
	@$(CC) -c ./src/main.cpp -o ./obj/main.o $(GTK_CLAUSE) $(FLAGS)

./obj/MainWindow.o: ./src/Windows/MainWindow.cpp
	@echo compiling MainWindow.cpp
	@$(CC) -c ./src/Windows/MainWindow.cpp -o ./obj/MainWindow.o $(GTK_CLAUSE) $(FLAGS)

/obj/Presentation.o: ./src/Presentation/Presentation.cpp
	@echo compiling Presentation.cpp
	@$(CC) -c ./src/Presentation/Presentation.cpp -o ./obj/Presentation.o $(GTK_CLAUSE) $(FLAGS)

./obj/StdDb.o: ./src/StdDb/StdDb.cpp
	@echo compiling StdDb.cpp
	@$(CC) -c ./src/StdDb/StdDb.cpp -o ./obj/StdDb.o $(GTK_CLAUSE) $(FLAGS)

./obj/CommonUtils.o: ./src/Common/CommonUtils.cpp
	@echo compiling CommonUtils.cpp
	@$(CC) -c ./src/Common/CommonUtils.cpp -o ./obj/CommonUtils.o $(GTK_CLAUSE) $(FLAGS)
