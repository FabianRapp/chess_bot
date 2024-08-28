#!/bin/bash

while true; do
	./chess > a
	exit_status=$?
	if [ $exit_status -ne 0 ]; then
		echo "exit status: $exit_status"
		break
	fi
done
