build:
	gcc ./src/main.c -o ./dist/main

run:
	./dist/main

clean:
	rm -f ./dist/*