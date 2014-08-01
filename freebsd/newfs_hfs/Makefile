DARWINDIR=	${.CURDIR}/../../darwin

.PATH:	${DARWINDIR}/diskdev_cmds/newfs_hfs

PROG=	newfs_hfs

SRCS=	hfs_endian.c makehfs.c newfs_hfs.c utf.c

CFLAGS+= -I${DARWINDIR}/xnu/bsd
CFLAGS+= -I${DARWINDIR}/diskdev_cmds/fsck_hfs/dfalib	# for SRuntime.h

BOOTDATA= ${SHAREDIR}/misc/hfsbootdata
CFLAGS+=  -DHFS_BOOT_DATA="\"${BOOTDATA}\""

MAN=	newfs_hfs.8

afterinstall:
	cd ${DARWINDIR}/diskdev_cmds/newfs_hfs && \
	    ${INSTALL} -o ${SHAREOWN} -g ${SHAREGRP} -m ${SHAREMODE} \
	        hfsbootdata.img \
	        ${DESTDIR}${BOOTDATA}

.include <bsd.prog.mk>
