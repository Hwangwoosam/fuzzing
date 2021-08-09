fuzzer: 0_fuzzer_file.c ../src/0_fuzzer.c ../include/0_fuzzer.h
	gcc -o fuzzer 0_fuzzer_file.c ../src/0_fuzzer.c 

bug_fuzzer: 1_bug_fuzzer_find.c ../src/0_fuzzer.c ../include/0_fuzzer.h ../include/1_bug_fuzzer.h ../src/1_bug_fuzzer.c
	gcc -o bug_fuzzer 1_bug_fuzzer_find.c ../src/0_fuzzer.c ../src/1_bug_fuzzer.c

info : 2_info_leak.c ../src/0_fuzzer.c ../include/0_fuzzer.h ../src/2_info_leak.c ../include/2_info.h
	gcc -o info 2_info_leak.c ../src/0_fuzzer.c ../src/2_info_leak.c

program_checker: 3_program_checker.c ../src/3_program_checker.c ../include/3_program_checker.h
	gcc -o p_checker 3_program_checker.c ../src/3_program_checker.c

run: 4_runner_test.c ../src/4_runner.c ../include/4_runner.h
	gcc -o runner 4_runner_test.c ../src/4_runner.c

program_runner: 5_program_runner.c ../src/5_program_runner.c ../include/5_program_runner.h
	gcc -o p_runner 5_program_runner.c ../src/5_program_runner.c

fuzzer_class: 6_fuzzer_class.c ../src/0_fuzzer.c ../src/6_fuzzer_class.c ../src/5_program_runner.c ../include/0_fuzzer.h ../include/5_program_runner.h ../include/6_fuzzer_class.h
	gcc -o fuzzer_c 6_fuzzer_class.c ../src/0_fuzzer.c ../src/5_program_runner.c ../src/6_fuzzer_class.c

clean:
	rm -rf fuzzer check info runner p_runner fuzzer_c