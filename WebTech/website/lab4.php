<html>
<head>
	<link rel="stylesheet" href="lab4.css">
</head>
<body style="font-size: 30px">

<?php

function highlight_uppercase($array){
    foreach($array as $data){
        $data = "<span class=\"uppercase\">".$data."</span>";
    }
    return $data;
}

function highlight_abbreviation($array){
    foreach($array as $data){
        $data = "<span class=\"abbreviation\">".$data."</span>";
    }
    return $data;
}

function highlight_numbers($array){
    foreach($array as $data){
        $data = "<span class=\"number\">".$data."</span>";
    }
    return $data;
}

$string = $_GET["text"];
$string = $string." ";
mb_regex_encoding("utf-8");
mb_internal_encoding("utf-8");
$string = mb_ereg_replace_callback('([A-ZА-Я]+[A-Za-zА-Яа-я]*[ ,])', 'highlight_uppercase', $string);
$string = mb_ereg_replace_callback('([A-ZА-Я][A-ZА-Я]+)', 'highlight_abbreviation', $string);
$string = mb_ereg_replace_callback('([0-9]+)', 'highlight_numbers', $string);
echo $string;

?>

</body>
</html>
