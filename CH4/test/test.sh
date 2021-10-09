#!/bin/sh

start_time=$(date +%s)
for i in 1 2 3 4 5 6 7 8 9 10 
do  
	rm ./seed_dir/*_new_seed
	cp ../seed_dir_origin/* ./seed_dir/ 
	./cjson_fuzzer
	cp TestResult.csv TestResult$i.csv
done
end_time=$(date +%s)

echo "It takes $(($end_time-$start_time)) second!"