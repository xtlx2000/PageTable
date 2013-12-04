pager: modules.c pager.h pager.c student_func.c interface.c
	gcc pager.c -o pager 

clean:
	rm -f pager
