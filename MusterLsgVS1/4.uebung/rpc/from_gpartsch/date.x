program TEXT_SERVER {
	version TEXT_VERS {
		int	ADD_ELEMENT(string) =1;
		string	SHOW_ELEMENT(int) = 2;
	} = 1;
} = 0x32345678;

