function Network_Table() {
	
	var json = JSON.parse(jData);

	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>Network Summary</caption>');
	document.write ("<tr> </tr>");
	document.write('<tr><th>' +  'Port' + '</th><th>' + 'Model' + '</th><th>' + 'Status'   + '</th><th>' + 'Duplex' + '</th><th>' + 'Base I/O Address' +	'</th><th>' + 'Slot' + '</th><th>' + 'Mac Address' + '</th><th>' +  ' Firmware Version' + '</th></tr>' );


	for(var j in json){
	  if ( j != 0 && json[j].NIPANam !=undefined && json[j].NIPANam !="" ) {
		document.write('<tr><td>' + NetPort(json[j].NIPAPort) + '</td><td>' + CheckUndefined(json[j].NIPANam) + '</td><td>' + NetworkStat(json[j].NIPAStatus)   + '</td><td>' + NetDupStat(json[j].NIPADuplexState) + '</td><td>' + NetIO(json[j].NIPAIoAddr) +	'</td><td>' + NetSlot(json[j].NIPASlot) + '</td><td>' + CheckUndefined(json[j].NIPAMACAddr) + '</td><td>' +  CheckUndefined(json[j].NIPAFWVer) + '</td></tr>' );
		}
	}

	document.write("</table>");
	document.write ("<br/> </br> ");
}

function NetworkStat (value) {
    switch (value) {
    case 1:
        return "Unknown";
        break;
    case 2:
        return "Ok";
        break;
    case 3:
        return "GeneralFailure";
        break;
    case 4:
        return "LinkFailure";
        break;
	default: 
		return "N/A";
    }
}

function NetDupStat (value) {
    switch (value) {
    case 1:
        return "Unknown";
        break;
    case 2:
        return "Half";
        break;
    case 3:
        return "Full";
        break;
	default: 
		return "N/A";
    }
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

function NetIO (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
    case "":
        return "N/A";
        break;
	case 0:
		return "N/A";
        break;
	default: 
		return value;
    }
}

function NetPort (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
    case -1:
        return "N/A";
        break;
	case "":
		return "N/A";
        break;
	default: 
		return value;
    }
}


function NetSlot (value) {
    switch (value) {
    case 0:
        return "Embedded";
        break;
	default: 
		return "N/A";
    }
}

function goBack() {
    window.history.back();
}
