LIBS = src/cli src/command src/controls src/misc

DeepOrange: libraries
	gcc $(wildcard libs/*) -o DeepOrange;

libraries:
	mkdir libs;
	for dir in $(LIBS); do \
		cd $$dir; \
		gcc -c *.c; \
		mv *.o ../../libs; \
		cd -; \
	done
	gcc -c src/cli/main.c;
	mv main.o libs/main.o;

clean:
	rm -rf libs;
	rm -f DeepOrange;
