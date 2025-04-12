build:
	gcc ./src/main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o ./dist/main

run:
	./dist/main

clean:
	rm -f ./dist/*