function PS_Table() {
	//document.write("<p>This is a PS table.</p>");
	if (!String.prototype.trim) {
	  String.prototype.trim = function () {
		return this.replace(/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g, '');
	  };
	}	
		
	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>Power Supply</caption>');
	document.write("</table>");
	
	var json = JSON.parse(jData);
	
	for(var j in json){
	document.write ("<br> </br>");
	if ( j != 0) {
			document.write("<table border=3; align='center'>");
			document.write ('<tr><th>' + "Power Supply Bay" + '</th><td>' +  j + '</td></tr></thead>' );
			document.write('<tr><th>' + "Status" + '</th><td>' + PresentStat(json[j].Present) + '</td></tr></thead>');
			document.write('<tr><th>' + "Condition " + '</th><td>' + ConditionStat(json[j].Condition) + '</td></tr></thead>');
			if ( json[j].Model == undefined || json[j].Model.trim() === "") {
				document.write('<tr><th>' + "Model" + '</th><td>' + "N/A" + '</td></tr></thead>');
			}
			else {
				document.write('<tr><th>' + "Model" + '</th><td>' + json[j].Model.trim() + '</td></tr></thead>');
			}
			if ( json[j].SerialNumber == undefined || json[j].SerialNumber.trim() === "") {
				document.write('<tr><th>' + "Serial Number" + '</th><td>' + "N/A" +'</td></tr></thead>');
			}
			else {
				document.write('<tr><th>' + "Serial Number" + '</th><td>' + json[j].SerialNumber.trim() +'</td></tr></thead>');
			}
			if (json[j].SparePartNum == undefined ||  json[j].SparePartNum.trim() == "") {
				document.write('<tr><th>' + "Spare Part Number" + '</th><td>' + "N/A" + '</td></tr></thead>');
			}
			else {
				document.write('<tr><th>' + "Spare Part Number" + '</th><td>' + json[j].SparePartNum.trim() + '</td></tr></thead>');
			}
			document.write('<tr><th>' + "Main Voltage" + '</th><td>' + CheckUndefined(json[j].MainVoltage) + " volts" + '</td></tr></thead>');
			document.write('<tr><th>' + "Maximum Capacity" + '</th><td>' + CheckUndefined(json[j].CapacityMaximum) +" watts" +'</td></tr></thead>');			
			if (json[j].FirmwareRev == undefined ||  json[j].FirmwareRev.trim() == "") {
				document.write('<tr><th>' + "Firmware Revision" + '</th><td>' + "N/A" +'</td></tr></thead>');
			}
			else {
				document.write('<tr><th>' + "Firmware Revision" + '</th><td>' + json[j].FirmwareRev.trim() +  '</td></tr></thead>');
			}
			document.write("</table>");
	}
	}
	document.write ("<br> </br>");

}

function ConditionStat (value) {
    switch (value) {
    case 1:
        return "Unknown";
        break;
    case 2:
        return "Ok";
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