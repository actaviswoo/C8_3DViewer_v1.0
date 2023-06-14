CPPFLAGS = -Wall -Werror -Wextra
CC = gcc
GFLAGS = --coverage
TFLAGS = -lcheck -lm -lpthread
ARFLAGS = rcs
OBJECTS = *.o
SOURCES = Project/affine.c Project/parsing.c
SOURCES_QT = 3DViewer_QT/main.cpp 3DViewer_QT/mainwindow.cpp 3DViewer_QT/mainwindow.h
SOURCES_TEST = Tests/test.c

all: clean 3dviewer.a

3dviewer.a: clean
	$(CC) $(CPPFLAGS) $(SOURCES) -c
	ar $(ARFLAGS) 3dviewer.a $(OBJECTS)
	rm $(OBJECTS)

test: 3dviewer.a
	$(CC) $(FLAGS) -c $(SOURCES_TEST) -o tests.o
	$(CC) tests.o 3dviewer.a $(TFLAGS) -o test
	./test

install_lcov:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

gcov_report: 3dviewer.a
	gcc -Wall -Werror -Wextra --coverage $(SOURCES_TEST) Project/3DViewer.h $(SOURCES) 3dviewer.a $(TFLAGS) -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t "gcovReport" -o gcov_test.info -c -d .
	genhtml -o report gcov_test.info
	open report/index.html

leaks: 3dviewer.a
	$(CC) $(CPPFLAGS) $(GFLAGS) 3dviewer.a $(SOURCES_TEST) $(TFLAGS) -o test
	CK_FORK=no leaks --atExit -- ./test

valgrind: clean
	rm -rf ~/Library/Containers/com.docker.docker
	mkdir -p ~/goinfre/Docker/Data
	ln -s ~/goinfre/Docker ~/Library/Containers/com.docker.docker
	cd Valgrind && sh run.sh

clean:
	rm -rf 3dviewer.a test *.html *.gcda *.gcno gcov_test *.info report *.o *.dSYM *.css

check:
	cppcheck $(SOURCES) $(SOURCES_TEST) Project/3DViewer.h
	cppcheck --language=c++ $(SOURCES_QT) --force
	clang-format -n $(SOURCES) $(SOURCES_TEST) Project/3DViewer.h -style=google
	clang-format -n $(SOURCES_QT) -style=google

install:
	qmake -o Executables/Makefile 3DViewer_QT/view.pro
	cd Executables && make

make start:
	cd Executables/view.app/Contents/MacOS && ./view

uninstall:
	cd Executables && make clean
	cd Executables && rm -rf Makefile qmake_qmake_qm_files.qrc view.app .qm .qmake.stash

dist:
	cd .. && tar -czf 3DViewer_v1.0.tgz src

dvi:
	open manual/manual.md
