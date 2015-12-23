function Storage_Table() {
	//document.write("<p>This is a Storage table.</p>");
	if (!String.prototype.trim) {
	  String.prototype.trim = function () {
		return this.replace(/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g, '');
	  };
	}	
		
	document.write ("<br> </br>  <br> </br>");
	document.write("<table id='t01'; border=3; align='center'>");
	document.write('<caption>Storage</caption>');
	document.write("</table>");
	
	var json = JSON.parse(jData);
	
	for(var j in json){
	document.write ("<br> </br>");
	if ( j != 0) {
			document.write("<table border=3; align='center'>");
			document.write ('<tr><th>' + "Model" + '</th><td>' +  CntlrModel(json[j].st_Model) + '</td></tr></thead>' );
			if ( json[j].st_FWRev == undefined || json[j].st_FWRev.trim() === "") {
				document.write('<tr><th>' + "Firmware version" + '</th><td>' + "N/A" + '</td></tr></thead>');
			}
			else {
				document.write('<tr><th>' + "Firmware version" + '</th><td>' + json[j].st_FWRev.trim() + '</td></tr></thead>');
			}
			document.write('<tr><th>' + "Controller Condition " + '</th><td>' + ConditionStat(json[j].st_Cond) + '</td></tr></thead>');
			if (json[j].st_Prod_Rev == undefined || json[j].st_Prod_Rev.trim() == "") {
					document.write('<tr><th>' + "Product Revision" + '</th><td>' + "N/A" + '</td></tr></thead>');
			}
			else {
					document.write('<tr><th>' + "Product Revision" + '</th><td>' + json[j].st_Prod_Rev.trim() + '</td></tr></thead>');
			}
			if ( json[j].st_SerlNum == undefined || json[j].st_SerlNum.trim() === "") {
					document.write('<tr><th>' + "Serial Number" + '</th><td>' + "N/A" +'</td></tr></thead>');
			}
			else {
					document.write('<tr><th>' + "Serial Number" + '</th><td>' + json[j].st_SerlNum.trim() +'</td></tr></thead>');
			}			
			document.write('<tr><th>' + "Rebuild priority" + '</th><td>' + CntrlRePri(json[j].st_RebuildPriority) + '</td></tr></thead>');
			document.write('<tr><th>' + "Internal Ports" + '</th><td>' + CheckUndefined(json[j].st_NumOfInt_Ports) + '</td></tr></thead>');
			document.write('<tr><th>' + "Current Temperature" + '</th><td>' + CntrlTemp(json[j].st_Cur_Temp) + '</td></tr></thead>');
			document.write("</table>");
	}
	}
	document.write ("<br> </br>");

}

function CntlrModel (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "IDA";
        break;
    case 3:
        return "IDA Expansion";
        break;
    case 4:
        return "IDA-2";
        break;
    case 5:
        return "SMART ";
        break;
    case 6:
        return "SMART-2/E ";
        break;
    case 7:
        return "SMART-2/P ";
        break;
    case 8:
        return "SMART - 2SL";
        break;
    case 9:
        return "Smart - 3100ES";
        break;
    case 10:
        return "Smart - 3200";
        break;
    case 11:
        return "SMART - 2DH";
        break;
    case 12:
        return "Smart - 221";
        break;
    case 13:
        return "Smart Array 4250ES";
        break;
    case 14:
        return "Smart Array 4200";
        break;
    case 15:
        return "Integrated Smart Array";
        break;
    case 16:
        return "Smart Array 431";
        break;
    case 17:
        return "Smart Array 5300";
        break;
    case 18:
        return "RAID LC2 Controller";
        break;
    case 19:
        return "Smart Array 5i";
        break;
    case 20:
        return "Smart Array 532";
        break;
    case 21:
        return "Smart Array 5312";
        break;
    case 22:
        return "Smart Array 641";
        break;
    case 23:
        return "Smart Array 642";
        break;
    case 24:
        return "Smart Array 6400";
        break;
    case 25:
        return "Smart Array 6400 EM";
        break;
    case 26:
        return "Smart Array 6i ";
        break;
    case 27:
        return "Generic Array";
        break;
    case 28:
        return "Reserved ";
        break;
    case 29:
        return "Smart Array P600";
        break;
    case 30:
        return "Smart Array P400";
        break;
    case 31:
        return "Smart Array E200";
        break;
    case 32:
        return "Smart Array E200i";
        break;
    case 33:
        return "Smart Array P400i";
        break;
    case 34:
        return "Smart Array P800";
        break;
    case 35:
        return "Smart Array E500";
        break;
    case 36:
        return "Smart Array P700m";
        break;
    case 37:
        return "Smart Array P212";
        break;
    case 38:
        return "Smart Array P410";
        break;
    case 39:
        return "Smart Array P410i";
        break;
    case 40:
        return "Smart Array P411";
        break;
    case 41:
        return "Smart Array B110i SATA RAID";
        break;
    case 42:
        return "Smart Array P712m";
        break;
    case 43:
        return "Smart Array P711m";
        break;
    case 44:
        return "Smart Array P812";
        break;
    case 45:
        return "StorageWorks 1210m";
        break;
    case 46:
        return "Smart Array P220i";
        break;
    case 47:
        return "Smart Array P222";
        break;
    case 48:
        return "Smart Array P420";
        break;
    case 49:
        return "Smart Array P420i";
        break;
    case 50:
        return "Smart Array P421";
        break;
    case 51:
        return "Smart Array B320i";
        break;
    case 52:
        return "Smart Array P822";
        break;
    case 53:
        return "Smart Array P721m";
        break;
    case 54:
        return "Smart Array B120i";
        break;
    case 55:
        return "HP Storage p1224";
        break;
    case 56:
        return "HP Storage p1228";
        break;
    case 57:
        return "HP Storage p1228m";
        break;
    case 58:
        return "Smart Array P822se";
        break;
    case 59:
        return "HP Storage p1224e";
        break;
    case 60:
        return "HP Storage p1228e";
        break;
    case 61:
        return "HP Storage p1228em";
        break;
    case 62:
        return "Smart Array P230i";
        break;
    case 63:
        return "Smart Array P430i";
        break;
    case 64:
        return "Smart Array P430";
        break;
    case 65:
        return "Smart Array P431";
        break;
    case 66:
        return "Smart Array P731m";
        break;
    case 67:
        return "Smart Array P830i";
        break;
    case 68:
        return "Smart Array P830";
        break;
    case 69:
        return "Smart Array P831";
        break;
	default: 
		return "N/A";
        }
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

function CntrlRePri (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Low";
        break;
    case 3:
        return "Medium";
        break;
    case 4:
        return "High";
        break;
	default: 
		return "N/A";
        }
}

function CntrlTemp (value) {
    switch (value) {
    case 1:
        return "Other";
        break;
    case 2:
        return "Low";
        break;
    case 3:
        return "Medium";
        break;
    case 4:
        return "High";
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