for argument in "$*";
do
	echo "$argument";
	echo "$argument" >> somefile;
done
