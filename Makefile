pager: modules.c pager.c
	gcc pager.c -o pager 

clean:
	rm -f pager
