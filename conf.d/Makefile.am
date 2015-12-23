NAGIOS3CONFDIR = /etc/nagios3/conf.d
confddir=$(NAGIOS3CONFDIR)
#confddir=$(NAGIOS3DIR)/conf.d
confd_DATA= generic-contact_nagios2.cfg
#EXTRA_DIST = $(confd_DATA)
#if [ -d $(NAGIOS3CONFDIR) ] ; then $(INSTALL) -m 644 $(DEB_BUILD_DIR)/conf.d/generic-contact_nagios2.cfg $(NAGIOS3CONFDIR) ; fi
if [ -d $(NAGIOS3CONFDIR) ] ; EXTRA_DIST = $(confd_DATA)  ; fi

