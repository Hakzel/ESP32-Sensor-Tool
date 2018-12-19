const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html><head>
                    <meta name='viewport' content='width=device-width, initial-scale=1'>
                    <meta name="theme-color" content="#232323">
                    <meta charset="utf-8">
                    <meta name="mobile-web-app-capable" content="yes">

<style>
                    body {background-color: #232323;
                                  color: white;
                                  }
                    .pagination {display: inline-block;
                                  margin: 0 auto;
                                  }
                    .pagination a { color: white;
                                    float: left;
                                    padding: 8px 16px;
                                    text-decoration: none;
                                    font-size: 16px;
                                   }

                    .pagination a.active { background-color: #343434;
                                    color: white;
                                    border-radius: 50px;
                                   }

                    .pagination a:hover:not(.active) {background-color: #ddd;
                                   border-radius: 5px;
                                   }
                    .button {background-color: #4CAF50; /* Green */
                                  border: none;
                                  color: white;
                                  padding: 4px 8px;
                                  border-radius: 5px;
                                  text-align: center;
                                  margin: 5px;
                                  text-decoration: none;
                                  display: inline-block;
                                  font-size: 14px;
                                  }
                    input[type=text] {background-color: #202020;
                                  color: white;
                                  border: 1px solid #4CAF50;
                                  border-radius: 5px;
                                  padding: 3px 5px;
                                  margin: 4px 0;
                                  box-sizing: border-box;
                                  }
                    input[type=number] {background-color: #202020;
                                  color: white;
                                  border: 1px solid #4CAF50;
                                  border-radius: 5px;
                                  padding: 3px 5px;
                                  margin: 4px 0;
                                  box-sizing: border-box;
                                 }
                    .content {    max-width: 500px;
                                  margin: auto;
                                  text-align: center;
                                  }                            
                    svg polyline {fill: none;
                                  stroke-dasharray: 1000;
                                  stroke-dashoffset: 1000;
                                  -webkit-animation: dash 1s linear forwards;
                                  animation: dash 1s linear forwards;
                                  }
                                  @-webkit-keyframes dash {
                                  to {
                                      stroke-dashoffset: 0;
                                     }
                                  }
                                  @keyframes dash {
                                  to {
                                      stroke-dashoffset: 0;
                                      }
                                  }
                    #mybutton {position: fixed;
                                  bottom: 10px;
                                  right: 10px;
                                  }
                    img {         width: auto;
                                  height: auto;
                                  max-width: 100%;
                                  }
                    </style>
                    </head>
                    
<body><div class="content">

<div class='pagination'>
  <a class='active'>Controlpage for the Multitalent</a>
</div><br>

<div> <!-- plot the graph here if available -->
	<span id="PlotPage"></span><br>
</div>
<br>
<div>
 <table align="center" style ="border-collapse: collapse;"><tr >
  <td style="border-right: 1px solid gray;border-bottom: 1px solid gray;" align='right'><button class="button" type="button"  onclick="sendData('GetPlot',1,'PlotPage')">show plot</button></td>
  <td style="border-bottom: 1px solid gray;" align='left'><button class="button" type="button"  onclick="sendData('GetPlot',0,'PlotPage')">hide plot</button><BR></td>
  </tr><tr>
  <td style="border-right: 1px solid gray;" align='right'><button class="button" type="button" onclick="sendData('GetControls',1,'ControlPage')">show controls</button></td>
  <td align='left'><button class="button" type="button" onclick="sendData('GetControls',0,'ControlPage')">hide controls</button><BR></td>
 </tr></table>
</div>
<br>
<div> <!-- plot the controls here if available -->
  <span id="ControlPage"></span><br>
</div>
 
<div>
	          <a><button class="button" onclick="sendData('GetDataLog',50,'DataLog')"> start datalogging </button></a> 
            <a><button class="button" onclick="sendData('GetDataLog',52,'DataLog')"> pause </button></a>
            <a><button class="button" onclick="sendData('GetDataLog',51,'DataLog')"> stop datalogging </button></a> 
            <br>
            Datalogging: <span style="color:#4CAF50" id="DataLog">datalogging inactive</span><br> 
            Filename: <span style="color:#4CAF50" id="DataLogName">Datalog</span><br>
            Last Comment: <span style="color:#4CAF50" id="DataComment">NA</span><br>
</div>


    <table align="center">
      <tr>
        <td align='left'> file name for data logging:</td> 
        <td align='left'><form><input type='text' size='15' id="DataFileName" placeholder="Datalog"></td>
        <td align='left'> <input type="button" onclick="SendString('DataFileName','DataLogName')" class="button"></form>
      </tr>
      <tr><form>
        <td align='left'> comment while logging:</td> 
        <td align='left'> <input type='text'  size='15' id="DataFileComment" placeholder="Comment"> </td>
        <td align='left'> <input type="button" onclick="SendString('DataFileComment','DataComment')" class="button"></form>
      </tr>
      <tr>
     </tr>
  </table>

<div>
  <br>
  <a><button class="button" onclick="sendData('GetFileLinks',1,'FileLinks')"> show Datalogs </button></a>
  <a><button class="button" onclick="sendData('GetFileLinks',2,'FileLinks')"> hide Datalogs </button></a>
  <span id="FileLinks"></span><br>
</div>

<br><br><br><br>

</div>

<script>
function sendData(Adress,Value,Replace_ID) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById(Replace_ID).innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "GetData?"+Adress+"="+Value, true);
  xhttp.send();
}

function SendString(Name,Replace_ID){
    var input = document.getElementById(Name).value;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById(Replace_ID).innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "GetString?"+Name+"="+input , true);
    xhttp.send();
}

<!--
setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 500000); //2000mSeconds update rate
 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("PLOTpage").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "getPlot", true);
  xhttp.send();
}
-->
</script>
</body>
</html>
)=====";
