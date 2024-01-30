#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#define STUDY_DAYS 2 \

struct date {
    int day, month, year;
};

struct subject {
    char* name;
    char* code;
};

struct schedule {   
    struct date datum;
    struct subject sub[12];
};

struct student {
    char* login;
    char* name;
    char* surname;
    struct schedule sch[STUDY_DAYS];
};

int compare_dates(struct date *d1, struct date *d2) {
    if (d1->year == d2->year) {
        if (d1->month == d2->month) {
            if (d1->day == d2->day) {
                return 0;
            } else if (d1->day < d2->day) {
                return -1;
            } else {
                return 1;
            }
        } else if (d1->month < d2->month) {
            return -1;
        } else {
            return 1;
        }
    } else if (d1->year < d2->year) {
        return -1;
    } else {
        return 1;
    }
}

bool has_subject (struct student* s1, char* subject) {
    //printf("Entered 1!\n");
    for (int i = 0; i < STUDY_DAYS; i++) {
        int g = 0;
        if (s1->sch[i].sub[g].name != '\0') {
            //printf("strcmp: %d\n", strcmp(subject, s1->sch[i].sub[g].name));
            if (strcmp(subject, s1->sch[i].sub[g].name) == 0 && s1->sch[i].sub[g].name != '\0') {
                return true;
            } else {
                g++;
            }
        }
    }
    return false;
}

int amount_of_days(struct date *d1, struct date *d2) {
    if (d1->month == d2->month) {
        if (d1->day > d2->day) {
            return d1->day - d2->day;
        } else if (d1->day < d2->day) {
            return d2->day - d1->day;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

int main() {
    struct date d1 = {3, 4, 2021};
    struct date d2 = {21, 4, 2021};
    struct subject sj1 = {"Pocitacovy seminar", "ISC"};
    struct subject sj2 = {"Matematicky seminar", "ISM"};
    struct subject sj3 = {"Fyzikalni seminar", "IFS"};
    struct student s1;
    s1.login = "serzhprap00";
    s1.name = "Serzhant";
    s1.surname = "Praporschikov";
    s1.sch[0].datum = d1;
    s1.sch[0].sub[0] = sj1;
    s1.sch[0].sub[1] = sj2;
    s1.sch[1].datum = d2;
    s1.sch[1].sub[0] = sj3;
    printf("d1: %d.%d.%d\nd2: %d.%d.%d\n", d1.day, d1.month, d1.year, d2.day, d2.month, d2.year);
    //printf("compare: %d\n", compare_dates(&d1, &d2));
    printf("s1 sub: %s\n", s1.sch[0].sub[1].name);
    printf("amount of days: %d\n", amount_of_days(&d1, &d2));

    if (has_subject(&s1, "Fyzikalni seminar")) {
        printf("TRUE!\n");
    } else {
        printf("FALSE!\n");
    }
    return 0;
}