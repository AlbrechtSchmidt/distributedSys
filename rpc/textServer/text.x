program PROG
{
	version VERS 
	{
		string	ShowElement(int)    = 1;
		int     AddElement (string) = 2;
		void    initArray(void)     = 3;
		int     Serverpid(void)     = 4;
	} = 1;
} = 0x30000000;

