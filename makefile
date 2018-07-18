VPATH=app:bll:dal
OBJSDIR=obj
TARGET1=main
TARGET2=test
OBJS1=main.o process.o tcp_link.o message.o manage.o usr.o
OBJS2=test.o process.o tcp_link.o message.o manage.o usr.o
RM=rm -f

$(TARGET1):$(OBJSDIR) $(OBJS1)
	g++ -o $(TARGET1) $(OBJSDIR)/*.o
$(OBJS1):%.o:%.cpp
	g++ -c $< -o $(OBJSDIR)/$@
clean:
	-$(RM) $(TARGET1)
	-$(RM) $(OBJSDIR)/*.o
