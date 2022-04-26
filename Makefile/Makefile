all: app

app: main.o funcao.o
	gcc obj/main.o obj/funcao.o -o bin/app

main.o: src/main.c
	gcc -c src/main.c -Iinc -o obj/main.o

funcao.o: src/funcao.c
	gcc -c src/funcao.c -Iinc -o obj/funcao.o

clean:
	rm obj/*.o bin/app
