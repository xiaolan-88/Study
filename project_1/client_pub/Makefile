OBJS := sem.o shm.o shmfifo.o client.o main.o

client:$(OBJS)
	@gcc $^ -o $@ -lpthread
	@echo "Done."

%.o : %.c
	@gcc -c $< -o $@

clean:
	@rm *.o client
	@echo "Clean."

