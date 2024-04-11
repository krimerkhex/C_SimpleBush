WWW = -Wall -Wextra -Werror

all: clean s21_cat

s21_cat:
	gcc $(WWW) s21_cat.c -o s21_cat

test: s21_cat
	bash t_cat.sh

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

install_valgrind:
	brew install --HEAD LouisBrunner/valgrind/valgrind

install_cpch:
	brew install cppcheck

cpplint:
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extensions=c *.h
	python3 ../../materials/linters/cpplint.py --extensions=c *.c
	rm -rf CPPLINT.cfg

valgf:
	valgrind --leak-check=full --track-origins=yes -s ./s21_cat -n bytes.txt

clean:
	rm -rf s21_cat
	rm -rf log.txt