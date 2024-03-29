TARGET = asrrgb

SRCDIR = src
INCDIR = src/include

CPPFLAGS = -I$(INCDIR) -std=c++17 -O3 `pkg-config --cflags libusb-1.0` -Wall
LDFLAGS = -lboost_program_options `pkg-config --libs libusb-1.0`

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp*
	@echo "Compiling $<.."
	@$(CXX) -c $< -o $@ $(CPPFLAGS)

clean:
	@rm -f $(TARGET)
	@rm -f $(SRCDIR)/*.o

.PHONY: clean
