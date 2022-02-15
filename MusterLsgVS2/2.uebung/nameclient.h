/*************************************************************
 *                                                           *
 * Simple Nameserver --- Client Interface                    *
 * 1995 by Frank Kargl                                       *
 *                                                           *
 *************************************************************/

#ifndef SNS
#define SNS

#define NAMESERVER	"memory"
#define	NAMEPORT	1234

/*** Bind(Name,Adresse); traegt Namen in NS ein ***/
/*** Rueckgabewerte :  1-eingetragen ; 0-Fehler ***/

extern int Bind(char *,char *);

/*** Unbind(Name); loescht Namen aus NS-Cache ***/
/*** Rueckgabewerte :  1-geloescht ; 0-Fehler ***/

extern int Unbind(char *);

/*** Lookup(Name); liefert IP Adresse zurueck ***/
/*** Rueckgabewerte: NULL-Fehler ; ptr-String ***/

extern char *Lookup(char *);

#endif
