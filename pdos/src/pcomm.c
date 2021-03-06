/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  pcomm - command processor for pdos                               */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "pos.h"
#include "dostime.h"

/* Written By NECDET COKYAZICI, Public Domain */
void putch(int a);
void bell(void);
void safegets(char *buffer, int size);



static char buf[200];
static unsigned char cmdt[140];
static char path[500] = ";" ; /* Used to store path */
static struct {
    int env;
    unsigned char *cmdtail;
    char *fcb1;
    char *fcb2;
} parmblock = { 0, cmdt, NULL, NULL };
static size_t len;
static char drive[2] = "A";
static char cwd[65];
static char prompt[50] = ">";
static int singleCommand = 0;
static int primary = 0;
static int term = 0;

static void parseArgs(int argc, char **argv);
static void processInput(void);
static void putPrompt(void);
static void doCopy(char *b);
static void doExec(char *b,char *p);
static void dotype(char *file);
static void dodir(char *pattern);
static void dover(void);
static void dodel(char *fnm);
static void dopath(char *s);
static void changedir(char *to);
static void changedisk(int drive);
static int ins_strcmp(char *one, char *two);
static int ins_strncmp(char *one, char *two, size_t len);
static void readBat(char *fnm);
static int exists(char *fnm);

int main(int argc, char **argv)
{
#ifdef USING_EXE
    pdosRun();
#endif
#ifdef __32BIT__
    install_runtime();
#endif
    parseArgs(argc, argv);
    if (singleCommand)
    {
        processInput();
        return (0);
    }
    if (primary)
    {
        printf("welcome to pcomm\n");
        readBat("AUTOEXEC.BAT");
    }
    else
    {
        printf("welcome to pcomm - exit to return\n");
    }
    while (!term)
    {
        putPrompt();
        safegets(buf, sizeof buf);

        processInput();
    }
    printf("thankyou for using pcomm!\n");
    return (0);
}

static void parseArgs(int argc, char **argv)
{
    int x;
    
    if (argc > 1)
    {
        if ((argv[1][0] == '-') || (argv[1][0] == '/'))
        {
            if ((argv[1][1] == 'C') || (argv[1][1] == 'c'))
            {
                singleCommand = 1;
            }            
            if ((argv[1][1] == 'P') || (argv[1][1] == 'p'))
            {
                primary = 1;
            }            
        }
    }
    if (singleCommand)
    {
        strcpy(buf, "");
        for (x = 2; x < argc; x++)
        {
            strcat(buf, *(argv + x));
            strcat(buf, " ");
        }
        len = strlen(buf);
        if (len > 0)
        {
            len--;
            buf[len] = '\0';
        }
    }
    return;
}

static void processInput(void)
{
    char *p;

    len = strlen(buf);
    if ((len > 0) && (buf[len - 1] == '\n'))
    {
        len--;
        buf[len] = '\0';
    }
    p = strchr(buf, ' ');
    if (p != NULL)
    {
        *p++ = '\0';
    }
    else
    {
        p = buf + len;
    }
    len -= (size_t)(p - buf);
    if (ins_strcmp(buf, "exit") == 0)
    {
#ifdef CONTINUOUS_LOOP
        primary = 0;
#endif
        if (!primary)
        {
            term = 1;
        }
    }
    else if (ins_strcmp(buf, "path") == 0)
    {
        dopath(p);
    }
    else if (ins_strcmp(buf, "type") == 0)
    {
        dotype(p);
    }
    else if (ins_strcmp(buf, "dir") == 0)
    {
        dodir(p);
    }
    else if (ins_strcmp(buf, "ver") == 0)
    {
        dover();
    }
    else if (ins_strcmp(buf, "echo") == 0)
    {
        printf("%s\n", p);
    }
    else if (ins_strcmp(buf, "cd") == 0)
    {
        changedir(p);
    }
    else if (ins_strncmp(buf, "cd.", 3) == 0)
    {
        changedir(buf + 2);
    }
    else if (ins_strncmp(buf, "cd\\", 3) == 0)
    {
        changedir(buf + 2);
    }
    else if (ins_strcmp(buf, "reboot") == 0)
    {
        PosReboot();
    }
    else if(ins_strcmp(buf, "del")==0)
    {
        dodel(p);
    }
    else if ((strlen(buf) == 2) && (buf[1] == ':'))
    {
        changedisk(buf[0]);
    }
    else if (ins_strcmp(buf, "copy") == 0)
    {
        doCopy(p);
    }
    else
    {
        doExec(buf,p);
    }
    return;
}

static void putPrompt(void)
{
    int d;
    
    d = PosGetDefaultDrive();
    drive[0] = d + 'A';
    PosGetCurDir(0, cwd);
    printf("\n%s:\\%s%s", drive, cwd, prompt);
    fflush(stdout);
    return;
}

static void dotype(char *file)
{
    FILE *fp;
    
    if (strcmp(file,"") == 0)
    {
        printf("Required Parameter Missing\n");
        return;
    }
        
    fp = fopen(file, "r");
    
    if (fp != NULL)
    {
        while (fgets(buf, sizeof buf, fp) != NULL)
        {
            fputs(buf, stdout);
        }
        fclose(fp);
    }
    
    else
    {
       printf("file not found: %s\n", file);
    }
    
    return;
}

static void dodel(char *fnm)
{    
    int ret;
    DTA *dta;

    dta = PosGetDTA();
    if(*fnm == '\0')
    {
        printf("Please Specify the file name \n");
        return;
    }
    ret = PosFindFirst(fnm,0x10);
    
    if(ret == 2)
    {
        printf("File not found \n");
        printf("No Files Deleted \n");
        return;
    }
    while(ret == 0)
    {           
        PosDeleteFile(dta->file_name);  
        ret = PosFindNext();
    }
    return;
}

static void dodir(char *pattern)
{
    DTA *dta;
    int ret;
    char *p;
    time_t tt;
    struct tm *tms;
    
    dta = PosGetDTA();
    if (*pattern == '\0')
    {
        p = "*.*";
    }
    else
    {
        p = pattern;
    }
    ret = PosFindFirst(p, 0x10);
    while (ret == 0)
    {
        tt = dos_to_timet(dta->file_date,dta->file_time);
        tms = localtime(&tt);
        printf("%-13s %9ld %02x %04d-%02d-%02d %02d:%02d:%02d\n", 
               dta->file_name,
               dta->file_size,
               dta->attrib,
               tms->tm_year + 1900,
               tms->tm_mon + 1,
               tms->tm_mday,
               tms->tm_hour,
               tms->tm_min,
               tms->tm_sec);
        ret = PosFindNext();
    }
    return;
}

static void dover(void)
{
    printf("%s %s\n", __DATE__, __TIME__);
    return;
}

static void dopath(char *s)
{    
     if (strcmp(s, "") == 0)
     {
        char *t;

        t = path;
        
        if (*t == ';')
        {
            t++;
        } 
        
        if (*t == '\0')
        {
            printf("No Path defined\n");
        }
        
        else
        {
            printf("%s\n", t);
        }
     }
     
     else if (*s == ';')
     {
        strcpy(path, s);
     }
     
     else
     {
        strcpy(path, ";");
        strcat(path, s);
     }
     return;
} 

static void doExec(char *b,char *p)
{
    char *r;
    char *s;
    char *t;
    size_t ln;
    size_t ln2;
    char tempbuf[FILENAME_MAX];
    
    s = path;    
    ln = strlen(p);
    cmdt[0] = ln;
    memcpy(cmdt + 1, p, ln);
    memcpy(cmdt + ln + 1, "\r", 2);
    
    while(*s != '\0')
    {    
        t = strchr(s, ';');
        
        if (t == NULL)
        {
            t = s + strlen(s);
        }

        ln2 = t - s;
        memcpy(tempbuf, s ,ln2);
        
        if (ln2 != 0)
        {
            tempbuf[ln2++] = '\\';
        }
        
        strcpy(tempbuf + ln2, b);
        ln2 += strlen(b);
        strcpy(tempbuf + ln2, ".com");

        if (exists(tempbuf))
        {
            PosExec(tempbuf, &parmblock);
            break;
        }
        
        strcpy(tempbuf + ln2 ,".exe");
        
        if (exists(tempbuf))
        {
            PosExec(tempbuf, &parmblock);
            break;
        }
        
        strcpy(tempbuf + ln2 ,".bat");
        
        if (exists(tempbuf))
        {
            readBat(tempbuf);
            break;
        }
        
        s = t;
        
        if (*s == ';')
        {
            s++;
        }
    }
    
    if (*s == '\0')
    {
        printf("command not found\n");
    }
    
    return;
}

static void doCopy(char *b)
{
    char bbb[512];
    char *src;
    char *dest;
    FILE *fsrc;
    FILE *fdest;
    int bytes_read = 0; 
    
    src = b;
    dest = strchr(b,' ');
     
    if (dest == NULL)
    {
        printf("\n Two Filenames required");
        return;
    }
    
    *dest++ ='\0';
        
    printf("Source is %s \n",src);
    printf("Dest is %s \n",dest);
       
    fsrc = fopen(src,"rb");
    
    if (fsrc != NULL)
    {
        fdest = fopen(dest,"wb");
        
        if (fdest != NULL)
        {
            while((bytes_read = fread(bbb,1,sizeof(bbb),fsrc)) != 0)
            {
                fwrite(bbb,1,bytes_read,fdest);
            }
            
            fclose(fdest);   
        }
        
        else
        {
            printf("Destination %s file not found \n",dest);
        }
        
        fclose(fsrc);
    }
    
    else
    {
        printf("Source file %s not found \n",src);
    }
      
    return;
    
}

static void changedir(char *to)
{
    PosChangeDir(to);
    return;
}

static void changedisk(int drive)
{
    PosSelectDisk(toupper(drive) - 'A');
    return;
}

static int ins_strcmp(char *one, char *two)
{
    while (toupper(*one) == toupper(*two))
    {
        if (*one == '\0')
        {
            return (0);
        }
        one++;
        two++;
    }
    if (toupper(*one) < toupper(*two))
    {
        return (-1);
    }
    return (1);
}

static int ins_strncmp(char *one, char *two, size_t len)
{
    size_t x = 0;
    
    if (len == 0) return (0);
    while ((x < len) && (toupper(*one) == toupper(*two)))
    {
        if (*one == '\0')
        {
            return (0);
        }
        one++;
        two++;
        x++;
    }
    if (x == len) return (0);
    return (toupper(*one) - toupper(*two));
}

static void readBat(char *fnm)
{
    FILE *fp;
    
    fp = fopen(fnm, "r");
    if (fp != NULL)
    {        
        while (fgets(buf, sizeof buf, fp) != NULL)
        {
            processInput();
        }
        fclose(fp);
    }
    return;
}

static int exists(char *fnm)
{
    FILE *fp;
    
    fp = fopen(fnm,"rb");
    
    if (fp != NULL)
    {
        fclose(fp);
        return 1;
    }
    
    return 0;
}


/* Written By NECDET COKYAZICI, Public Domain */


void putch(int a)
{
    putchar(a);
    fflush(stdout);
}


void bell(void)
{
    putch('\a');
}


void safegets(char *buffer, int size)
{
    int a;
    int shift;
    int i = 0;

    while (1)
    {

        a = PosGetCharInputNoEcho();

/*
        shift = BosGetShiftStatus();

        if (shift == 1)
        {
            if ((a != '\n') && (a != '\r') && (a != '\b'))
            if (islower(a))
            a -= 32;
     
            else if ((a >= '0') && (a <= '9'))
            a -= 16;
        }

*/

        if (i == size)
        {
            buffer[size] = '\0';
      
            if ((a == '\n') || (a == '\r'))
            return;

            bell();
        }

        if ((i == 0) && (a == '\b'))
        continue;

        if (i < size)
        {

            if ((a == '\n') || (a == '\r'))
            {
                putch('\n');

                buffer[i] = '\0';

                return;
            }


            if (a == '\b')
            {
        
                if (i == 0)
                continue;
        
                else
                i--;

                putch('\b');
                putch(' ');
                putch('\b');

                continue;
            }
            else
            putch(a);


            if (isprint((unsigned char)a))
            {
                buffer[i] = a;
                i++;
            }
            else bell();

        }
        else bell();

    }

}
