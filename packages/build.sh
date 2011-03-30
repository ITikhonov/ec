#!/bin/sh

rm -f all.c

echo "#include <string.h>" > all._

for x in *.c; do
	init=$(basename $x .c|tr '.-' '__')_init;
	echo "struct package *$init(char*);" >> all._
done

echo "struct package *make_package(char *name, int l, char *arg) {" >> all._

for x in *.c; do
	init=$(basename $x .c|tr '.-' '__')_init;
	name=$(basename $x .c|tr 'a-z' 'A-Z')
	gcc -c -D PACKAGE=$init -Wall -Werror -g $x;
	echo "if(l==${#name} && strncmp(name,\"$name\",l)==0) return $init(arg);" >> all._
done

echo "return 0;}" >> all._

mv all._ all.c
gcc -c -Wall -Werror -g all.c

