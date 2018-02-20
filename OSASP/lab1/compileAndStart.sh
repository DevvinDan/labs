gcc $1 -o $2 
if [ $? -eq 0 ]; then
    ./$2
else
    echo FAIL
fi
