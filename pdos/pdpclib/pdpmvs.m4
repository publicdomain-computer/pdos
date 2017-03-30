//PDPMVS   JOB CLASS=C,REGION=0K
//*
//PDPASM   PROC MEMBER='',LIST='NOLIST'
//ASM      EXEC PGM=ASMA90,
//   PARM='DECK,&LIST,TERM'
//SYSLIB   DD DSN=SYS1.MACLIB,DISP=SHR,DCB=BLKSIZE=32720
//         DD DSN=&&MACLIB,DISP=(OLD,PASS)
//         DD DSN=SYS1.MODGEN,DISP=SHR
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(20,10))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(20,10))
//SYSUT3   DD UNIT=SYSALLDA,SPACE=(CYL,(20,10))
//SYSTERM  DD SYSOUT=*
//SYSPRINT DD SYSOUT=*
//SYSLIN   DD DUMMY
//SYSGO    DD DUMMY
//SYSPUNCH DD DSN=&&OBJSET,UNIT=SYSALLDA,SPACE=(80,(9000,9000)),
//            DISP=(,PASS)
//*
//LKED     EXEC PGM=IEWL,PARM='NCAL',
//            COND=(0,LT,ASM)
//SYSLIN   DD DSN=&&OBJSET,DISP=(OLD,DELETE)
//SYSLMOD  DD DSN=&&NCALIB(&MEMBER),DISP=(OLD,PASS)
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(2,1))
//SYSPRINT DD SYSOUT=*
//         PEND
//CREATE   EXEC PGM=IEFBR14
//DD12     DD DSN=&&NCALIB,DISP=(,PASS),
// DCB=(RECFM=U,LRECL=0,BLKSIZE=6144),
// SPACE=(CYL,(10,10,20)),UNIT=SYSALLDA
//DD13     DD DSN=&&LOADLIB,DISP=(,PASS),
// DCB=(RECFM=U,LRECL=0,BLKSIZE=6144),
// SPACE=(CYL,(10,10,20)),UNIT=SYSALLDA
//DD14     DD DSN=&&MACLIB,DISP=(,PASS),
// DCB=(RECFM=FB,LRECL=80,BLKSIZE=6160),
// SPACE=(CYL,(10,10,20)),UNIT=SYSALLDA
//*
//PDPTOP   EXEC PGM=IEBGENER
//SYSUT2   DD  DSN=&&MACLIB(PDPTOP),DISP=(OLD,PASS)
//SYSUT1   DD  *
undivert(pdptop.mac)dnl
/*
//SYSPRINT DD  SYSOUT=*
//SYSIN    DD  DUMMY
//*
//PDPMAIN  EXEC PGM=IEBGENER
//SYSUT2   DD  DSN=&&MACLIB(PDPMAIN),DISP=(OLD,PASS)
//SYSUT1   DD  *
undivert(pdpmain.mac)dnl
/*
//SYSPRINT DD  SYSOUT=*
//SYSIN    DD  DUMMY
//*
//PDPPRLG  EXEC PGM=IEBGENER
//SYSUT2   DD  DSN=&&MACLIB(PDPPRLG),DISP=(OLD,PASS)
//SYSUT1   DD  *
undivert(pdpprlg.mac)dnl
/*
//SYSPRINT DD  SYSOUT=*
//SYSIN    DD  DUMMY
//*
//PDPEPIL  EXEC PGM=IEBGENER
//SYSUT2   DD  DSN=&&MACLIB(PDPEPIL),DISP=(OLD,PASS)
//SYSUT1   DD  *
undivert(pdpepil.mac)dnl
/*
//SYSPRINT DD  SYSOUT=*
//SYSIN    DD  DUMMY
//*
//MVSSTART EXEC PDPASM,MEMBER=MVSSTART
//SYSIN  DD  *
undivert(mvsstart.asm)dnl
/*
//MVSSUPA  EXEC PDPASM,MEMBER=MVSSUPA,LIST='LIST'
//SYSIN  DD  *
undivert(mvssupa.asm)dnl
/*
//START    EXEC PDPASM,MEMBER=START
//SYSIN  DD *
undivert(start.s)dnl
/*
//STDIO    EXEC PDPASM,MEMBER=STDIO
//SYSIN  DD *
undivert(stdio.s)dnl
/*
//STDLIB   EXEC PDPASM,MEMBER=STDLIB
//SYSIN  DD  *
undivert(stdlib.s)dnl
/*
//CTYPE    EXEC PDPASM,MEMBER=CTYPE
//SYSIN  DD  *
undivert(ctype.s)dnl
/*
//STRING   EXEC PDPASM,MEMBER=STRING
//SYSIN  DD  *
undivert(string.s)dnl
/*
//TIME     EXEC PDPASM,MEMBER=TIME
//SYSIN  DD  *
undivert(time.s)dnl
/*
//ERRNO    EXEC PDPASM,MEMBER=ERRNO
//SYSIN  DD  *
undivert(errno.s)dnl
/*
//ASSERT   EXEC PDPASM,MEMBER=ASSERT
//SYSIN  DD  *
undivert(assert.s)dnl
/*
//LOCALE   EXEC PDPASM,MEMBER=LOCALE
//SYSIN  DD  *
undivert(locale.s)dnl
/*
//MATH     EXEC PDPASM,MEMBER=MATH
//SYSIN  DD  *
undivert(math.s)dnl
/*
//SETJMP   EXEC PDPASM,MEMBER=SETJMP
//SYSIN  DD  *
undivert(setjmp.s)dnl
/*
//SIGNAL   EXEC PDPASM,MEMBER=SIGNAL
//SYSIN  DD  *
undivert(signal.s)dnl
/*
//@@MEMMGR EXEC PDPASM,MEMBER=@@MEMMGR
//SYSIN  DD  *
undivert(__memmgr.s)dnl
/*
//PDPTEST EXEC PDPASM,MEMBER=PDPTEST
//SYSIN  DD  *
undivert(pdptest.s)dnl
/*
//LKED     EXEC PGM=IEWL,PARM='MAP,LIST,AMODE=31,RMODE=ANY'
//SYSLIN   DD DDNAME=SYSIN
//SYSLIB   DD DSN=&&NCALIB,DISP=(OLD,PASS)
//SYSLMOD  DD DSN=&&LOADLIB(PDPTEST),DISP=(OLD,PASS)
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(2,1))
//SYSPRINT DD SYSOUT=*
//SYSUDUMP DD SYSOUT=*
//SYSIN DD *
 INCLUDE SYSLIB(MVSSTART)
 INCLUDE SYSLIB(START)
 INCLUDE SYSLIB(MVSSUPA)
 INCLUDE SYSLIB(STDIO)
 INCLUDE SYSLIB(STDLIB)
 INCLUDE SYSLIB(CTYPE)
 INCLUDE SYSLIB(STRING)
 INCLUDE SYSLIB(TIME)
 INCLUDE SYSLIB(ERRNO)
 INCLUDE SYSLIB(ASSERT)
 INCLUDE SYSLIB(LOCALE)
 INCLUDE SYSLIB(MATH)
 INCLUDE SYSLIB(SETJMP)
 INCLUDE SYSLIB(SIGNAL)
 INCLUDE SYSLIB(@@MEMMGR)
 INCLUDE SYSLIB(PDPTEST)
 ENTRY @@MAIN
/*
//*
//PDPTEST  EXEC PGM=PDPTEST,PARM='-tt dd:in dd:out',
//         COND=(0,LT,LKED)
//STEPLIB  DD  DSN=&&LOADLIB,DISP=(OLD,PASS)
//SYSPRINT DD  SYSOUT=*
//SYSTERM  DD  SYSOUT=*
//SYSUDUMP DD  SYSOUT=*
//SYSIN    DD  DUMMY
//IN       DD  *
line 1
line two
/*
//OUT      DD  SYSOUT=*
//*
//