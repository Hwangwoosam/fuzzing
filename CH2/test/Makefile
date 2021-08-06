fuzzer: fuzzer_file.c ../src/fuzzer.c ../include/fuzzer.h
	gcc -o fuzzer 0_fuzzer_file.c ../src/fuzzer.c 

bug_fuzzer: bug_fuzzer_find.c ../src/fuzzer.c ../include/fuzzer.h ../include/bug_fuzzer.h ../src/bug_fuzzer.c
	gcc -o bug_fuzzer 1_bug_fuzzer_find.c ../src/fuzzer.c ../src/bug_fuzzer.c

info : info_leak.c ../src/fuzzer.c ../include/fuzzer.h ../src/info_leak.c ../include/info.h
	gcc -o info 2_info_leak.c ../src/fuzzer.c

clean:
	rm -rf fuzzer_f check info