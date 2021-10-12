#!/bin/sh

start_time=$(date +%s)
for i in 1 2 3 4 5 
do  
	./cjson_fuzzer
	rm ./seed_dir/*_new_seed
	cp ./seed_dir_origin/* ./seed_dir/* 
	cp result_summary.csv TestResult$i.csv
done
end_time=$(date +%s)

echo "It takes $(($end_time-$start_time)) second!"