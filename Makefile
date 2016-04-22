LIBS = src/cli src/command src/misc src/queue

install: libraries
	gcc $(wildcard libs/*) -o DeepOrange;

debug: librariesdebug
	gcc -g $(wildcard libs/*) -o DeepOrange;

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

librariesdebug:
	mkdir libs;
	for dir in $(LIBS); do \
		cd $$dir; \
		gcc -c -g *.c; \
		mv *.o ../../libs; \
		cd -; \
	done
	gcc -c -g src/cli/main.c;
	mv main.o libs/main.o;

clean:
	rm -rf libs;
	rm -f DeepOrange;
