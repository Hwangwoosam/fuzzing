cat:
	gcc -o fuzzer_cat test_cat.c ../src/fuzzer.c ../src/create_inp.c ../src/config.c ../../coverage/src/coverage.c

troff:
	gcc -o fuzzer_troff test_troff.c ../src/fuzzer.c ../src/create_inp.c ../src/config.c 

huns:
	gcc -o fuzzer_huns test_huns.c ../src/fuzzer.c ../src/create_inp.c ../src/config.c 

cgi:
	gcc -o fuzzer_cgi test_cgi.c  ../src/fuzzer.c ../src/create_inp.c ../src/config.c ../../coverage/src/coverage.c
clean:
	rm -rf fuzzer_cat fuzzer_troff fuzzer_huns fuzzer_cgi *.gcov *.gcno