<html>
<head></head>
<body style="font-size: 20px;">
<?php
	$array = [1, 2, 3, 5, 5, 3, "cat", 8, 0, "dog", "cat", 5];

    $ranks_rus = ["квинтиллион", "квадриллион", "триллион", "миллиард", "миллион", "тысяча", ""];
    $hundreds_rus = ["", "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"];
    $dozens_rus = ["", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто"];
    $special_ten_rus = ["десять", "одинадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "двеятнадцать"];
    $ones_rus = ["", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"];

    $ranks_eng = ["quintillion", "quadrillion", "trillion", "billion", "million", "thousand", ""];
    $dozens_eng = ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"];
    $special_ten_eng = ["ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"];
    $ones_eng = ["", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];



	$num = $_GET["number"];
    $length = strlen($num);

    if ($length > 20 || $length < 1){
        echo "Введите число от 1 до 20 разрядов";
        return;
    }

    if (!ctype_digit($num)){
        echo "Ошибка: введите положительное целое число без пробелов";
        return;
    }

    $num = str_pad($num, 21, '0', STR_PAD_LEFT);

    $result = "";

    if ($_GET["language"] == "rus"){

        for ($i = 0; $i < 7; $i++){
            $triplet = substr($num, 3 * $i, 3);
            if ($triplet == "000"){
                continue;
            }

            $result = $result.$hundreds_rus[$triplet[0]]." ";
            if ($triplet[1] == "1"){
                $result = $result.$special_ten_rus[$triplet[2]]." ";
                if ($i == 5){
                    $result = $result.$ranks_rus[$i];
                    $result = mb_substr($result, 0, -1, "UTF-8")." ";
                } else {
                    $result = $result.$ranks_rus[$i];
                    if ($i != 6){
                        $result = $result."ов ";
                    }

                }

            } else {
                $result = $result.$dozens_rus[$triplet[1]]." ";
                $result = $result.$ones_rus[$triplet[2]];

                if ($i == 5){
                    if ($triplet[2] == "1"){

                        $result = mb_substr($result, 0, -2, "UTF-8");
                        $result = $result."на";
                    } else if ($triplet[2] == "2"){
                        $result = mb_substr($result, 0, -1, "UTF-8");
                        $result = $result."е";
                    }
                }

                $result = $result." ";
                $result = $result.$ranks_rus[$i];

                if ($i == 6){
                    continue;
                }

                if ($triplet[2] == "1"){
                    $result = $result." ";
                } else if ($triplet[2] == "2" || $triplet[2] == "3" || $triplet[2] == "4"){
                    if ($i == 5){
                        $result = mb_substr($result, 0, -1, "UTF-8");
                        $result = $result."и ";
                    } else {
                        $result = $result."а ";
                    }
                } else {
                    if ($i == 5){
                        $result = mb_substr($result, 0, -1, "UTF-8");
                        $result = $result." ";
                    } else {
                        $result = $result."ов ";
                    }
                }
            }

        }

        if ($result == ""){
            $result = $result."ноль";
        }

        echo $result;

    } else if ($_GET["language"] == "eng"){

        for ($i = 0; $i < 7; $i++){

            $triplet = substr($num, 3 * $i, 3);
            if ($triplet == "000"){
                continue;
            }

            if ($triplet[0] != "0"){
                $result = $result.$ones_eng[$triplet[0]]." hundred ";
            }

            if ($triplet[1] == "1"){
                $result = $result.$special_ten_eng[$triplet[2]]." ";
                $result = $result.$ranks_eng[$i].", ";

            } else {
                $result = $result.$dozens_eng[$triplet[1]]." ";
                $result = $result.$ones_eng[$triplet[2]]." ";
                $result = $result.$ranks_eng[$i].", ";
            }

        }

        if ($result == ""){
            $result = $result."zero";
        } else {
            $result = substr($result, 0, -2);
        }

        echo $result;


    }

?>
</body>
</html>
