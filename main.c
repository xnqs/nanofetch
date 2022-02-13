#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// colors
#define COL_CYAN    "\x1b[36m"
#define COL_MAGENTA "\x1b[35m"
#define COL_BLUE    "\x1b[94m"
#define COL_RED     "\x1b[31m"
#define COL_GREEN   "\x1b[32m"
#define COL_YELLOW  "\x1b[33m"
#define COL_RESET   "\x1b[0m"
#define COL_TEXT    COL_MAGENTA // used for text
#define COL_LOGO    COL_BLUE // used for logo
#define COL_BORDER  COL_BLUE // used for border

// functions to pipe command output into string so we can print it out
void parse(char *dest, char *cmd);
void trim_left(char *dest, int amt);
void get_packages(char *packages, char *cmd);

int main(void) {
    // declared strings here for us to copy into
    static char os_release[100] = "Arch Linux"; // put your distribution here, it saves time :D
    static char kernel[100];
    static char packages[100];
    static char memory_used[100], memory_total[100];
    static char uptime[100];

    // get hostname, os, kernel, etc.
    parse(kernel,"uname -r");
    parse(packages, "pacman -Q | wc -l");
    parse(memory_used,"free -mh --si | awk  {'print $3'} | head -n 2 | tail -1");
    parse(memory_total,"free -mh --si | awk  {'print $2'} | head -n 2 | tail -1");
    parse(uptime,"uptime -p"); trim_left(uptime,3);
    
    // print it all out
    printf("\n");
    printf(COL_LOGO"   #####  #  #####    "COL_BORDER"│"COL_TEXT" OS: "COL_RESET"%s\n",os_release);
    printf(COL_LOGO"  #####  ###  #####   "COL_BORDER"│"COL_TEXT" Kernel: "COL_RESET"%s\n",kernel); 
    printf(COL_LOGO" #####  #####  #####  "COL_BORDER"│"COL_TEXT" Packages: "COL_RESET"%s\n",packages);
    printf(COL_LOGO"  #####  ###  #####   "COL_BORDER"│"COL_TEXT" Memory: "COL_RESET"%s/%s\n",memory_used,memory_total);
    printf(COL_LOGO"   #####  #  #####    "COL_BORDER"│"COL_TEXT" Uptime: "COL_RESET"%s\n",uptime);
    printf("\n");
}

// function definitions
void parse(char *dest, char *cmd) {
    char c = 0;
    FILE* tmp;

    // popen exception
    if (0 == (tmp = (FILE*)popen(cmd,"r"))) {
        fprintf(stderr,"ERROR: popen(\"%s\",\"r\") failed.\n",cmd);
        return;
    }

    // pipe everything into the string
    int i;
    for (i = 0; fread(&dest[i], sizeof(char), 1, tmp); i++) {
        if (dest[i]=='\n') {
            dest[i] = '\0';
            return;
        }
    }
    dest[i] = '\0';
}

void trim_left(char *dest, int amt) {
    char *ptr = dest;
    ptr += amt;
    strcpy(dest,ptr);
}
