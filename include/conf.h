/* conf.h */
/* (GENERATED FILE; DO NOT EDIT) */

#define	NULLPTR	(char *)0

/* Device table declarations */
struct	devsw	{			/* device table entry		*/
	int	dvnum;
	char	dvnam[10];
	int	dvrefct;
	int	(*dvinit)();
	int	(*dvopen)();
	int	(*dvclose)();
	int	(*dvread)();
	int	(*dvwrite)();
	int	(*dvseek)();
	int	(*dvgetc)();
	int	(*dvputc)();
	int	(*dvcntl)();
	int	dvport;
	int	dvivec;
	int	dvovec;
	int	(*dviint)();
	int	(*dvoint)();
	char	*dvioblk;
	int	dvminor;
	};

extern	struct	devsw devtab[];		/* one entry per device		*/

/* Device name definitions */

#define	CONSOLE	0		/* class tty	*/
#define	STDIO	1		/* class xio	*/
#define	STDIN	2		/* class sio	*/
#define	STDOUT	3		/* class sio	*/
#define	STDERR	4		/* class sio	*/
#define	NULLDEV	5		/* class null	*/
#define	DOS	20		/* class dos	*/
#define	NAMESPACE	25		/* class nam	*/
#define	PIPE	26		/* class pipm	*/

/* Control block sizes */

#define	Ntty	8
#define	Nxio	1
#define	Nsio	10
#define	Nnull	1
#define	Ndos	1
#define	Nmf	4
#define	Nnam	1
#define	Npipm	1
#define	Npip	4

#define	NDEVS	31

/* Declarations of I/O routines referenced */

extern	int	ioerr();
extern	int	ttyinit();
extern	int	ttyopen();
extern	int	ionull();
extern	int	ttyread();
extern	int	ttywrite();
extern	int	ttygetc();
extern	int	ttyputc();
extern	int	ttycntl();
extern	int	ttyiin();
extern	int	lwinit();
extern	int	lwopen();
extern	int	lwclose();
extern	int	lwread();
extern	int	lwwrite();
extern	int	lwgetc();
extern	int	lwputc();
extern	int	lwcntl();
extern	int	xioopen();
extern	int	sioinit();
extern	int	sioopen();
extern	int	sioclose();
extern	int	sioread();
extern	int	siowrite();
extern	int	sioseek();
extern	int	siogetc();
extern	int	sioputc();
extern	int	siocntl();
extern	int	nullopen();
extern	int	nullread();
extern	int	nullwrit();
extern	int	nullgetc();
extern	int	dsinit();
extern	int	dsopen();
extern	int	dsread();
extern	int	dswrite();
extern	int	dsseek();
extern	int	dscntl();
extern	int	lfinit();
extern	int	lfopen();
extern	int	lfclose();
extern	int	lfread();
extern	int	lfwrite();
extern	int	lfseek();
extern	int	lfgetc();
extern	int	lfputc();
extern	int	msopen();
extern	int	mscntl();
extern	int	mfinit();
extern	int	mfclose();
extern	int	mfread();
extern	int	mfwrite();
extern	int	mfseek();
extern	int	mfgetc();
extern	int	mfputc();
extern	int	naminit();
extern	int	namopen();
extern	int	namcntl();
extern	int	pipopen();
extern	int	pipinit();
extern	int	pipclose();
extern	int	pipread();
extern	int	pipwrite();
extern	int	pipgetc();
extern	int	pipputc();


/* Configuration and size constants */

#define MEMMARK			/* enable memory marking	*/
#define WINDOWS			/* enable windowing subsystem	*/
#define NPROC	64		/* number of user processes	*/
#define NSEM	128		/* total number of semaphores	*/
#define	RTCLOCK			/* system has a real-time clock	*/
#define NPORTS  16
#define	TIMERGRAN	1	/* timer granularity, secs/10	*/
#define VERSION "L3X 1.1"	/* label printed at startup	*/


