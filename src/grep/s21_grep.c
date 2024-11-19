#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 999999

typedef struct flags {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} flags;

void set_option(int argc, char *argv[], flags *flag, char pattern[BUF_SIZE]);
void save_pattern(int f, const char *way, char pattern[BUF_SIZE], int *i);
void grep_file(int argc, char *argv[], flags *flag, char pattern[BUF_SIZE]);
void regex_pettern(regex_t regex, flags *flag, FILE *file,
                   const char *file_name);

int main(int argc, char *argv[]) {
  flags flag = {0};
  char pattern[BUF_SIZE] = {0};

  set_option(argc, argv, &flag, pattern);
  if ((argc - optind) >= 1) grep_file(argc, argv, &flag, pattern);

  return EXIT_SUCCESS;
}

void set_option(int argc, char *argv[], flags *flag, char pattern[BUF_SIZE]) {
  int c = 0, i = 0, f;
  while ((c = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1) {
    switch (c) {
      case 'e':
        flag->e = true;
        f = 0;
        save_pattern(f, optarg, pattern, &i);
        break;
      case 'i':
        flag->i = true;
        break;
      case 'v':
        flag->v = true;
        break;
      case 'c':
        flag->c = true;
        break;
      case 'l':
        flag->l = true;
        break;
      case 'n':
        flag->n = true;
        break;
      case 'h':
        flag->h = true;
        break;
      case 's':
        flag->s = true;
        break;
      case 'f':
        flag->f = true;
        f = 1;
        save_pattern(f, optarg, pattern, &i);
        break;
      case 'o':
        flag->o = true;
        break;
      default:
        fprintf(stderr, "usage: s21_grep -flag e:ivclnhsf:o");
        exit(EXIT_FAILURE);
    }
  }
  if (i > 0 && pattern[i - 1] == '|') pattern[i - 1] = '\0';
}

void save_pattern(int f, const char *way, char pattern[BUF_SIZE], int *i) {
  char c;
  char prev_s = '\n';
  if (f == 1) {
    FILE *file = fopen(way, "r");
    if (!file) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", way);
      exit(EXIT_FAILURE);
    }
    while ((c = fgetc(file)) != EOF) {
      if (prev_s == '\n' && c == '\n') {
        pattern[(*i)++] = '.';
        pattern[(*i)++] = '*';
        pattern[(*i)++] = '|';
      } else if (c == '\r' || c == '\n') {
        pattern[(*i)++] = '|';
      } else {
        pattern[(*i)++] = c;
      }
      prev_s = c;
    }
    fclose(file);
  } else {
    while ((c = *way++) != '\0') {
      pattern[(*i)++] = c;
    }
    pattern[(*i)++] = '|';
  }
}

void grep_file(int argc, char *argv[], flags *flag, char pattern[BUF_SIZE]) {
  if (!flag->e && !flag->f) {
    strcpy(pattern, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) flag->h = true;
  for (int i = optind; i < argc; i++) {
    regex_t regex;
    int cflags = (flag->i) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      if (!flag->s)
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[i]);
    } else {
      regcomp(&regex, pattern, cflags);
      regex_pettern(regex, flag, file, argv[i]);
      regfree(&regex);
      fclose(file);
    }
  }
}

void regex_pettern(regex_t regex, flags *flag, FILE *file,
                   const char *file_name) {
  char buffer[BUF_SIZE] = {0};
  regmatch_t pmatch[1];
  int line_number = 0, match_sum = 0;

  while (fgets(buffer, BUF_SIZE - 1, file) != NULL) {
    int result = regexec(&regex, buffer, 1, pmatch, 0);
    int match = 0;
    line_number++;

    if ((result == 0 && !flag->v) || (result == REG_NOMATCH && flag->v))
      match = 1;

    match_sum += match;
    if (!flag->l && match) {
      if (!flag->h && !flag->c) printf("%s:", file_name);
      if (!flag->c) {
        if (flag->n) printf("%d:", line_number);
        if (!flag->o) {
          printf("%s", buffer);
          if (buffer[strlen(buffer) - 1] != '\n') printf("\n");
        } else {
          char *match_str = buffer;
          while (result == 0) {
            printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                   match_str + pmatch[0].rm_so);
            match_str += pmatch[0].rm_eo;
            result = regexec(&regex, match_str, 1, pmatch, 0);
          }
        }
      }
    }
  }
  if (flag->c && !flag->l) {
    if (!flag->h) printf("%s:", file_name);
    printf("%d\n", match_sum);
  }
  if (flag->l && match_sum > 0) {
    printf("%s\n", file_name);
  }
}
