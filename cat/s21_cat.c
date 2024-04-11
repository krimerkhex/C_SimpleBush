#include <stdio.h>
#include <ctype.h>
#include <getopt.h>

struct flags {
    int bFlag;
    int nFlag;
    int vFlag;
    int tFlag;
    int eFlag;
    int sFlag;
    int voidStrFlag;
};

void struct_init(struct flags * f) {
    f->bFlag = 0,
    f->nFlag = 0,
    f->vFlag = 0,
    f->tFlag = 0,
    f->eFlag = 0,
    f->sFlag = 0,
    f->voidStrFlag = 0;
}

void s21_cat(FILE *file, struct flags * f);
int funcFindFlags(char ch, struct flags * f);

int main(int argc, char *argv[]) {
    int flagError = 0;
    struct flags f;
    struct_init(&f);
    static struct option long_options[] = {
        {"number-nonblank", 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {0, 0, 0, 0}
    };
    while (!flagError) {
        int ch = getopt_long(argc, argv, "beEnstTv", long_options, NULL);
        if (ch == -1) {
            break;
        } else {
            flagError = funcFindFlags(ch, &f);
        }
    }
    if (!flagError) {
        argc -= optind;
        argv += optind;
        for (; *argv != NULL; argv++) {
            FILE *fp;
            if ((fp = fopen(*argv, "r+")) != NULL) {
                s21_cat(fp, &f);
                fclose(fp);
            } else {
                printf("s21_cat: %s No such file or directory ", *argv);
            }
        }
    } else {
        printf("usage: %s [-benstv --number-nonblank --number --squeeze-blank] [file ...]\n", argv[0]);
    }
    return 0;
}

int funcFindFlags(char ch, struct flags * f) {
    int flag = 0;
    switch (ch) {
        case 'b':
            f->bFlag = 1;
            f->nFlag = 1;
            break;
        case 'e':
            f->eFlag = 1;
            f->vFlag = 1;
            break;
        case 'E':
            f->eFlag = 1;
            break;
        case 'n':
            f->nFlag = 1;
            break;
        case 's':
            f->sFlag = 1;
            break;
        case 't':
            f->tFlag = 1;
            f->vFlag = 1;
            break;
        case 'T':
            f->tFlag = 1;
            break;
        case 'v':
            f->vFlag = 1;
            break;
        case '?':
        default:
            flag = 1;
    }
    return flag;
}

void s21_cat(FILE *file, struct flags * f) {
    int ch, prev;
    long unsigned line = 0;
    for (prev = '\n'; (ch = getc(file)) != EOF; prev = ch) {
        if (prev == '\n') {
            if (f->sFlag) {
                if (ch == '\n') {
                    if (f->voidStrFlag)
                        continue;
                    f->voidStrFlag = 1;
                } else {
                    f->voidStrFlag = 0;
                }
            }
            if (f->nFlag) {
                if (!(f->bFlag) || ch != '\n') {
                    printf("%6lu\t", ++line);
                }
            }
        }
        if (ch == '\n') {
            if (f->eFlag)
                printf("%c", '$');
        } else if (ch == '\t') {
            if (f->tFlag) {
                printf("^I");
                continue;
            }
        } else if (f->vFlag) {
            if (!isascii(ch)) {
                printf("M-");
                ch = toascii(ch);
            }
            if (iscntrl(ch)) {
                printf("^%c", ch == '\177' ? '?' : ch | 0100);
                continue;
            }
        }
        printf("%c", ch);
    }
}
