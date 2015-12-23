function Fan_Table() {
	
	var json = JSON.parse(jData);

	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>Fault Tolerant Fans</caption>');
	document.write ("<tr> </tr>");
	document.write('<tr><th>' + 'Fan' + '</th><th>' + 'Location'   + '</th><th>' + 'Type' + '</th><th>' + 'Present' +	'</th><th>' + 'HotPluggable' + '</th><th>' + 'Speed' + '</th><th>' +  'Redundancy state' + '</th></tr>' );


	for(var j in json){
	  if ( j != 0 && json[j].FanIndex !=undefined  && json[j].FanLocale !=undefined ) {
		document.write('<tr><td>' + json[j].FanIndex + '</td><td>' + fanLoc(json[j].FanLocale)   + '</td><td>' + TypeOfFan(json[j].FanType) + '</td><td>' + PresentStat(json[j].FanPresent) +	'</td><td>' + fanplug(json[j].FanHotPlug) + '</td><td>' + fanSpd(json[j].FanSpeed) + '</td><td>' +  fanRedun(json[j].FanRedundant) + '</td></tr>' );
		}
	}

	document.write("</table>");
	document.write ("<br/> </br> ");
}

function TypeOfFan (value) {
    switch (value) {
    case 1:
        return "Unknown";
        break;
    case 2:
        return "Tach Output";
        break;
    case 3:
        return "Spin Detect";
        break;
	default: 
		return "N/A";
    }
}

function PresentStat (value) {
    switch (value) {
    case 1:
        return "Unknown";
        break;
    case 2:
        return "Absent";
        break;
    case 3:
        return "Present";
        break;
	default: 
		return "N/A";
    }
}

function fanLoc (value) {
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
        return "Cpu";
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
    case 14:
        return "ManagementBoard";
        break;
    case 15:
        return "Backplane";
        break;
    case 16:
        return "NetworkSlot";
        break;
    case 17:
        return "BladeSlot";
        break;
    case 18:
        return "Virtual";
        break;
	default: 
		return "N/A";
    }
}

			
function fanplug (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Non Hot Plug";
        break;
    case 3:
        return "Hot Plug";
        break;
	default: 
		return "N/A";
    }
}			
			

function fanSpd (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Normal";
        break;
    case 3:
        return "High";
        break;
	default: 
		return "N/A";
    }
}				

			
function fanRedun (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Non Redundant";
        break;
    case 3:
        return "Redundant";
        break;
	default: 
		return "N/A";
    }
}				



function goBack() {
    window.history.back();
}
