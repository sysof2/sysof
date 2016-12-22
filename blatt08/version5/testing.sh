#!/bin/bash
sum=0
testit() { 
	echo ./testit $@
	output="$(./testit $@)"
	points="$(<<<"$output" sed -n 's_Points.*: \(.*\)\..*_\1_p')" 
	echo $points
	sum=$((sum+points*6))
	
}

# run tests

for count in 10000 50000 100000 500000 1000000; do 
	for size_profile in uniform normal1 decrease; do 
		for alloc_profile in oneinthree cluster; do 
			testit 1 $count $size_profile $alloc_profile
		done 
	done 
	echo "Total points: $sum" 
done 
