/* find all src files and dependencies */

#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <string.h>
#include <ctype.h>

#define LINLEN 1280
#define LINTAIL LINLEN-256
#define MAXCOL 79
#define OSNAME "L3X"
#define HNAMELEN 20
#define HPATHLEN 40
#define MAXINCL 20
#define MAXDEPT 20


int na;
char * basedir;
int fprn_cnt;		// counts include nesting depth
int recur;

char srcfiles[200][13];

char line[LINLEN];

int sort_fun( const void *a, const void *b)
{
   return( strcmp((char *) a, (char *) b) );
}


int cp(const char *, FILE *);


void findsrc(void)
{
	int done;
	int nc;
	struct ffblk ffblk;
	char * s;
	
	done = findfirst("*.c",&ffblk,0);		// find c files
	nc = 0;
	while (!done) {
		s = strlwr(ffblk.ff_name);
		strcpy(srcfiles[nc++], s);
		done = findnext(&ffblk);
	}
	done = findfirst("*.asm",&ffblk,0);		// find asm files
	na = nc;
	while (!done) {
		s = strlwr(ffblk.ff_name);
		strcpy(srcfiles[na++], s);
		done = findnext(&ffblk);
	}

	qsort(srcfiles, na, 13, sort_fun);
}


int cfindincl(void)
{
	char * t, *ipn;
	
	if (line[0] == '#' && line[1] == 'i' &&
	    line[2] == 'n' && line[3] == 'c') {
		t = strchr(line, '<');
		*strchr(t+1, '>') = '\0';
		t[0] = '\\'; t[-1] = ')'; t[-2] = 'H'; 
		t[-3] = '('; t[-4] = '$';
		ipn = strcpy(line+LINTAIL, basedir);
		strcat(ipn,"/src/h/");
		strcat(ipn,t+1);

		return(t-line-4);
	}
	return(-1);
}


int afindincl(void)
{
	char c, *s, * t;
	char inclstr[] = "include";
	
	if ((s = strstr(strlwr(line),inclstr)) == NULL)
		return(-1);
	s+=7;
	while ((c = *s) == ' ' || c == '\t') 
		s++;
	if (c == '\n')
		return(-1);
	for (t = s; (c = *t) != ' ' && c != '\t' && c!='\n'; t++)
		;
	*t = '\0';
	strcpy(line+LINTAIL, s);
	return(s-line);
}


int fprn_h(FILE * mkfp, char * src, int findincl(void))
{
	char hfiles[MAXINCL][HNAMELEN+HPATHLEN];
	int ifn;
	int nh;
	FILE * fp;
	int j;

	if (++fprn_cnt > MAXDEPT) {
		fprintf(stderr, "#include nesting too deep or auto-include\n");
		fprintf(stderr, "last include file: %s\n", src);
		return(-1);
	}

	if ((fp = fopen(src, "r")) == NULL) {
		fprintf(stderr, "Couldn't open include file %s\n", src);
		return(-1);
	}
	nh = 0;
	while (fgets(line,LINLEN,fp) != 0)
		if ((ifn = findincl()) != -1) {
			strcpy(hfiles[nh], line+ifn);
			if (recur)
				strcpy(hfiles[nh]+HNAMELEN, line+LINTAIL);
			nh++;
		}
	fclose(fp);

/* Note that findincl() returns the position of the substring of line[]
 * to be written as include file in MAKEFILE, e.g. $(H)\mem.h;
 * moreover, it stores the full include file name at line[LINTAIL]...	*/

	if (nh == 0)
		return(0);

	qsort(hfiles, nh, HNAMELEN+HPATHLEN, sort_fun);
	for (j = 0; j < nh; j++) {
		fprintf(mkfp, " %s", hfiles[j]);
		if (!recur)
			continue;
		if (fprn_h(mkfp,hfiles[j]+HNAMELEN,findincl) < 0)
			return(-1);
	}

	return(nh);
}


int makedeps(FILE * mkfp, char * src)
{
	char fn[13], *xc;
	int i, j;
	int ((*pfun) (void));

	strcpy(fn, src);
	*(xc = strchr(src, '.')) = '\0';	// removes ext from srcfiles[i]!
	switch (tolower(xc[1])) {
		case 'c': 
			pfun = &cfindincl;
			break;
		case 'a': 
			pfun = &afindincl;
			break;
	}

	fprintf(mkfp, "%s.obj:", src);

	fprn_cnt = 0;
	if (fprn_h(mkfp,fn,*pfun) < 0)
		return(-1);						// -1: error

	fprintf(mkfp, " %s\n", fn);

	return(0);
}


void prnobjs(FILE * mkfp)
{
	char fn[13];
	int i, npr;
	
	fprintf(mkfp, "\n");
	npr = fprintf(mkfp, "OBJS = ");
	for (i = 0; i < na; i++) {
		strcpy(fn, srcfiles[i]);
		*strchr(fn, '.') = '\0';
		if (npr + strlen(fn) + 6 > MAXCOL) {
			fprintf(mkfp, "\\\n");
			npr = 0;
		}
		npr += fprintf(mkfp, "%s.obj ", fn);
	}
	fprintf(mkfp, "\n\n");
}


int main(int argc, char * argv[])
{
	FILE * fp, *mkfp;
	int i;
	char fn[256];

	if (argc > 2) {
		fprintf(stderr, "Usage: makemake [-n] (-n = non-recursive)\n");
		return(-1);
	}
	
	recur = 1;
	if (argc == 2)
		if (argv[1][0] != '-' || argv[1][1] != 'n' ) {
			fprintf(stderr, "Usage: makemake [-n] (-n = non-recursive)\n");
			return(-1);
		} else
			recur = 0;

	fprintf(stderr, "Building dependencies %srecursively\n", 
			recur ? "" : "non ");

	if ((basedir = getenv(OSNAME)) == NULL) {
		fprintf(stderr, "environment var for base OS dir not defined\n");
		return(-1);
	}

	if ((mkfp = fopen("makefile", "w")) == NULL) {
		fprintf(stderr, "makefile can't be created\n");
		return(-1);
	}

	findsrc(); 

	strcpy(fn,basedir);
	strcat(fn,"/lib/make1.add");
	if (cp(fn, mkfp) != 0) {
		fprintf(stderr, "file %s can't be opened\n", fn);
		remove("makefile");
		return(-1);
	}

	strcpy(fn, "LIBFILE");
	if (cp(fn, mkfp) != 0) {
		fprintf(stderr, "file %s can't be opened\n", fn);
		remove("makefile");
		return(-1);
	}

	strcpy(fn,basedir);
	strcat(fn,"/lib/make2.add");
	if (cp(fn, mkfp) != 0) {
		fprintf(stderr, "file %s can't be opened\n", fn);
		remove("makefile");
		return(-1);
	}

	prnobjs(mkfp);

	strcpy(fn,basedir);
	strcat(fn,"/lib/make3.add");
	if (cp(fn, mkfp) != 0) {
		fprintf(stderr, "file %s can't be opened\n", fn);
		remove("makefile");
		return(-1);
	}

	for (i = 0; i < na; i++)
		if (makedeps(mkfp, srcfiles[i]) < 0)	// will also remove 
			return(-1);							//	extension from srcfiles[i]
	
	if ((fp = fopen("xmodules", "w")) == NULL) {
		fprintf(stderr, "xmodules can't be created\n");
		return(-2);
	}
	for (i = 0; i < na; i++)
		fprintf(fp, "%s\n", srcfiles[i]);
	fclose(fp);

	if ((fp = fopen("xbldlib", "w")) == NULL) {
		fprintf(stderr, "xbldlib can't be created\n");
		return(-3);
	}
	na--;
	for (i = 0; i < na; i++)
		fprintf(fp, "+%s &\n", srcfiles[i]);
	fprintf(fp, "+%s\n", srcfiles[na]);
	fclose(fp);

	return 0;
}


int cp(const char * fn1, FILE * fp) 
{
	FILE * fp1, * fp2;
	char line[LINLEN];

	if ((fp1 = fopen(fn1, "r")) == NULL)
		return(-1);
	while (fgets(line, LINLEN, fp1) != 0)
		fputs(line, fp);
	fclose(fp1);
	return(0);
}

