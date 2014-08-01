DARWINDIR=	${.CURDIR}/../../../darwin

.PATH: ${DARWINDIR}/diskdev_cmds/fsck_hfs

PROG=	fsck_hfs

SRCS=	fsck_hfs.c strings.c utilities.c cache.c

CFLAGS+= -I${DARWINDIR}/xnu/bsd

LDADD=	-L${.OBJDIR}/../dfalib -ldfalib
DPADD=	${.OBJDIR}/../dfalib/libdfalib.a

NOMAN=	true	# XXX write man page

.include <bsd.prog.mk>
