DARWINDIR=	${.CURDIR}/../../darwin
BSDDIR=		${DARWINDIR}/xnu/bsd

.PATH:	${BSDDIR}/hfs \
	${BSDDIR}/hfs/hfscommon/BTree \
	${BSDDIR}/hfs/hfscommon/Catalog \
	${BSDDIR}/hfs/hfscommon/Misc \
	${BSDDIR}/hfs/hfscommon/Unicode \
	${BSDDIR}/vfs

KMOD=	hfs

# Unused:
#	hfs_attrlist.c
#	hfs_lockf.c
#	hfs_quota.c
#	hfs_search.c
#	BTreeScanner.c

SRCS=	vnode_if.h \
	BTree.c \
	BTreeAllocate.c \
	BTreeMiscOps.c \
	BTreeNodeOps.c \
	BTreeTreeOps.c \
	BTreeWrapper.c \
	Catalog.c \
	CatalogIterators.c \
	CatalogUtilities.c \
	FileExtentMapping.c \
	FileIDsServices.c \
	MacOSStubs.c \
	UnicodeWrappers.c \
	VolumeAllocation.c \
	hfs_btreeio.c \
	hfs_catalog.c \
	hfs_chash.c \
	hfs_cnode.c \
	hfs_encodinghint.c \
	hfs_encodings.c \
	hfs_endian.c \
	hfs_link.c \
	hfs_lookup.c \
	hfs_readwrite.c \
	hfs_vfsops.c \
	hfs_vfsutils.c \
	hfs_vnops.c \
	rangelist.c \
	vfs_utfconv.c

CFLAGS+= -I${BSDDIR}
CFLAGS+= -I${BSDDIR}/hfs
CFLAGS+= -I${BSDDIR}/hfs/hfscommon/headers

# Remove -ansi since Darwin code uses C++ comments.
CWARNFLAGS= -Wall

.include <bsd.kmod.mk>
