<?php
try {
    $pdo = new PDO('mysql:host=localhost;dbname=MyDataBase', 'root', '1480112');
}
catch (PDOException $e) {
    echo "Невозможно установить соединение с базой данных";
}

function printTableStructure ($tableDescription) {
    echo "<table border='1' cellspacing='0' width='800'>";
    echo "<tr>";
    echo "<td style=\"color: green;\">" . "Field " . "</td>";
    foreach ($tableDescription as $field) {
        echo "<td style=\"color: green;\">" . $field['Field'] . "</td>";
    }
    echo "</tr>";
    echo "<tr>";
    echo "<td>" . "Type" . "</td>";
    foreach ($tableDescription as $field) {
        echo "<td>" . $field['Type'] . "</td>";
    }
    echo "</tr>";
    echo "<tr>";
    echo "<td>" . "Key" . "</td>";
    foreach ($tableDescription as $field) {
        echo "<td>" . $field['Key'] . "</td>";
    }
    echo "</tr>";
    echo "</table>";
    return 0;
}

function printTable ($table) {

    echo "<table border='1' cellspacing='0' width='800'>";
    echo "<tr>";
    echo "<td style=\"color: green;\">" . "Fields" . "</td>";
    foreach ($table[0] as $key => $value) {
        echo "<td style=\"color: green;\">" . $key . "</td>";
    }
    echo "</tr>";
    foreach ($table as $record) {
        echo "<tr>";
        echo "<td></td>";
        foreach ($record as $field => $value) {
            echo "<td>" . $value . "</td>";
        }
        echo "</tr>";
    }
    echo "</table>";
    return 0;
}

try {
    $query = "SET CHARACTER SET 'UTF8'";
    $pdo->query($query);
    $query = "SET NAMES 'UTF8'";
    $pdo->query($query);

    $query = "SHOW TABLES";
    $statement = $pdo->query($query);
    $tables = $statement->fetchAll();
    foreach ($tables as $table) {
        $tableName = $table[0];

        echo "<h3 style=\"color: red;\">Table $tableName</h3>";

        $statement = $pdo->query("DESCRIBE $tableName");
        $result = $statement->fetchAll(PDO::FETCH_ASSOC);
        printTableStructure ($result);

        echo "<br><br>";

        $statement = $pdo->query("SELECT * FROM $tableName");
        $table = $statement->fetchAll(PDO::FETCH_ASSOC);
        printTable($table);

        echo "<br><hr><br>";


    }
} catch (PDOException $e) {
    echo "Ошибка выполнения запроса: " . $e->getMessage();
}
