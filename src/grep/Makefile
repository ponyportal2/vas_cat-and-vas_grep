build:
	gcc -Wextra -Werror -Wall vas_grep.c -o vas_grep
s21_grep:
	gcc -Wextra -Werror -Wall vas_grep.c -o vas_grep
all:
	gcc -Wextra -Werror -Wall vas_grep.c -o vas_grep
debug:
	gcc -Wextra -Werror -Wall vas_grep.c -o vas_grep -g
test: build
	bash diff_grep.sh