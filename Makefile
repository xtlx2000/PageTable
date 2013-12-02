pager: modules.c modules.h pager.c
	gcc pager.c -o pager 

clean:
	rm -f pager
