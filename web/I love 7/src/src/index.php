<?php
    error_reporting(0);
    $digit = $_GET['digit'];
    if ((int) $digit) {
        $digit = $_GET['digit'];
        if (preg_match('/[<>`~\\\'()]/', $digit)) {
            http_response_code(403);
            die('403 Forbidden');
        }
    } else {
        $digit = "0";
    }
?>
<!DOCTYPE html>
<html>
    <head>
        <title>I love 7</title>
    </head>
    <body>
        <h1>7 * <?php echo htmlspecialchars($digit); ?></h1>
        <h3 class="result"></h3>
        <form action="/">
            <input type="text" name="digit" placeholder="digit">
            <input type="submit" value="Submit">
        </form>
        <script>
            var multiply = function(a, b) {
                return a * b;
            }

            var result = multiply(7, <?php echo $digit; ?>);

            document.querySelector('.result').textContent = 'The result is: ' + result;
        </script>
    </body>
</html>

