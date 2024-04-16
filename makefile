CFLAGS = -g -Wall
hw4: hw4.o GSCImage.o GSCPixel.o Pixel.o RGBImage.o RGBPixel.o Token.o
	g++ $(CFLAGS) hw4.o GSCImage.o GSCPixel.o Pixel.o RGBImage.o RGBPixel.o Token.o -o hw4 -fsanitize=address 

hw4.o: hw4.cpp
	g++ $(CFLAGS) -c hw4.cpp 

GSCImage.o: Image.hpp GSCImage.cpp GSCImage.hpp
	g++ $(CFLAGS) -c GSCImage.cpp

GSCPixel.o: GSCPixel.cpp GSCPixel.hpp
	g++ $(CFLAGS) -c GSCPixel.cpp

Pixel.o: Pixel.cpp Pixel.hpp 
	g++ $(CFLAGS) -c Pixel.cpp

RGBImage.o: Image.hpp RGBImage.cpp RGBImage.hpp
	g++ $(CFLAGS) -c RGBImage.cpp

RGBPixel.o: RGBPixel.cpp RGBPixel.hpp
	g++ $(CFLAGS) -c RGBPixel.cpp

Token.o: Token.cpp
	g++ $(CFLAGS) -c Token.cpp

clean: 
	rm *.o hw4
		
