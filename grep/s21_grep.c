#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

struct flags {
    int eFlag;
    int iFlag;
    int vFlag;
    int cFlag;
    int lFlag;
    int nFlag;
    int hFlag;
    int sFlag;
    int fFlag;
    int oFlag;
    int flagVoidPattern;
    int indexStruct;
    char pattern[420][420];
};

regmatch_t pmatch;

void struct_init(struct flags * f) {
    f->eFlag = 0;
    f->iFlag = 0;
    f->vFlag = 0;
    f->cFlag = 0;
    f->lFlag = 0;
    f->nFlag = 0;
    f->hFlag = 0;
    f->sFlag = 0;
    f->fFlag = 0;
    f->oFlag = 0;
    f->flagVoidPattern = 0;
    f->indexStruct = 0;
    for (int i = 0; i < 420; i++) {
        for (int j = 0; j < 420; j++) {
            f->pattern[i][j] = 0;
        }
    }
}

void funcReadFileAddStruct(char *optarg, struct flags * f) {
    FILE *file;
    char *line = NULL;
    size_t size = 0;
    if ((file = fopen(optarg, "r")) != NULL) {
        while (getline(&line, &size, file) != -1) {
            strcpy(f->pattern[(f->indexStruct)++], line);
        }
        fclose(file);
        free(line);
    } else if (!(f->sFlag)) {
        printf("s21_grep: %s No such file or directory\n", optarg);
        exit(0);
    }
}

int funcFindFlags(char ch, struct flags * f) {
    int flag = 0;
    switch (ch) {
        case 'e':
            f->eFlag = 1;
            strcpy(f->pattern[(f->indexStruct)++], optarg);
            break;
        case 'i':
            f->iFlag = 1;
            break;
        case 'v':
            f->vFlag = 1;
            break;
        case 'c':
            f->cFlag = 1;
            break;
        case 'l':
            f->lFlag = 1;
            break;
        case 'n':
            f->nFlag = 1;
            break;
        case 'h':
            f->hFlag = 1;
            break;
        case 's':
            f->sFlag = 1;
            break;
        case 'f':
            f->fFlag = 1;
            funcReadFileAddStruct(optarg, f);
            break;
        case 'o':
            f->oFlag = 1;
            break;
        default:
            flag = 1;
    }
    return flag;
}

int findStr(char *str, struct flags * f) {
    regex_t regex;
    if (str[strlen(str)-1] != '\n') {
        strcat(str, "\n");
    }
    int reti = 0, compVal = 0, flag = 0;
    int icase = (f->iFlag) == 1 ? REG_ICASE : 0;
    for (int i = 0; i < (f->indexStruct); i++) {
        compVal = regcomp(&regex, f->pattern[i], icase);
        if (!compVal) {
            reti = regexec(&regex, str, 0, NULL, 0);
            if (!reti) {
                flag = 1;
                break;
            }
        }
    }
    regfree(&regex);
    return flag ^ f->vFlag;
}

void funcFlagO(char *str, struct flags * f) {
    regex_t regex;
    int reti = 0, compVal = 0;
    int icase = (f->iFlag) == 1 ? REG_ICASE : 0;
    for (int i = 0; i < (f->indexStruct); i++) {
        compVal = regcomp(&regex, f->pattern[i], icase);
        char *tempStr = str;
        if (!compVal) {
            while (tempStr) {
                reti = regexec(&regex, tempStr, 2, &pmatch, 0);
                if (!reti) {
                    if (pmatch.rm_so == pmatch.rm_eo) {
                        pmatch.rm_so = 0;
                    }
                    for (int i = pmatch.rm_so; i < pmatch.rm_eo; i++) {
                      printf("%c", tempStr[i]);
                    }
                    printf("\n");
                    if (strlen(tempStr) - pmatch.rm_eo < 1) {
                        break;
                    }
                    tempStr += pmatch.rm_eo;
                } else {
                    break;
                }
            }
        }
    }
    regfree(&regex);
}

void s21_grep(FILE *file, int argc, char *argv, struct flags * f) {
    char str[1024];
    int numStr = 1;
    int numFlagC = 0;
    while (fgets(str, 1023, file)) {
        if (findStr(str, f) == 1) {
            if ((f->lFlag) | (f->cFlag)) {
                numFlagC++;
                continue;
            } else if (argc > 1 && !(f->hFlag)) {
                printf("%s:", argv);
            }
            if ((f->nFlag)) {
                printf("%d:", numStr);
            }
            if ((f->oFlag) && !(f->vFlag) && !(f->flagVoidPattern)) {
                funcFlagO(str, f);
            } else {
                printf("%s", str);
            }
        }
        numStr++;
    }
    if ((f->cFlag)) {
        if (argc > 1 && !(f->hFlag)) {
           printf("%s:", argv);
        }
        numFlagC = (numFlagC > 0 && (f->lFlag)) ? 1 : numFlagC;
        printf("%d\n", numFlagC);
    }
    if ((f->lFlag) && numFlagC > 0) {
        printf("%s\n", argv);
    }
}

void findPatternInFile(int argc, char *argv[], struct flags * f) {
    if (!(f->indexStruct) && !(f->fFlag)) {
        strcpy(f->pattern[(f->indexStruct)++], *argv);
        argv++;
        argc--;
    }
    for (int i = 0; i < (f->indexStruct); i++) {
       if (f->pattern[i][0] == 0 || f->pattern[i][0] == 10) {
            (f->flagVoidPattern) = 1;
            (f->iFlag) = 1;
       }
    }
    FILE *fp;
    for (; *argv != NULL; argv++) {
        if ((fp = fopen(*argv, "r")) != NULL) {
            s21_grep(fp, argc, *argv, f);
            fclose(fp);
        } else if (!(f->sFlag)) {
            printf("s21_grep: %s No such file or directory\n", *argv);
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    int ch;
    int flagError = 0;
    struct flags f;
    struct_init(&f);
    while (1) {
        ch = getopt(argc, argv, "e:ivclnhsf:o");
        if (ch == -1)
            break;
        flagError = funcFindFlags(ch, &f);
    }
    for (int i = 0; i < (f.indexStruct); i++) {
    }
    if (!flagError) {
        argc -= optind;
        argv += optind;
        findPatternInFile(argc, argv, &f);
    } else {
        printf("flagError!");
    }
    return 0;
}
