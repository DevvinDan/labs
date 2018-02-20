cd /
grep -rl "$1" $2 | xargs stat --printf="File: %n Size: %s\n"
