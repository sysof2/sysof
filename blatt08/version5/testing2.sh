#!/bin/bash
sum=0
testit() { 
	echo ./testit $@
	output="$(./testit $@)"
	points="$(<<<"$output" sed -n 's_Points.*: \(.*\)\..*_\1_p')" 
	echo $points
	sum=$((sum+points))
	
}

# run tests

for count in 5000 10000 25000 50000 75000 100000 250000 500000 750000 1000000; do 
	for size_profile in uniform normal1 fixed8 fixed16 fixed24 fixed104 fixed200 increase decrease; do 
		for alloc_profile in oneinthree cluster; do 
			testit 1 $count $size_profile $alloc_profile
		done 
	done 
	echo "Total points: $sum" 
done 
