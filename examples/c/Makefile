CFLAGS_LEMMATIZER=`pkg-config --cflags lemmatizer`
LIBS_LEMMATIZER=`pkg-config --libs lemmatizer`
OBJ=firstform.o

%.o: %.c
	$(CC) $(CFLAGS_LEMMATIZER) -c -o $@ $<

firstform: $(OBJ)
	$(CC) -o $@ $^ $(LIBS_LEMMATIZER)

.PHONY: clean

clean:
	rm -f *.o *~ core firstform 
