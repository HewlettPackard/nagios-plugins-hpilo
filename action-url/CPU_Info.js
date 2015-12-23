function CPU_Table() {
	//document.write("<p>This is a Storage table.</p>");
	if (!String.prototype.trim) {
	  String.prototype.trim = function () {
		return this.replace(/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g, '');
	  };
	}	
		
	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>CPUs</caption>');
	document.write("</table>");
	
	var json = JSON.parse(jData);
	
	for(var j in json){
	document.write ("<br> </br>");
	if ( j != 0 && json[j].CpuSocketNum !=undefined ) {
			document.write("<table border=3; align='center'>");
			document.write ('<tr><th>' + "Socket Number" + '</th><td>' +  CheckUndefined(json[j].CpuSocketNum) + '</td></tr></thead>' );
			document.write ('<tr><th>' + "Cores" + '</th><td>' +  CheckUndefinedCPU(json[j].CpuCore) + '</td></tr></thead>' );
			if ( json[j].CpuName == undefined || json[j].CpuName.trim() === "") {
				document.write('<tr><th>' + "Processor Name" + '</th><td>' + "N/A" + '</td></tr></thead>');
			}
			else {
				document.write('<tr><th>' + "Processor Name" + '</th><td>' + json[j].CpuName.trim() + '</td></tr></thead>');
			}
			document.write('<tr><th>' + "Slot" + '</th><td>' + CheckUndefinedCPU(json[j].CpuSlot) + '</td></tr></thead>');
			document.write('<tr><th>' + "Status" + '</th><td>' + CPUConditionStat(json[j].CpuStat) + '</td></tr></thead>');
			document.write('<tr><th>' + "Max Threads" + '</th><td>' + CheckUndefined(json[j].CPUCoreMaxThreads) + '</td></tr></thead>');
			document.write('<tr><th>' + "CPU Speed " + '</th><td>' + CPUSpdCond(json[j].CpuSpeed) + '</td></tr></thead>');
			document.write('<tr><th>' + "CPU Max Speed " + '</th><td>' + CPUSpdCond(json[j].CPUMaxSpeed) + '</td></tr></thead>');
			document.write("</table>");
	}
	}
	document.write ("<br> </br>");

}

function CPUConditionStat (value) {
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
    case 5:
        return "Disabled";
        break;
	default: 
		return "N/A";
        }
}

function CheckUndefinedCPU (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
	case "":
		return "N/A";
        break;
	case 0:
		return "Can't be determined";
        break;
	default: 
		return value;
    }
}

function CPUSpdCond (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
	case "":
		return "N/A";
        break;
	case 0:
		return "Unavailable";
        break;
	default: 
		return value + " MHz" ;
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