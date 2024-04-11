WWW = -Wall -Wextra -Werror

all:  s21_grep

s21_grep: clean
	gcc $(WWW) s21_grep.c -o s21_grep

test: s21_grep
	sh t_grep.sh


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
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extensions=c *.h
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	rm -rf CPPLINT.cfg


clean:
	rm -rf s21_grep
	rm -rf log.txt