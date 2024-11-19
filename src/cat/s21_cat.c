#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct flgs {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
} flgs;

static void set_option(int argc, char* argv[], flgs* flg);
static void read_file(int argc, char* argv[], flgs* flg);

int main(int argc, char* argv[]) {
  flgs flg = {0};
  set_option(argc, argv, &flg);
  read_file(argc, argv, &flg);

  return EXIT_SUCCESS;
}

static void set_option(int argc, char* argv[], flgs* flg) {
  char current_option;
  int index = 0;

  static struct option long_option[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((current_option =
              getopt_long(argc, argv, "beEnstTv", long_option, &index)) != -1) {
    switch (current_option) {
      case 'b':
        flg->b = true;
        break;
      case 'e':
        flg->e = true;
        flg->v = true;
        break;
      case 'E':
        flg->e = true;
        break;
      case 'n':
        flg->n = true;
        break;
      case 's':
        flg->s = true;
        break;
      case 't':
        flg->t = true;
        flg->v = true;
        break;
      case 'T':
        flg->t = true;
        break;
      case 'v':
        flg->v = true;
        break;
      case '?':
        fprintf(stderr, "usage: cat [-benstTv] [file ...]\n");
        exit(EXIT_FAILURE);
    }
  }
  if (flg->b == true) flg->n = false;
}

static void read_file(int argc, char* argv[], flgs* flg) {
  while (optind < argc) {
    int line_number = 1;
    char prev_char = '\n';
    int empty_str_count = 0;

    char c;
    FILE* file = fopen(argv[optind], "r");
    if (file == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[optind]);
      optind++;
      continue;
    }
    while ((c = fgetc(file)) != EOF) {
      if (flg->s && prev_char == '\n' && c == '\n') {
        empty_str_count++;
        if (empty_str_count > 1) {
          continue;
        }
      } else {
        empty_str_count = 0;
      }

      if (prev_char == '\n' && ((flg->b && c != '\n') || flg->n)) {
        printf("%6d\t", line_number++);
      }

      if (flg->e && c == '\n') putchar('$');

      if (flg->t && c == '\t') {
        putchar('^');
        c = 'I';
      }
      if (flg->v && ((c >= 0 && c < 9) || (c > 10 && c < 32))) {
        putchar('^');
        c = c + 64;
      } else if (flg->v && c == 127) {
        putchar('^');
        c = 63;
      }

      putchar(c);
      prev_char = c;
    }
    fclose(file);
    optind++;
  }
}
