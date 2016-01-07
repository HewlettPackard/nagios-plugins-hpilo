actionurldir=$(prefix)/libexec/hpeilo
actionurl_DATA=Base.css Fan_Info.js nagios_hpeilo_service_details.php  PS_Info.js Temp_Info.js CPU_Info.js Memory_Info.js Network_Info.js  Storage_Info.js

EXTRA_DIST = $(actionurl_DATA)
install-data-hook:
	cp $(prefix)/libexec/nagios_hpeilo_engine  $(actionurldir)
