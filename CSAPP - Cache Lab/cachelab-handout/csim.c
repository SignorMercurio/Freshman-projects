#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#define MAXLINE 200

typedef struct SetEntry
{
    int *val;
    clock_t *LastAccess;
    long *tag;
}SE;
int hit, miss, eviction, s, E, b;
char *file = NULL;
struct SetEntry *Setptr;
long clock_time = 0;

void get_opt(int argc, char **argv)
{
    int ch;
    while (~(ch = getopt(argc, argv, "s:E:b:t:")))
        switch (ch)
        {
            case 's': s = atoi(optarg); break;
            case 'E': E = atoi(optarg); break;
            case 'b': b = atoi(optarg); break;
            case 't': file = optarg; break;
            default: puts("Illegal Optargs"); exit(0);
        }
}

void init()
{
    int S = 1<<s;
    if (S <= 0)
    {
        fprintf(stderr, "Non-positive S\n");
        exit(0);
    }
    Setptr = (SE*)malloc(S * sizeof(SE));

    for (int i = 0; i < S; ++i)
    {
        Setptr[i].val = (int*)malloc(E * sizeof(int));
        Setptr[i].LastAccess = (clock_t*)malloc(E * sizeof(clock_t));
        Setptr[i].tag = (long*)malloc(E * sizeof(long));

        for (int j = 0 ; j < E; ++j)
        {
            Setptr[i].val[j] = 0;
            Setptr[i].LastAccess[j] = 0;
            Setptr[i].tag[j] = 0;
        }
    }
}

void gc()
{
    int S = 1<<s;
    Setptr = (SE*)malloc(S * sizeof(SE));

    for (int i = 0; i < S; ++i)
    {
        free(Setptr[i].val);
        free(Setptr[i].LastAccess);
        free(Setptr[i].tag);
    }
    free(Setptr);
}

int getset(void *addr)
{
    return (int)(((long)addr >> b) & ((1<<s)-1));
}

long gettag(void *addr)
{
    return (long)((long)addr >> (s + b));
}

void Load(void *addr, int size)
{
    int Setnum = getset(addr);
    printf("Setnum = %d, ", Setnum);
    SE *now = &Setptr[Setnum];

    int i, full = 1, empty_item = 0, last_item = 0;
    int last_time = now->LastAccess[0];

    for (i = 0; i < E; ++i)
        if (now->val[i] == 1 && gettag(addr) == now->tag[i])
        {
            now->LastAccess[i] = ++clock_time;
            break;
        } else if (!now->val[i])
        {
            full = 0;
            empty_item = i;
        } else if (now->LastAccess[i] < last_time)
        {
            last_item = i;
            last_time = now->LastAccess[i];
        }

    if (i == E)
    {
        ++miss;
        if (full)
        {
            now->tag[last_item] = gettag(addr);
            now->LastAccess[last_item] = ++clock_time;
            ++eviction;
        } else
        {
            now->val[empty_item] = 1;
            now->tag[empty_item] = gettag(addr);
            now->LastAccess[empty_item] = ++clock_time;
        }
    } else ++hit;
}

void Store(void *addr, int size)
{
    int Setnum = getset(addr);
    printf("Setnum = %d, ", Setnum);
    SE *now = &Setptr[Setnum];

    int i;
    for (i = 0; i < E; ++i)
        if (now->val[i] == 1 && gettag(addr) == now->tag[i])
        {
            now->LastAccess[i] = ++clock_time;
            break;
        }
    if (i == E) Load(addr, size);
    else ++hit;
}

void Modify(void *addr, int size)
{
    int Setnum = getset(addr);
    printf("Setnum = %d, ", Setnum);
    Load(addr, size);
    Store(addr, size);
}

int main(int argc, char **argv)
{
    get_opt(argc, argv);
    init();
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening files\n");
        exit(0);
    }

    char op[MAXLINE], buf[MAXLINE];
    void *addr;
    int size;
    while (fgets(buf, MAXLINE, fp) != NULL)
    {
        sscanf(buf, "%s %p,%d", op, &addr, &size);
        if (*op == 'L') Load(addr, size);
        else if (*op == 'M') Modify(addr, size);
        else if (*op == 'S') Store(addr, size);
    }
    gc();
    printSummary(hit, miss, eviction);
    return 0;
}
