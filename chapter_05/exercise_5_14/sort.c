#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NR_OF_LINES 5000

int parse_arg_list(int argc, char *argv[]);

size_t read_lines(char *line_ptr[], const size_t max_nr_of_lines);
void write_lines(char *line_ptr[], const size_t nr_of_lines);

int numcmp(const char *s1, const char *s2);
void swap(void *v[], size_t i, size_t j);
void eqsort(void *v[], size_t start, size_t end, int (*comp)(void *, void *), int order);

int order = 1; // 1 ascendent, -1 descendent
int (*comp)(const char *, const char *) = strcmp;

int main(int argc, char *argv[])
{
  if (!parse_arg_list(argc, argv))
  {
    puts("ERROR: Invalid arguments.");
    return EXIT_FAILURE;
  }

  size_t nr_of_lines;
  char *line_ptr[MAX_NR_OF_LINES];

  if ((nr_of_lines = read_lines(line_ptr, MAX_NR_OF_LINES)) != -1)
  {
    eqsort((void **)line_ptr, 0, nr_of_lines - 1, (int (*)(void *, void *))comp, order);
    write_lines(line_ptr, nr_of_lines);
  }
  else
  {
    puts("ERROR: Input too large.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int parse_arg_list(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    size_t arg_len = strlen(argv[i]);
    if (arg_len > 1 && argv[i][0] == '-')
    {
      for (int j = 1; j < arg_len; ++j)
      {
        switch (argv[i][j])
        {
        case 'n':
          comp = numcmp;
          break;

        case 'r':
          order = -1;
          break;

        default:
          return 0;
          break;
        }
      }
    }
    else
    {
      return 0;
    }
  }

  return 1;
}

size_t read_lines(char *line_ptr[], const size_t max_nr_of_lines)
{
  size_t line_length;
  size_t nr_of_lines = 0;
  size_t bufsize = 0;

  char *current_line = NULL;
  char *current_line_copy = NULL;

  while ((line_length = getline(&current_line, &bufsize, stdin)) != -1)
  {
    if (nr_of_lines >= max_nr_of_lines || (current_line_copy = (char *)malloc(line_length * sizeof(char))) == NULL)
    {
      return -1;
    }
    else
    {
      current_line[line_length - 1] = '\0';
      strcpy(current_line_copy, current_line);
      line_ptr[nr_of_lines++] = current_line_copy;
    }
  }

  free(current_line);

  return nr_of_lines;
}

void write_lines(char *line_ptr[], const size_t nr_of_lines)
{
  for (size_t i = 0; i < nr_of_lines; ++i)
  {
    puts(line_ptr[i]);
    free(line_ptr[i]);
  }
}

int numcmp(const char *s1, const char *s2)
{
  double nr1 = atof(s1);
  double nr2 = atof(s2);

  if (nr1 < nr2)
  {
    return -1;
  }
  else if (nr1 > nr2)
  {
    return 1;
  }

  return 0;
}

void swap(void *v[], size_t i, size_t j)
{
  void *temp;
  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void eqsort(void *v[], size_t start, size_t end, int (*comp)(void *, void *), int order)
{
  if ((long)start >= (long)end)
  {
    return;
  }

  swap(v, start, (start + end) / 2);

  size_t last = start;
  for (size_t i = start + 1; i <= end; ++i)
  {
    if (order * (*comp)(v[i], v[start]) < 0)
    {
      swap(v, ++last, i);
    }
  }

  swap(v, start, last);
  eqsort(v, start, last - 1, comp, order);
  eqsort(v, last + 1, end, comp, order);
}

// NOTE: run: ./sort -nr < file_in.txt
