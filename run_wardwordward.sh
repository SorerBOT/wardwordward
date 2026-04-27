#!/bin/bash

gcc wardwordward.c -o wardwordward
echo "" | fzf --disabled --ansi --query="" --bind "change:reload(./wardwordward {q})"
