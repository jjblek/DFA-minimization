COMPILER=g++
FLAGS=-std=c++17
COMPILE=$(COMPILER) $(FLAGS)
RM=rm -f

EXE=DFA.x

OBJS=main.o DFA.o

$(EXE): $(OBJS)
	$(COMPILER) -o $@ $(OBJS)

.cpp.o:
	$(COMPILE) -c $<

clean:
	@echo "Cleaning...";
	$(RM) *.o $(EXE)