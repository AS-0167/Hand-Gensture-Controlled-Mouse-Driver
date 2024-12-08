# Compiler and flags
CXX = g++
CXXFLAGS = -I/usr/include/opencv4
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio

# Target executable
TARGET = gesture_mouse
SRC = gesture_mouse.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	sudo $(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Run the program
run: $(TARGET)
	sudo ./$(TARGET)

# Clean up generated files
clean:
	rm -f $(TARGET)
