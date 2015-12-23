function Temp_Table() {
	
	var json = JSON.parse(jData);

	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>Temperature Sensors</caption>');
	document.write ("<tr> </tr>");
	document.write('<tr><th>' + 'Sensor' + '</th><th>' + 'Location'   + '</th><th>' + 'Temperature' + '</th><th>' + 'Threshold' +	'</th><th>' + 'Sensor Condition' +	'</th><th>' + 'Reaction Type' + '</th></tr>' );


	for(var j in json){
	  if ( j != 0 && json[j].TmpInd !=undefined && json[j].TmpLocale != undefined ) {
			document.write('<tr><td>' + json[j].TmpInd + '</td><td>' + tempLoc(json[j].TmpLocale)   + '</td><td>' + CheckUndefined(json[j].TmpCelsius) + '</td><td>' + CheckUndefined(json[j].TmpThres) +	'</td><td>' + tempCondition(json[j].TmpCond) + '</td><td>' + tempThresholdType(json[j].TmpThresType) + '</td></tr>' );
	  }
	}

	document.write("</table>");
	document.write ("<br/> </br> ");
}

function CheckUndefined (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
	case "":
		return "N/A";
        break;
	default: 
		return value;
    }
}


function tempLoc (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Unknown";
        break;
    case 3:
        return "System";
        break;
    case 4:
        return "SystemBoard";
        break;
    case 5:
        return "IoBoard";
        break;
    case 6:
        return "CPU";
        break;
    case 7:
        return "Memory";
        break;
    case 8:
        return "Storage";
        break;
    case 9:
        return "Removable Media";
        break;
    case 10:
        return "Power Supply";
        break;
    case 11:
        return "Ambient";
        break;
    case 12:
        return "Chassis";
        break;
    case 13:
        return "BridgeCard";
        break;
	default: 
		return "N/A";
    }
}


function tempCondition (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "OK";
        break;
    case 3:
        return "Degraded";
        break;
    case 4:
        return "Failed";
        break;
	default: 
		return "N/A";
    }
}			
			
	
			
function tempThresholdType (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Blowout";
        break;
    case 3:
        return "Caution";
        break;
    case 4:
        return "Critical";
        break;
    case 5:
        return "No reaction";
        break;
	default: 
		return "N/A";
    }
}				
