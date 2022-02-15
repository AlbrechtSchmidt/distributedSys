#include <stdio.h>
#include <rpc/rpc.h>
#include <ctype.h>
#include "text.h"

main(argc, argv)  int argc; char * argv[]; 
{
	CLIENT	*cl;
	char	*server;
        int     *i_res, *i_parameter, temp;
	char	**s_result;
        char    buf[50], ch[10];
        char    *txt;
        char ** ptr;
       
	if (argc != 2) 
        { 
                fprintf(stderr,"Aufruf: %s hostname\n", argv[0]);
		exit(1);
	}
        
        server= (char *) malloc (strlen (argv[1]));
	strcpy(server, argv[1]);

	if ((cl = clnt_create(server,PROG,VERS,"udp")) == NULL) 
        {
		clnt_pcreateerror(server);
		exit(2);
	}
 

        do
        { 
          printf("\n\n");
          printf("(E)ingabe        \n");
          printf("(A)usgabe        \n");
          printf("(I)nitialisieren \n");
          printf("(Q)uit           \n");
          printf("(K)ill & Quit    \n");
          printf(">>>>> ");
          gets(ch); 
          printf("\n");

          switch(toupper(ch[0]))
          {
               case 'E' : printf(" Eingabetext >> ");
                          gets(buf);
                          txt = (char *) malloc (strlen (buf));
                          strcpy(txt, buf);
                          ptr = &txt;
                          i_res = addelement_1(ptr, cl);
                          temp = *i_res;
                          printf("\n Text ist unter Nummer %i  eingefuegt !!! \n", temp);
                          break;
               case 'A' : printf(" Textnummer >> ");
	                  gets(buf);
                          temp = atoi(buf); 
                          i_parameter = &temp;
                          printf("\n");
	
	                  if ((s_result = showelement_1(i_parameter,cl)) == NULL) 
                          {
		               clnt_perror(cl,server);
		               exit(4);
	                  }

	                  printf(" Textnummer %i auf \"%s\"  ist : >> %s <<\n"
                                      ,*i_parameter,server,*s_result);
                          break;
               case 'I' : initarray_1(NULL, cl);
                          break;
               case 'Q' : clnt_destroy(cl);
	                  exit(0);
               case 'K' : i_res = serverpid_1(NULL, cl); 
                          temp = *i_res;
                          if (temp == 0) 
                          {
                             clnt_destroy(cl);
                             printf("\n Send Kill \n");
                             printf("\n Server is going down in 1 Sec !!!\n\n");
                             exit(0);
                           }
                           else printf("\n Try it again !!\n");
               default  : printf("\n\n Falsche Eingabe !!! \n"); 
          }
        }
        while(1==1); /* Endlosschleife: Abbruch durch EXIT bei case 'Q' ! */

}

