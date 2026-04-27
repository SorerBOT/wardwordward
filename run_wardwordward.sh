#!/bin/bash

gcc wardwordward.c -o wardwordward -g -fsanitize=address
echo "" | fzf --disabled --ansi --query="" --bind "change:reload(./wardwordward {q})"
