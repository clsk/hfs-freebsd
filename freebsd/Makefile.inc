DEBUG_FLAGS?=	-g

BINDIR?=	${PREFIX}/sbin
NOSHARED?=	YES		# boot-time utilities should be static

.if defined(PREFIX)
MANDIR?=	${PREFIX}/man/man
SHAREDIR?=	${PREFIX}/share
.endif
