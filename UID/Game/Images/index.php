<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link rel="shortcut icon" href="http://a.espncdn.com/combiner/i?img=/redesign/assets/img/icons/ESPN-icon-mma.png&h=80&w=80&scale=crop&cquality=40">

    <link rel="stylesheet" href="http://netdna.bootstrapcdn.com/bootstrap/3.3.2/css/bootstrap.min.css">
    <title>MMA club</title>
    <link href="./css/main.css" rel="stylesheet">
    <link href="./css/sticky-footer.css" rel="stylesheet">

    <style type="text/css">
      .fight-calendar{
        background-color: yellow;
      }
    </style>
  </head>

  <body>
    <div class="navbar navbar-default navbar-fixed-top" role="navigation">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">MMA club</a>
        </div>
        <div class="navbar-collapse collapse">
          <ul class="nav navbar-nav">
            <li class="active"><a href="#">Новости</a></li>
            <li><a href="#warriors">Бойцы</a></li>
            <li><a href="./lab2.php">Календарь</a></li>
          </ul>
        </div>
      </div>
    </div>

    <div class="container">

      <!--div class="page-header"-->
        <div class="row">
          <div class="text-center">
            <div class="col-xs-1">01</div>
            <div class="col-xs-1">02</div>
            <div class="col-xs-1">03</div>
            <div class="col-xs-1"><div class="fight-calendar">04</div></div>
            <div class="col-xs-1">05</div>
            <div class="col-xs-1">06</div>
            <div class="col-xs-1">07</div>
            <div class="col-xs-1">08</div>
            <div class="col-xs-1">09</div>
            <div class="col-xs-1">10</div>
            <div class="col-xs-1">11</div>
            <div class="col-xs-1">12</div>
        </div>
      </div>
      <!--/div-->


      <?php foreach($news as $new):?>
      <div class="new_block">
      <div class="row">
        <div class="col-sm-9">
          <h3><a href="<?php echo $new['href'];?>"><?php echo $new['title'];?></a></h3>
          <p><?php echo $new['desc'];?></p>
          <p class="text-muted">22.22.2222</p>
        </div>
        <div class="col-sm-3">
          <img class="img-responsive" src="<?php echo $new['img_src'];?>" alt="hi" width="300" height="150"> 
        </div>
      </div>
      </div>

      <?php endforeach;?>


    <div id="footer">
      <div class="container">
        <p class="text-muted">Что-то</p>
      </div>
    </div>


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"></script>
    <script src="http://netdna.bootstrapcdn.com/bootstrap/3.3.2/js/bootstrap.min.js"></script>
  </body>
</html>
