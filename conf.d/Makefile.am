NAGIOS3CONFDIR = /etc/nagios3/conf.d
confddir=$(prefix)/conf.d
confd_DATA=generic-contact_nagios2.cfg
EXTRA_DIST = $(confd_DATA)
install-data-hook:
	if [  -d $(NAGIOS3CONFDIR) ] ; then \
		 cp  $(confddir)/$(confd_DATA) $(NAGIOS3CONFDIR) \
        ;  fi
