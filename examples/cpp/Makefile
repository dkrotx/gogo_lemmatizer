CFLAGS_LEMMATIZER=`pkg-config --cflags lemmatizer`
LIBS_LEMMATIZER=`pkg-config --libs lemmatizer`
OBJ=lemmatize.o

%.o: %.cpp
	$(CXX) $(CFLAGS_LEMMATIZER) -c -o $@ $<

lemmatize: $(OBJ)
	$(CXX) -o $@ $^ $(LIBS_LEMMATIZER)

.PHONY: clean

clean:
	rm -f *.o *~ core lemmatize
