CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
TARGET = scheduler
SRCDIR = src
OBJDIR = build/obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

.PHONY: all clean report

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

report:
	cd report && xelatex main.tex && xelatex main.tex

clean:
	rm -rf $(OBJDIR) $(TARGET)

clean-report:
	rm -f report/main.aux report/main.log report/main.out report/main.toc report/main.lof report/main.lot report/main.pdf
