#!/usr/bin/env bash

echo -ne "\0" | cat $1 - | brainwhat $2
