LIB=-lrpcsvc -lnsl

txt:
	cc -c -o txt.o -g txt.c
	cc -c txt_clnt.c
	cc -c txt_xdr.c
	cc -o txt txt.o txt_clnt.o txt_xdr.o $(LIB)

txt_svc:
	cc -c -o txt_svc_proc.o txt_svc_proc.c
	cc -c txt_svc.c
	cc -o txt_svc txt_svc_proc.o txt_svc.o txt_xdr.o $(LIB)

