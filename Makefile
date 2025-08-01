CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -MMD -MP -I.
LDFLAGS = -lsfml-system -lsfml-window -lsfml-graphics


SRCS = main.cpp Montserrat.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
TARGET = Snake

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f *.o *.d $(TARGET)

