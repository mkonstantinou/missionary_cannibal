NAME=mypipes
SRC=misscann.cpp
RM=rm -f
OUTPUT=missionary_cannibal
#
all: 
	g++ $(SRC) -o $(OUTPUT)
clean:
	-$(RM) *.o
	-$(RM) *~
	-$(RM) \#*
	-$(RM) *.swp
	-$(RM) *.core
fclean:clean
re:fclean all
