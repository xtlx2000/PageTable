#pager: pager.c modules.c modules.h
#	gcc pager.c modules.c modules.h -o pager 

pager: pager.c
	gcc pager.c -o pager 

clean:
	rm -f pager
