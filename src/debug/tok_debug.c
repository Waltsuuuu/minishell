#include "minishell.h"

void print_tokens(const t_input *in)
 {
 	int i;

 	if (!in || !in->tokens)
 		return ;
 	i = 0;
 	printf("\n TOKENS \n");
 	while (i < in->n_tokens)
 	{
 		printf("[%d] type=%d pos=%d text=\"%s\" was_quoted = \"%d\"\n",
 			i, in->tokens[i].type, in->tokens[i].pos,
 			in->tokens[i].text ? in->tokens[i].text : "(null)", in->tokens[i].was_quoted);
 		i++;
 	}
 }

// print_tokens(&shell.input);						// Token debug
