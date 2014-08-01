/*	From $NetBSD: mount_msdos.c,v 1.18 1997/09/16 12:24:18 lukem Exp $	*/

/*
 * Copyright (c) 2002 Yar Tikhiy
 * Copyright (c) 1994 Christopher G. Demetriou
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Christopher G. Demetriou.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef lint
static const char rcsid[] =
  "$FreeBSD$";
#endif /* not lint */

#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>

#include "hfs/hfs_mount.h"

#include <ctype.h>
#include <err.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "mntopts.h"

static struct mntopt mopts[] = {
	MOPT_STDOPTS,
	MOPT_FORCE,
	MOPT_UPDATE,
	{ NULL }
};

static gid_t	a_gid __P((char *));
static uid_t	a_uid __P((char *));
static mode_t	a_mask __P((char *));
static void	usage __P((void)) __dead2; /* no return from usage() */

int
main(argc, argv)
	int argc;
	char **argv;
{
	int c, error, mntflags, noperm, set_gid, set_uid, set_mask;
	char *dev, *dir;
	char mntpath[MAXPATHLEN];
	struct hfs_mount_args args;
	struct stat sb;

	mntflags = noperm = set_gid = set_uid = set_mask = 0;
	bzero(&args, sizeof(args));

	while ((c = getopt(argc, argv, "Pu:g:m:o:")) != -1) {
		switch (c) {
		case 'P':
			noperm = 1;
			break;
		case 'u':
			args.hfs_uid = a_uid(optarg);
			noperm = set_uid = 1;
			break;
		case 'g':
			args.hfs_gid = a_gid(optarg);
			noperm = set_gid = 1;
			break;
		case 'm':
			args.hfs_mask = a_mask(optarg);
			noperm = set_mask = 1;
			break;
		case 'o':
			getmntopts(optarg, mopts, &mntflags, &args.flags);
			break;
		case '?':
		default:
			usage();
			break;
		}
	}

	if (optind + 2 != argc)
		usage();

	dev = argv[optind];
	dir = argv[optind + 1];

	/*
	 * Resolve the mountpoint with realpath(3) and remove unnecessary
	 * slashes from the devicename if there are any.
	 */
	checkpath(dir, mntpath);
	rmslashes(dev, dev);

	args.fspec = dev;
	args.export.ex_root = -2;	/* default uid to map root to */
	if (mntflags & MNT_RDONLY)
		args.export.ex_flags = MNT_EXRDONLY;
	else
		args.export.ex_flags = 0;

	if (noperm) {
		mntflags |= MNT_ACLS;	/* YYY hack! should use HFSMNT_FOO */
		if (!set_gid || !set_uid || !set_mask) {
			if (stat(mntpath, &sb) == -1)
				err(EX_OSERR, "stat %s", mntpath);

			if (!set_uid)
				args.hfs_uid = sb.st_uid;
			if (!set_gid)
				args.hfs_gid = sb.st_gid;
			if (!set_mask)
				args.hfs_mask = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
		}
	}

	if (mount("hfs", mntpath, mntflags, &args) < 0)
		err(EX_OSERR, "%s", dev);

	exit (0);
}

gid_t
a_gid(s)
	char *s;
{
	struct group *gr;
	char *ep;
	gid_t gid;

	if ((gr = getgrnam(s)) != NULL)
		gid = gr->gr_gid;
	else {
		gid = strtoul(s, &ep, 0);
		if (*ep)
			errx(EX_NOUSER, "unknown group id: %s", s);
	}
	return (gid);
}

uid_t
a_uid(s)
	char *s;
{
	struct passwd *pw;
	char *ep;
	uid_t uid;

	if ((pw = getpwnam(s)) != NULL)
		uid = pw->pw_uid;
	else {
		uid = strtoul(s, &ep, 0);
		if (*ep)
			errx(EX_NOUSER, "unknown user id: %s", s);
	}
	return (uid);
}

mode_t
a_mask(s)
	char *s;
{
	mode_t rv;
	char *ep;

	rv = strtoul(s, &ep, 8);
	if (*ep)
		errx(EX_USAGE, "invalid file mode: %s", s);
	return (rv);
}

void
usage()
{
	fprintf(stderr, "%s\n", 
	"usage: mount_hfs [-P] [-o options] [-u user] [-g group] [-m mask] bdev dir");
	exit(EX_USAGE);
}
