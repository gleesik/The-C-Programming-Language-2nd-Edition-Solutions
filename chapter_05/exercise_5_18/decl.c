#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

int get_token(void);

enum
{
  NAME,
  PARENS,
  BRACKETS
};

int token_type;

char token[MAX_TOKEN_LEN];
char name[MAX_TOKEN_LEN];
char data_type[MAX_TOKEN_LEN];
char out[1000];

int main(void)
{
  int current_token_type;
  while ((current_token_type = get_token()) != EOF)
  {
    strcpy(data_type, token);
    out[0] = '\0';

    printf("%s: %s %s\n", name, out, data_type);
  }

  return EXIT_SUCCESS;
}

int get_token(void)
{
  int c;
  char *token_p = token;

  while ((c = getc(stdin)) == ' ' || c == '\t')
    ;

  if (c == '(')
  {
    if ((c = getc(stdin)) == ')')
    {
      strcpy(token, "()");
      return token_type = PARENS;
    }
    else
    {
      ungetc(c, stdin);
      return token_type = c;
    }
  }
  else if (c == '[')
  {
    *token_p++ = c;
    while ((*token_p++ = getc(stdin)) != ']')
      ;
    *token_p = '\0';
    return token_type = BRACKETS;
  }
  else if (isalpha(c))
  {
    *token_p++ = c;
    while (isalnum(c = getc(stdin)))
    {
      *token_p++ = c;
    }
    *token_p = '\0';
    ungetc(c, stdin);
    return token_type = NAME;
  }

  return token_type = c;
}
