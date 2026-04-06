CC      = clang
SRCS    = $(wildcard src/*.c)
TARGET  = pietvm

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
