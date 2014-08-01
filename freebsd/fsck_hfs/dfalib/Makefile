DARWINDIR=	${.CURDIR}/../../../darwin

.PATH: ${DARWINDIR}/diskdev_cmds/fsck_hfs/dfalib

LIB=	dfalib

SRCS=	BlockCache.c \
	BTree.c \
	BTreeAllocate.c \
	BTreeMiscOps.c \
	BTreeNodeOps.c \
	BTreeScanner.c \
	BTreeTreeOps.c \
	CatalogCheck.c \
	HardLinkCheck.c \
	SBTree.c \
	SControl.c \
	SVerify1.c \
	SVerify2.c \
	SRepair.c \
	SRebuildCatalogBTree.c \
	SUtils.c \
	SKeyCompare.c \
	SDevice.c \
	SExtents.c \
	SAllocate.c \
	SCatalog.c \
	SStubs.c \
	VolumeBitmapCheck.c
         
CFLAGS+= -DDEBUG_BUILD=0	# must be defined since used in C expressions
CFLAGS+= -I${DARWINDIR}/xnu/bsd

NOINSTALLLIB=	true
NOPIC=		true
NOPROFILE=	true

.include <bsd.lib.mk>
