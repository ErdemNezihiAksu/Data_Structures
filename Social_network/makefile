all:compile run clean


compile: Social_network.c
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@gcc -o test Social_network.c SN_graph.c

run:
	@echo "-------------------------------------------"
	@echo "Running the tests...."
	@echo "======================================================================="
	@./test
	@echo "======================================================================="
	@echo "Completed tests...."

clean:
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	@rm -f *.o
	@rm -f test