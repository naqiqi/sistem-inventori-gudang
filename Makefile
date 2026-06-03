CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
SRC      = src/main.cpp src/hash_table.cpp src/bst.cpp
TARGET   = inventori

all: $(TARGET)

$(TARGET): $(SRC) src/product.h src/sorted_vector.h src/file_manager.h src/data_generator.h
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
	@echo "Build berhasil: ./$(TARGET)"

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
