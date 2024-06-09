CC = clang++
CFLAGS = -std=c++17 -Wall -Werror

all: final

final: main.o cam.o vec.o ray.o obj.o interval.o
	cd ../build && \
	$(CC) $(CFLAGS) \
	main.o \
	cam.o \
	vec.o \
	ray.o \
	obj.o \
	interval.o \
	-o main && \
	cd ../src

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o ../build/main.o

cam.o: camera.cpp
	$(CC) $(CFLAGS) -c camera.cpp -o ../build/cam.o

vec.o: vec3.cpp
	$(CC) $(CFLAGS) -c vec3.cpp -o ../build/vec.o

ray.o: ray.cpp
	$(CC) $(CFLAGS) -c ray.cpp -o ../build/ray.o

obj.o: object.cpp
	$(CC) $(CFLAGS) -c object.cpp -o ../build/obj.o

interval.o: interval.cpp
	$(CC) $(CFLAGS) -c interval.cpp -o ../build/interval.o
