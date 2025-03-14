CC := g++
CXXFLAGS := -O3
LDFLAGS := -lmraa -lyaml-cpp -lgpiod

all: test test_yaml

icm20948_i2c.o:
	$(CC) $(CXXFLAGS) -c ../icm20948_i2c.cpp $(LDFLAGS)

icm20948_utils.o:
	$(CC) $(CXXFLAGS) -c ../icm20948_utils.cpp $(LDFLAGS)

gpioevent.o:
	$(CC) $(CXXFLAGS) -c ../gpioevent.cpp $(LDFLAGS)

test: icm20948_i2c.o icm20948_utils.o gpioevent.o
	$(CC) $(CXXFLAGS) test.cpp icm20948_i2c.o icm20948_utils.o gpioevent.o -I ../ -o test $(LDFLAGS)

test_yaml: icm20948_utils.o
	$(CC) $(CXXFLAGS) test_yaml.cpp icm20948_utils.o -I ../ -o test_yaml $(LDFLAGS)

.PHONY: clean
clean:
	@rm -f *.o
	@rm -f test
	@rm -f test_yaml