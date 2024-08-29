#!/bin/bash

while true; do
	./chess
	exit_status=$?
	if [ $exit_status -nq 0] && [ $exit_status -nq 123]; then
		echo "exit with error $exit_status"
		break
	elif [ $exit_status -eq 123 ]; then
		echo "exit status: $exit_status"
		break
	fi
done
