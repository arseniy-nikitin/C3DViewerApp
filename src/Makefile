CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
LINUX_FLAGS=-lsubunit -lrt -lpthread -lm

CHECK_SRC=./tests/test_*.check
TEST_SRC=./tests/test_*.c
BD_SRC = ../build/

DIR=./3DViewer/
SRC=$(foreach dir,$(DIR),$(wildcard $(dir)*.c))
OBJ=$(patsubst %.c,%.o,$(SRC))

MSG=Minor bug fixes
BRANCH=$(shell git branch --show-current)

OS:=$(shell uname -s)

ifeq ($(OS),Linux)
  OPEN_CMD = xdg-open
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

.PHONY: test_3DViewer.c 3DViewer.a

all:
	make install
	make test
	make run

rebuild:
	make clean
	make uninstall
	make all

install: clean
	mkdir -p $(BD_SRC)
	cd $(BD_SRC) && qmake ../src/3DViewer/3DViewer.pro && make && make clean && rm -rf .qmake.stash Makefile

uninstall:
	rm -rf ../build

dist:
	mkdir 3DViewer_v1.0
	cp -R 3DViewer tests Makefile 3DViewer_v1.0
	tar cvzf ../3DViewer_v1.0.tar 3DViewer_v1.0
	rm -rf 3DViewer_v1.0

dvi:
	open ./readme.md

run:
ifeq ($(OS), Linux)
	$(OPEN_CMD) $(BD_SRC)3DViewer
else
	$(OPEN_CMD) $(BD_SRC)3DViewer.app
endif

test_3DViewer.c: $(CHECK_SRC)
	checkmk clean_mode=1 $(CHECK_SRC) > ./tests//$@

3DViewer.a: $(OBJ)
	ar rc $@ $(OBJ)
	ranlib $@
	cp $@ lib$@
	make clean_obj

clean_obj:
	rm -rf $(OBJ)

test: test_3DViewer.c 3DViewer.a
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) ${TEST_SRC} 3DViewer.a -o unit_test -lcheck -lm
else
	$(CC) $(CFLAGS) ${TEST_SRC} 3DViewer.a -o unit_test -lcheck $(LINUX_FLAGS)
endif
	./unit_test

clean_test:
	rm -rf unit_test

# If lcov cause errors add "--filter range" after --no-external flag, works only in MAC
gcov_report: test
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o gcov_test -lcheck -lm
else
	$(CC) $(CFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o gcov_test -lcheck $(LINUX_FLAGS)
endif
	./gcov_test
	lcov -t "stest" -o s21_test.info --no-external -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html

clean_gcov:
	rm -rf *.gcda *.gcno *.info report gcov_test

#cppcheck --enable=all --suppress=missingIncludeSystem ${SRC}
style:
	clang-format --style=Google -n */*.c */*.cpp */*.h

clean:
	make clean_obj
	make clean_test
	make clean_gcov
	rm -rf *.a *.dSYM ./tests/test_*.c
	rm -rf ./build-3DViewer*

push:
ifeq ($(BRANCH), develop)
	git add ./3DViewer 
endif
	git add Makefile tests readme.md
	git commit -m "$(BRANCH): $(MSG)"
	git push origin $(BRANCH)
