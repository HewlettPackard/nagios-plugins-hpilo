function Memory_Table() {
	
	var json = JSON.parse(jData);

	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>Memory Summary</caption>');
	document.write ("<tr> </tr>");
	document.write('<tr><th>' + 'Location' + '</th><th>' + 'Status'   + '</th><th>' + 'Present' + '</th><th>' + 'Locked' +	'</th><th>' + 'Hot Plug' + '</th><th>' + 'Total Memory' + '</th><th>' +  'In Use by OS' +  '</th><th>' +  'Op Frequency' +  '</th><th>' +  'Op Voltage' + '</th></tr>' );


	for(var j in json){
	  if ( j != 0 && json[j].Mem2BoardCpuNum !=undefined ) {
		document.write('<tr><td>' + "CPU " + json[j].Mem2BoardCpuNum + '</td><td>' + MemErrStat(json[j].Mem2BoardErrorStat)   + '</td><td>' + MemPresentStat(json[j].Mem2BoardOnlineStat) + '</td><td>' + MemLock(json[j].Mem2BoardLocked) +	'</td><td>' + MemHotplug(json[j].Mem2BoardHotPlug) + '</td><td>' + CheckUndefinedMem(json[j].Mem2BoardTotalMemSize) + '</td><td>' +  CheckUndefinedMem(json[j].Mem2BoardOsMemSize) + '</td><td>' +  MemOpFreq(json[j].Mem2BoardOper_Freq) + '</td><td>' +  MemVolts(json[j].Mem2BoardOper_Volt) + '</td></tr>' );
		}
	}

	document.write("</table>");
	document.write ("<br/> </br> ");
}


function MemErrStat (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "noError";
        break;
    case 3:
        return "dimmEccError";
        break;
    case 4:
        return "unlockError";
        break;
    case 5:
        return "configError";
        break;
    case 6:
        return "busError";
        break;
    case 7:
        return "powerError";
        break;
    case 8:
        return "advancedEcc";
        break;
    case 9:
        return "onlineSpare";
        break;
    case 10:
        return "mirrored";
        break;
    case 11:
        return "mirroredDimmError";
        break;
    case 12:
        return "memoryRaid";
        break;
    case 13:
        return "raidDimmError";
        break;
    case 14:
        return "lockStep";
        break;
    case 15:
        return "lockStepError";
        break;
	default: 
		return "N/A";
    }
}

function MemPresentStat (value) {
    switch (value) {
    case 1:
        return "Other";
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

function MemLock (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "unlocked";
        break;
    case 3:
        return "locked";
        break;
	default: 
		return "N/A";
    }
}				

function MemHotplug (value) {
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

function CheckUndefinedMem (value) {
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
		return value/1024 + " GB";
    }
}
	
function MemOpFreq (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
	case "":
		return "N/A";
        break;
	default: 
		return value + " MHz" ;
    }
}	

function MemVolts (value) {
    switch (value) {
    case undefined:
        return "N/A";
        break;
	case "":
		return "N/A";
        break;
	default: 
		return value/1000 + " V";
    }
}

function goBack() {
    window.history.back();
}
