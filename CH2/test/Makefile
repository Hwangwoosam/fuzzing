fuzzer_f: fuzzer_file.c ../src/fuzzer.c ../include/fuzzer.h
	gcc -o fuzzer_f fuzzer_file.c ../src/fuzzer.c 
check: check_test.c ../src/fuzzer.c ../include/fuzzer.h ../include/check.h ../src/check_err.c
	gcc -o check check_test.c ../src/fuzzer.c ../src/check_err.c
info : info_leak.c ../src/fuzzer.c ../include/fuzzer.h ../src/info_leak.c ../include/info.h
	gcc -o info info_leak.c ../src/fuzzer.c ../src/info_leak.c
clean:
	rm -rf fuzzer_f check info