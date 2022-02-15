  n=sscanf(msg, "%s%s%s%s%s", buf[0], buf[1], buf[2], buf[3], buf[4]);
        
        printf("\n n=%i \n", n);
        printf("\n buf[0]= %s", buf[0]);
        printf("\n buf[1]= %s", buf[1]);
        printf("\n buf[2]= %s", buf[2]);
        printf("\n buf[3]= %s", buf[3]);
        printf("\n buf[4]= %s", buf[4]);
 
 
        if (n > 3) strcpy(retString, "Too many Args or Command not known !!!");
        
        else
        {
        if ( (strncmp("Bind", buf[0],1)==0) )
        {
           if (n==3)
           {
             ret = binding(buf[1],buf[2]);
             if (ret=1) strcpy(retString, "Binding ok !!!");
                else strcpy(retString, "out of Memory -- Binding fail !!!");
           }
           else strcpy(retString, "Bind with wrong Args !!!");
        } 
        else
        { 
           if (strncmp("Lookup", buf[0], 1)==0)
           {
             if (n==2)
             {
               /* printf("\n in main : %s \n", buf[1]); */
               lookup(buf[1]);
             }
             else strcpy(retString, "Lookup with wrong Args !!!");
           }
           else
           {
               if (strncmp("Unbind", buf[0], 1)==0)
               {
                  if (n==2)
                  {
                    ret = unbind(buf[1]);
                    if (ret==1) strcpy(retString, "unbinding ok !!!");
                      else strcpy(retString, "Not found -- unbinding fail !!!");
 
                  }
                  else strcpy(retString, "Unbind with wrong Args !!!");
               }
               else strcpy(retString,"Command not known !!!");
           }
        } 
       } 
        sendto(sock, retString, sizeof(retString), 0,
                   (struct sockaddr *) & client, sizeof(client));
        printf( "\n  ... send: %s \n", retString);

