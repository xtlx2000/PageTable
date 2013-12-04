pager: modules.c modules.h pager.c student_func.c interface.c
	gcc pager.c -o pager 

clean:
	rm -f pager
