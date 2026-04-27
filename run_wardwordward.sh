#!/bin/bash

gcc ./src/wardwordward.c -o ./bin/wardwordward
echo "" | fzf --disabled --ansi --query="" --bind "change:reload(./bin/wardwordward {q})"
