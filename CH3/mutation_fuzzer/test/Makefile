test:
	gcc -o test test.c ../src/create_inp.c ../src/coverage.c ../src/fuzzer.c ../src/config.c
troff:
	gcc -o s_troff troff_test.c ../src/create_inp.c ../src/coverage.c ../src/fuzzer.c ../src/config.c
bc:
	gcc -o bc bc_test.c ../src/create_inp.c ../src/coverage.c ../src/fuzzer.c ../src/config.c
flip_fuzzer:
	gcc -o flip_fuzzer flip_fuzzer_test.c ../src/create_inp.c ../src/coverage.c ../src/fuzzer.c ../src/config.c
flip:
	gcc --coverage  -o flip flip_random.c ../src/create_inp.c
clean:
	rm -rf test bc s_troff *.gcov *.gcda *.gcno flip flip_fuzzer tmp.* ./gcovtest/*.gcov ./gcovtest/*.gcda ./gcovtest/*.gcno