PROG=	mount_hfs

SRCS=	mount_hfs.c getmntopts.c

MOUNT=	/usr/src/sbin/mount
CFLAGS+= -I${MOUNT}
.PATH:	${MOUNT}

CFLAGS+= -I${.CURDIR}/../../darwin/xnu/bsd

MAN=	mount_hfs.8

.include <bsd.prog.mk>
