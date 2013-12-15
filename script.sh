#!/bin/bash

CurrentKernelVer=31

while [ "$CurrentKernelVer" -ne 101 ]; do
	y=$(($CurrentKernelVer + 1))	
	wget https://www.kernel.org/pub/linux/kernel/v3.x/incr/patch-3.0.$CurrentKernelVer-$y.gz -O ../patch-3.0.$CurrentKernelVer-$y.gz
	gunzip -d ../patch-3.0.$CurrentKernelVer-$y.gz
	patch -p1 < ../patch-3.0.$CurrentKernelVer-$y
	rm ../patch-3.0.$CurrentKernelVer-$y
	CurrentKernelVer=$(( $CurrentKernelVer + 1 ))
done

