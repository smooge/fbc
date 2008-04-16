#!/bin/sh


usage()
{
	cat <<EOF
	
Usage: ./install-standalone.sh -i|-u [dest]

-i	Install standalone FreeBASIC in dest/freebasic
-u	Uninstall standalone FreeBASIC from dest/freebasic
dest	Destination directory (defaults to /usr/share)

EOF
	exit 1
}


i_success_msg()
{
	echo
	echo "================================================================================"
	echo "***** FreeBASIC compiler successfully installed in $dest *****"
	echo $linkmsg
	echo "================================================================================"
	echo
}
i_create_link()
{
	if [ -w $link ]; then
		rm -f "$link"/fbc
		echo "#!/bin/sh">"$link"/fbc
		echo "$dest/fbc -prefix $dest \$*">>"$link"/fbc
		chmod a+x "$link"/fbc
		linkmsg="A link to the compiler binary has also been created as $link/fbc"
	else
		linkmsg="It was not possible to install a link to the compiler binary as $link/fbc"
	fi
}
install()
{
	dest="$INSTALLDIR/freebasic"
	link="/usr/bin"
	
	mkdir -p -m 0755 "$dest"/bin/linux && \
	mkdir -p -m 0755 "$dest"/inc && \
	mkdir -p -m 0755 "$dest"/lib/linux && \
	cp -up lib/linux/fbrt0.o "$dest"/lib/linux/ && \
	cp -up lib/linux/libfb.a "$dest"/lib/linux/ && \
	cp -up lib/linux/libfbmt.a "$dest"/lib/linux/ && \
	cp -up lib/linux/libfbgfx.a "$dest"/lib/linux/ && \
	cp -up lib/linux/libtinyptc.a "$dest"/lib/linux/ && \
	cp -up lib/linux/elf_i386.x "$dest"/lib/linux/ && \
	cp -up lib/linux/crt1.o "$dest"/lib/linux/ && \
	cp -up lib/linux/crti.o "$dest"/lib/linux/ && \
	cp -up lib/linux/libgcc.a "$dest"/lib/linux/ && \
	cp -up lib/linux/libsupc++.a "$dest"/lib/linux/ && \
	cp -up lib/linux/crtbegin.o "$dest"/lib/linux/ && \
	cp -up lib/linux/crtn.o "$dest"/lib/linux/ && \
	cp -up lib/linux/gcrt1.o "$dest"/lib/linux/ && \
	cp -up lib/linux/libgcc_eh.a "$dest"/lib/linux/ && \
	cp -up lib/linux/crtend.o "$dest"/lib/linux/ && \
	cp -up lib/linux/libbfd.a "$dest"/lib/linux/ && \
	cp -up lib/linux/libiberty.a "$dest"/lib/linux/ && \
	cp -rup inc/* "$dest"/inc/  && \
	cp -up bin/linux/ar "$dest"/bin/linux && \
	cp -up bin/linux/as "$dest"/bin/linux && \
	cp -up bin/linux/gprof "$dest"/bin/linux && \
	cp -up bin/linux/ld "$dest"/bin/linux && \
	cp fbc "$dest" && \
	gzip -c docs/fbc.1 > /usr/share/man/man1/fbc.1.gz && \
	chmod a+rX "$dest"/lib/ && \
	chmod a+rX "$dest"/inc/ && \
	chmod a+rX "$dest"/bin/ && \
	i_create_link && \
	i_success_msg
}


uninstall()
{
	dest="$INSTALLDIR/freebasic"
	link="/usr/bin"
	
	rm -fr $dest
	if [ -w $link ]; then
		rm -f "$link"/fbc
	fi
	if [ -w /usr/share/man/man1/fbc.1.gz ]; then
		rm -f /usr/share/man/man1/fbc.1.gz
	fi

	echo
	echo "================================================================================"
	echo "FreeBASIC compiler successfully uninstalled from $dest"
	echo "================================================================================"
	echo
}


case "$1" in
	"-i")	ACTION="install";;
	"-u")	ACTION="uninstall";;
	*)		usage;;
esac

if [ ! -e ./fbc ]; then
	echo
	echo "ERROR: Unable to find ./fbc"
	echo
	exit 1
fi

if [ "$ACTION" = "install" ]; then
	PREFIXDIR=`./fbc -version | grep prefix | cut -c24- -`
	if [ "$PREFIXDIR" != "" ]; then
		echo
		echo "ERROR: ./fbc is configured for $PREFIXDIR.  Use install.sh to install."
		echo
		exit 1
	fi
fi

if [ "$2" != "" ]; then
	INSTALLDIR="$2"
else
	INSTALLDIR="/usr/share"
fi

if [ ! -d $INSTALLDIR ]; then
	echo
	echo "$INSTALLDIR is not a valid directory"
	echo
	exit 1
fi

if [ ! -w $INSTALLDIR ]; then
	echo
	echo "You need root priviledges to access $INSTALLDIR"
	echo
	exit 1
fi

case $ACTION in
	"install")	install;;
	"uninstall")uninstall;;
esac
