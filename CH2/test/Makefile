fuzzer: 0_fuzzer_file.c ../src/0_fuzzer.c ../include/0_fuzzer.h
	gcc -o fuzzer 0_fuzzer_file.c ../src/0_fuzzer.c 

bug_fuzzer: 1_bug_fuzzer_find.c ../src/0_fuzzer.c ../include/0_fuzzer.h ../include/1_bug_fuzzer.h ../src/1_bug_fuzzer.c
	gcc -o bug_fuzzer 1_bug_fuzzer_find.c ../src/0_fuzzer.c ../src/1_bug_fuzzer.c

info : 2_info_leak.c ../src/0_fuzzer.c ../include/0_fuzzer.h ../src/2_info_leak.c ../include/2_info.h
	gcc -o info 2_info_leak.c ../src/0_fuzzer.c

program_checker: 3_program_checker.c ../src/3_program_checker.c ../include/3_program_checker.h
	gcc -o p_checker 3_program_checker.c ../src/3_program_checker.c

clean:
	rm -rf fuzzer_f check info