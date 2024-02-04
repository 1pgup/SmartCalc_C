CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
ADD_FLAGS = -lcheck -lsubunit -pthread -lrt
LDFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0) -lm
TARGET = smart_calc
TARGET_TEST = test_exec
SRC_FILES = calc_algorithm/*.c visual_functions/*.c smart_calc.c
TEST_FILES = calc_algorithm/*.c test.c
DOC_FILES = documentation.tex
LATEX = latex

all: $(TARGET)
	sudo apt update
	sudo apt install libgtk-3-dev
	sudo apt install libcairo2-dev
	sudo apt install texlive

test: $(TARGET_TEST)
	./$(TARGET_TEST)
	rm -f $(TARGET_TEST)

test_val: $(TARGET_TEST)
	valgrind --tool=memcheck --leak-check=yes -s ./$(TARGET_TEST)
	rm -f $(TARGET_TEST)

$(TARGET_TEST): $(TEST_FILES)
	$(CC) $(CFLAGS) -o $(TARGET_TEST) $(TEST_FILES) $(ADD_FLAGS) $(LDFLAGS)

$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES) $(LDFLAGS)

gcov_report: 
	$(CC) --coverage $(CFLAGS) -o $(TARGET_TEST) $(TEST_FILES) $(ADD_FLAGS) $(LDFLAGS)
	./$(TARGET_TEST)
	lcov -t "$(TARGET_TEST)" -o $(TARGET_TEST).info -c -d .
	genhtml -o report $(TARGET_TEST).info
	rm -f *.gcda *.gcno *.info $(TARGET_TEST)

install: all
	sudo install -m 755 $(TARGET) /usr/local/bin

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

clean: uninstall clean_cov
	rm -f $(TARGET) *.dvi *.aux *.log

clean_cov:
	rm -f *.gcno *.gcda *.info *.gcov $(TARGET_TEST)

DVI_FILES = $(DOC_FILES:.tex=.dvi)

dvi: $(DVI_FILES)

%.dvi: %.tex
	$(LATEX) $<

$(DVI_FILES): $(DOC_FILES)

dist: clean
	tar -czvf smart_calc.tar.gz *

check_linters:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -n *.c calc_algorithm/* visual_functions/*
	rm -rf .clang-format

fix_linters:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -i *.c calc_algorithm/* visual_functions/*
	rm -rf .clang-format

.PHONY: all install uninstall clean dvi dist