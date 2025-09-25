#include "minishell.h"
/*
unset

Jokaiselle argumentille:

Validointi.

Jos kelvollinen → poista listasta (huomioi jos pääsolmu poistetaan).

Ei virhettä jos avainta ei ole.

Paluuarvo: virheellisestä tunnisteesta 1, muuten 0.*/

int	run_unset()
{
	if (!is_builtin_valid(arg))
	{
    	print_invalid_identifier("unset", arg);
    	shell->last_status = 1;
    	continue;
	}

}