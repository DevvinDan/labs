<?php
	$array = [1, 2, 3, 5, 5, 3, "cat", 8, 0, "dog", "cat", 5];

	$exists = [];
	foreach($array as $value){
		if (!in_array($value, $exists)){
			array_push($exists, $value);
			echo $value." ";
		}
	}
