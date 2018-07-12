VPATH=app:bll:dal
OBJSDIR=obj
TARGET=main
OBJS=main.o process.o tcp_link.o message.o
RM=rm -f

$(TARGET):$(OBJSDIR) $(OBJS)
	g++ -o $(TARGET) $(OBJSDIR)/*.o
$(OBJS):%.o:%.cpp
	g++ -c $< -o $(OBJSDIR)/$@
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJSDIR)/*.o
