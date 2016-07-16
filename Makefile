LIBS = src/cli src/command

default: libraries
	g++ $(wildcard libs/*) -std=c++11 -o DeepOrange;

debug: librariesdebug
	g++ -g $(wildcard libs/*) -std=c++11 -o DeepOrange;

libraries:
	mkdir libs;
	for dir in $(LIBS); do \
		cd $$dir; \
		g++ -std=c++11 -c *.cpp; \
		mv *.o ../../libs; \
		cd -; \
	done
	g++ -std=c++11 -c src/cli/main.cpp;
	mv main.o libs/main.o;

librariesdebug:
	mkdir libs;
	for dir in $(LIBS); do \
		cd $$dir; \
		g++ -std=c++11 -c -g *.cpp; \
		mv *.o ../../libs; \
		cd -; \
	done
	g++ -std=c++11 -c -g src/cli/main.cpp;
	mv main.o libs/main.o;

clean:
	rm -rf libs;
	rm -f DeepOrange;
